/// @file
///
/// The `Channel<T>` class.

#ifndef HORUS_FUTURE_CHANNEL_H_
#define HORUS_FUTURE_CHANNEL_H_

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <utility>

#include "horus/event_loop/waker.h"
#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/future/poll.h"
#include "horus/internal/attributes.h"
#include "horus/types/in_place.h"
#include "horus/types/list.h"
#include "horus/types/one_of.h"

namespace horus {

/// Exception thrown by sides of the channel when the other side has been destroyed.
class ChannelClosedError final : public std::runtime_error {
 public:
  /// Constructs the error.
  ChannelClosedError() : std::runtime_error{"channel is closed"} {}
};

/// A multiple-producer-single-consumer channel of communication used to send data from any thread
/// to the event loop.
template <class T>
class Channel final {
 public:
  /// The sender side of the channel, which can be used outside of the event loop.
  class Sender;
  /// The receiver side of the channel, which must be used in the event loop.
  class Receiver;

  /// Capacity given to `Channel()` to obtain an unbounded channel.
  static constexpr std::size_t kUnbounded{-1UL};

  /// Initializes a bounded `Channel` which can queue up to `capacity` messages without blocking.
  /// Pass `kUnbounded` for an unbounded channel.
  ///
  /// @throws std::bad_alloc If the data for the channel could not be allocated.
  explicit Channel(std::size_t capacity) noexcept(false)
      : Channel{std::make_shared<Shared>(capacity)} {
    assert(capacity > 0);
  }

  /// Returns the `Sender` for this channel.
  ///
  /// The `Sender` may be moved out of its current location.
  Sender& GetSender() noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND { return sender_; }

  /// Returns the `Receiver` for this channel.
  ///
  /// The `Receiver` may be moved out of its current location.
  Receiver& GetReceiver() noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND { return receiver_; }

 private:
  /// Type of `Shared::{first,last}_send_waker` and `SendFuture::waker_`.
  struct WakerNode {
    /// The actual waker value.
    horus_internal::FutureWaker waker;
    /// A link to its `WakerList`.
    horus_internal::IntrusiveListLink<WakerNode> link;
  };
  /// A list of `WakerNode`s.
  using WakerList = horus_internal::IntrusiveListByField<WakerNode, &WakerNode::link>;

  /// Data shared between both ends of the channel.
  struct Shared {
    /// Constructs the shared state.
    explicit Shared(std::size_t cap) noexcept : capacity{cap} {}

    /// The capacity of the `queue`.
    const std::size_t capacity;

    /// A linked list of pending `SendFuture` wakers.
    WakerList send_wakers;
    /// A linked list of pending `ReceiveFuture` wakers.
    WakerList recv_wakers;

    /// Mutex which guards the `queue`.
    std::mutex mutex;
    /// Condition variable used to notify of state changes.
    std::condition_variable queue_cv;
    /// The queue of items to receive.
    std::deque<T> queue;
    /// Whether one end of the queue was shutdown; whichever side of the queue observes this value
    /// to be true should free the queue.
    std::atomic<bool> shutdown{false};

    /// Wakes up a pending sender. Requires `mutex` to be held.
    void WakeSender() noexcept {
      WakerNode* const waker_node{send_wakers.Front()};
      if (waker_node != nullptr) {
        waker_node->waker.Wake();
      } else {
        queue_cv.notify_one();
      }
    }

    /// Wakes up a pending receiver. Requires `mutex` to be held.
    void WakeReceiver() noexcept {
      WakerNode* const waker_node{recv_wakers.Front()};
      if (waker_node != nullptr) {
        waker_node->waker.Wake();
      }
    }
  };

  /// Constructs a `Channel`.
  explicit Channel(const std::shared_ptr<Shared>& shared) noexcept
      : sender_{shared}, receiver_{shared} {}

  /// Throws `ChannelClosedError` if the channel is closed.
  static Shared& ThrowIfClosed(const std::shared_ptr<Shared>& shared) {
    if (shared == nullptr || shared->shutdown) {
      throw ChannelClosedError{};
    }
    return *shared;
  }

  /// Releases ownership of one of the sides of the shared state.
  static void Release(std::shared_ptr<Shared>& shared) {
    if (shared == nullptr) {
      return;
    }
    shared->shutdown = true;
    shared = nullptr;
  }

  /// @see GetSender()
  Sender sender_;
  /// @see GetReceiver()
  Receiver receiver_;
};

/// @copydoc Channel::Sender
template <class T>
using ChannelSender = typename Channel<T>::Sender;
/// @copydoc Channel::Receiver
template <class T>
using ChannelReceiver = typename Channel<T>::Receiver;

template <class T>
class Channel<T>::Sender final {
 public:
  /// Future returned by `Send()`.
  class SendFuture;

  /// (Internal use only) Constructs the `Sender`.
  explicit Sender(const std::shared_ptr<Shared>& shared) noexcept(false) : shared_{shared} {}

  /// Cannot copy.
  Sender(const Sender&) = delete;
  /// Cannot copy.
  Sender& operator=(const Sender&) = delete;
  /// Move constructor.
  Sender(Sender&&) noexcept = default;
  /// Move assignment operator.
  Sender& operator=(Sender&&) noexcept = default;

  /// Destroys the sender, closing the channel.
  ~Sender() noexcept { Release(shared_); }

  /// Sends a value constructed with the given `args` to the receiver.
  SendFuture Send(T&& value, const FutureInfo& info = CurrentFutureInfo()) {
    return SendFuture{shared_, std::move(value), info};
  }

  /// Sends a value constructed with the given `args` to the receiver. If the channel is at
  /// capacity, waits until it no longer is. This function may only be called _outside_ of the event
  /// loop that `Receiver` awaits on.
  template <class... Args>
  void SendOrWait(Args&&... args);

  /// Sends a value constructed with the given `args` to the receiver.
  ///
  /// Due to Autosar, rvalue `args` will be consumed no matter whether the send succeeds or not,
  /// although this may change in the future.
  bool TrySend(T&& value);

 private:
  /// A (non-null) pointer to the shared state.
  std::shared_ptr<Shared> shared_;
};

template <class T>
class Channel<T>::Receiver final {
 public:
  /// Future returned by `Receive()`.
  class ReceiveFuture;

  /// (Internal use only) Constructs the `Receiver`.
  explicit Receiver(const std::shared_ptr<Shared>& shared) noexcept(false) : shared_{shared} {}

  /// Cannot copy.
  Receiver(const Receiver&) = delete;
  /// Cannot copy.
  Receiver& operator=(const Receiver&) = delete;
  /// Move constructor.
  Receiver(Receiver&&) noexcept = default;
  /// Move assignment operator.
  Receiver& operator=(Receiver&&) noexcept = default;

  /// Destroys the receiver, closing the channel.
  ~Receiver() noexcept { Release(shared_); }

  /// Returns a future which will complete with the next sent value.
  ReceiveFuture Receive(const FutureInfo& info = CurrentFutureInfo()) {
    return ReceiveFuture{shared_, info};
  }

 private:
  /// A pointer to the shared state.
  std::shared_ptr<Shared> shared_;
};

/// @copydoc Channel::Sender::SendFuture
template <class T>
using ChannelSendFuture = typename Channel<T>::Sender::SendFuture;

template <class T>
class Channel<T>::Sender::SendFuture final : public Future<void> {
 public:
  /// (Internal use only) Constructs the `SendFuture`.
  explicit SendFuture(const std::shared_ptr<Shared>& shared, T&& value,
                      const FutureInfo& info) noexcept
      : shared_{shared}, value_{InPlaceType<T>, std::move(value)}, info_{info} {}

  /// Not copyable.
  SendFuture(const SendFuture&) = delete;
  /// Not copyable.
  SendFuture& operator=(const SendFuture&) = delete;
  /// Move constructor.
  SendFuture(SendFuture&&) noexcept = default;
  /// Move assignment operator.
  SendFuture& operator=(SendFuture&&) noexcept = default;

  /// Destroys the future.
  ~SendFuture() noexcept final;

  /// Polls the future until the message is sent through the channel.
  PollResult<void> UnsafePoll(PollContext& context) final;

 private:
  /// A pointer to the shared state.
  std::weak_ptr<Shared> shared_;
  /// The waker used to wake up the future.
  WakerNode waker_;
  /// The value to send.
  OneOf<T, void> value_;
  /// Future information.
  FutureInfo info_;
};

/// @copydoc Channel::Receiver::ReceiveFuture
template <class T>
using ChannelReceiveFuture = typename Channel<T>::Receiver::ReceiveFuture;

template <class T>
class Channel<T>::Receiver::ReceiveFuture final : public Future<T> {
 public:
  /// (Internal use only) Constructs the `ReceiveFuture`.
  explicit ReceiveFuture(const std::shared_ptr<Shared>& shared, const FutureInfo& info) noexcept
      : shared_{shared}, info_{info} {}

  /// Not copyable.
  ReceiveFuture(const ReceiveFuture&) = delete;
  /// Not copyable.
  ReceiveFuture& operator=(const ReceiveFuture&) = delete;
  /// Move constructor.
  ReceiveFuture(ReceiveFuture&&) noexcept = default;
  /// Move assignment operator.
  ReceiveFuture& operator=(ReceiveFuture&&) noexcept = default;

  /// Destroys the future.
  ~ReceiveFuture() noexcept final;

  /// Polls the future until a message is sent through the channel.
  PollResult<T> UnsafePoll(PollContext& context) final;

 private:
  /// A pointer to the shared state.
  std::weak_ptr<Shared> shared_;
  /// The waker used to wake up the future.
  WakerNode waker_;
  /// Future information.
  FutureInfo info_;
};

template <class T>
template <class... Args>
void Channel<T>::Sender::SendOrWait(Args&&... args) {
  Shared& shared{ThrowIfClosed(shared_)};
  std::unique_lock<std::mutex> lock{shared.mutex};
  shared.queue_cv.wait(lock, [&shared]() noexcept -> bool {
    return shared.shutdown || (shared.queue.size() < shared.capacity &&
                               shared.send_wakers.IsEmpty());  // Prioritize futures.
  });
  if (shared.shutdown) {
    throw ChannelClosedError{};
  }
  shared.queue.emplace_back(std::forward<Args>(args)...);
  shared.WakeReceiver();
}

template <class T>
bool Channel<T>::Sender::TrySend(T&& value) {
  Shared& shared{*shared_};
  const std::unique_lock<std::mutex> lock{shared.mutex};
  if (shared.shutdown || shared.queue.size() == shared.capacity ||
      shared.send_wakers.IsEmpty()) {  // Prioritize futures.
    return false;
  }
  shared.queue.push_back(std::move(value));
  shared.WakeReceiver();
  return true;
}

template <class T>
PollResult<void> Channel<T>::Sender::SendFuture::UnsafePoll(PollContext& context) {
  const PollContext::Trace trace{context, "ChannelSend", info_};

  T* const value{value_.template TryAs<T>()};
  if (value == nullptr) {
    throw PolledAfterCompletionError{};
  }
  const std::shared_ptr<Shared> shared_ptr{shared_.lock()};
  Shared& shared{ThrowIfClosed(shared_ptr)};
  {
    const std::unique_lock<std::mutex> lock{shared.mutex};
    if (shared.queue.size() == shared.capacity) {
      waker_.waker = context.Waker();
      if (waker_.link.prev == nullptr && waker_.link.next == nullptr) {
        // This is the first time we get polled to this point, so we insert `waker_` into the linked
        // list.
        shared.send_wakers.PushBack(waker_);
      }
      return PendingResult<void>();
    }
    shared.send_wakers.Remove(waker_);
    shared.queue.push_back(std::move(*value));
    shared.WakeReceiver();
    value_.template Emplace<void>();  // So that future polls will throw.
  }
  shared_.reset();
  return ReadyResult<void>();
}

template <class T>
Channel<T>::Sender::SendFuture::~SendFuture() noexcept {
  const std::shared_ptr<Shared> shared_ptr{shared_.lock()};
  if (shared_ptr != nullptr) {
    const std::unique_lock<std::mutex> lock{shared_ptr->mutex};
    shared_ptr->send_wakers.Remove(waker_);
  }
}

template <class T>
PollResult<T> Channel<T>::Receiver::ReceiveFuture::UnsafePoll(PollContext& context) {
  const PollContext::Trace trace{context, "ChannelReceive", info_};

  const std::shared_ptr<Shared> shared_ptr{shared_.lock()};
  Shared& shared{ThrowIfClosed(shared_ptr)};
  std::unique_lock<std::mutex> lock{shared.mutex};
  if (shared.queue.empty()) {
    waker_.waker = context.Waker();
    if (waker_.link.prev == nullptr && waker_.link.next == nullptr) {
      // This is the first time we get polled to this point, so we insert `waker_` into the linked
      // list.
      shared.recv_wakers.PushBack(waker_);
    }
    return PendingResult<T>();
  }
  shared.recv_wakers.Remove(waker_);

  T result{std::move(shared.queue.front())};
  shared.queue.pop_front();
  if (shared.queue.size() == shared.capacity - 1) {
    // Queue is no longer full, maybe a sender is waiting?
    shared.WakeSender();
  }
  lock.unlock();  // Since `reset()` below might destroy the shared state.
  shared_.reset();
  return ReadyResult<T>(std::move(result));
}

template <class T>
Channel<T>::Receiver::ReceiveFuture::~ReceiveFuture() noexcept {
  const std::shared_ptr<Shared> shared_ptr{shared_.lock()};
  if (shared_ptr != nullptr) {
    const std::unique_lock<std::mutex> lock{shared_ptr->mutex};
    shared_ptr->recv_wakers.Remove(waker_);
  }
}

}  // namespace horus

#endif  // HORUS_FUTURE_CHANNEL_H_
