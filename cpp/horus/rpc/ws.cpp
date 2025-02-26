#include "horus/rpc/ws.h"

#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXWebSocketCloseInfo.h>
#include <ixwebsocket/IXWebSocketErrorInfo.h>
#include <ixwebsocket/IXWebSocketMessage.h>
#include <ixwebsocket/IXWebSocketMessageType.h>

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <exception>
#include <limits>
#include <memory>
#include <mutex>
#include <new>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

#include "horus/event_loop/event_loop.h"
#include "horus/functional/move_only_function.h"
#include "horus/future/any.h"
#include "horus/future/cancel.h"
#include "horus/future/from_continuation.h"
#include "horus/future/from_poll.h"
#include "horus/future/future.h"
#include "horus/future/map.h"
#include "horus/future/poll.h"
#include "horus/future/then.h"
#include "horus/future/try.h"
#include "horus/internal/attributes.h"
#include "horus/internal/enum.h"
#include "horus/pb/buffer.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/pb/serialize.h"
#include "horus/rpc/endpoint.h"
#include "horus/rpc/retry_policy.h"
#include "horus/strings/logging.h"
#include "horus/strings/str_cat.h"
#include "horus/types/in_place.h"
#include "horus/types/one_of.h"
#include "horus/types/scope_guard.h"
#include "horus/types/string_view.h"

namespace horus {
namespace horus_internal {

std::string AddressPortPairToUrl(StringView address, std::uint16_t port) {
  return StrCat("ws://", address, ":", port);
}

}  // namespace horus_internal

namespace {

/// Implementation of an `RpcEndpoint` which sends and receives data through WebSockets.
class WebSocketRpcEndpoint final : public RpcEndpoint {
 public:
  /// Constructs a `WebSocketRpcEndpoint` which runs in the given `event_loop` and connects to the
  /// given `url`. `Initialize()` must be called on the resulting value.
  WebSocketRpcEndpoint(horus_internal::EventLoop& event_loop, std::string&& url,
                       MessageHandler&& message_handler);

  /// Cannot be copied or moved.
  WebSocketRpcEndpoint(const WebSocketRpcEndpoint&) = delete;
  /// Cannot be copied or moved.
  WebSocketRpcEndpoint& operator=(const WebSocketRpcEndpoint&) = delete;
  /// Cannot be copied or moved.
  WebSocketRpcEndpoint(WebSocketRpcEndpoint&&) = delete;
  /// Cannot be copied or moved.
  WebSocketRpcEndpoint& operator=(WebSocketRpcEndpoint&&) = delete;

  /// Destroys the `WebSocketRpcEndpoint`.
  ~WebSocketRpcEndpoint() final {
    {
      const std::unique_lock<std::mutex> lock{send_mtx_};
      send_shutdown_ = true;
      send_cv_.notify_all();
    }
    send_thread_.join();

    CancelPending();

    if (on_event_ != nullptr) {
      on_event_(on_event_receiver_, LifecycleEvent{InPlaceType<ShutdownEvent>});
      on_event_ = nullptr;
    }
  }

  /// Initializes the shared `WebSocketRpcEndpoint`.
  static void Initialize(std::shared_ptr<WebSocketRpcEndpoint> self);

  /// @copydoc RpcEndpoint::Uri()
  StringView Uri() const noexcept final { return url_; }

  /// @copydoc RpcEndpoint::Send()
  AnyFuture<void> Send(pb::RpcMessage&& message, const RpcOptions& options) noexcept(false) final {
    return SendImpl<void>(std::move(message), options);
  }

  /// @copydoc RpcEndpoint::SendWithResponse()
  AnyFuture<pb::RpcMessage> SendWithResponse(pb::RpcMessage&& message,
                                             const RpcOptions& options) noexcept(false) final {
    return SendImpl<pb::RpcMessage>(std::move(message), options);
  }

  /// @copydoc RpcEndpoint::SetLifecycleEventCallback()
  void SetLifecycleEventCallback(void* receiver,
                                 void (*on_event)(void* receiver,
                                                  LifecycleEvent&& event)) noexcept final {
    on_event_receiver_ = receiver;
    on_event_ = on_event;
  }

  /// Returns whether the WebSocket is connected.
  bool IsConnected() const noexcept { return websocket_.getReadyState() == ix::ReadyState::Open; }

 private:
  /// A pending request to send.
  struct PendingRequest {
    /// The message to send.
    pb::RpcMessage message;

    /// The policy to follow when sending the message.
    RpcRetryPolicy retry_policy;

    /// The continuation through which progress can be reported.
    ///
    /// If this is `Continuation<void>`, this is a one-way RPC. Otherwise, this is a two-way RPC.
    OneOf<void, Continuation<void>, Continuation<pb::RpcMessage>> continuation;
  };

  /// A pending response to receive.
  struct PendingResponse {
    /// The continuation through which progress can be reported.
    Continuation<pb::RpcMessage> continuation;
  };

  /// Data shared with the `send_thread_`.
  struct SharedData {};

  /// The maximum number of pending requests at any given time.
  static constexpr std::size_t kRequestCapacity{64};
  /// The maximum number of pending responses at any given time.
  static constexpr std::size_t kResponseCapacity{32};

  /// Invokes `invocable(*self)` in the event loop.
  template <class F>
  static void InvokeInEventLoop(std::shared_ptr<WebSocketRpcEndpoint>&& self, F invocable) noexcept
      HORUS_SDK_EXCLUDES(send_mtx_);

  /// Delivers a lifecycle event.
  static void DeliverEvent(std::shared_ptr<WebSocketRpcEndpoint>&& self,
                           LifecycleEvent&& event) noexcept HORUS_SDK_EXCLUDES(send_mtx_) {
    InvokeInEventLoop(
        std::move(self), [owned_event{std::move(event)}](
                             const std::shared_ptr<WebSocketRpcEndpoint>& endpoint,
                             horus_internal::EventLoop& /* event_loop */) mutable noexcept {
          if (endpoint->on_event_ != nullptr) {
            try {
              endpoint->on_event_(endpoint->on_event_receiver_, std::move(owned_event));
            } catch (const std::exception& e) {
              Log("exception thrown by WebSocket lifecycle event handler: ", e.what());
            }
          }
        });
  }

  /// Delivers a lifecycle event.
  template <class Event>
  static void DeliverEvent(std::shared_ptr<WebSocketRpcEndpoint>&& self, Event&& event) noexcept
      HORUS_SDK_EXCLUDES(send_mtx_) {
    DeliverEvent(std::move(self), LifecycleEvent{InPlaceType<Event>, std::forward<Event>(event)});
  }

  /// Handles a received request message.
  static void HandleRequestMessage(std::shared_ptr<WebSocketRpcEndpoint>&& self,
                                   pb::RpcMessage&& request_message);

  /// Common implementation of `Send()` and `SendWithResponse()`.
  template <class T>
  AnyFuture<T> SendImpl(pb::RpcMessage&& message, const RpcOptions& options) noexcept(false)
      HORUS_SDK_EXCLUDES(send_mtx_);

  /// Cancels all pending continuations due to a disconnection.
  void CancelPending() noexcept HORUS_SDK_EXCLUDES(send_mtx_);

  /// Function running in `send_thread_`.
  void RunSendThread() noexcept HORUS_SDK_EXCLUDES(send_mtx_);

  /// Object used to invoke callbacks on the event loop.
  horus_internal::EventLoop::Invoker invoker_;
  /// The end URL.
  const std::string url_;
  /// The function to call when a message is received.
  MessageHandler message_handler_;
  /// Handle to the underlying websocket.
  ix::WebSocket websocket_;

  /// Value given to `on_event_()` when an event is emitted.
  void* on_event_receiver_{nullptr};
  /// Function to call when an event is emitted. May be null.
  void (*on_event_)(void*, LifecycleEvent&&){nullptr};

  // Everything below this line is guarded by `send_mtx_`.

  /// Mutex used to send work to the `send_thread_`.
  std::mutex send_mtx_;
  /// Condition variable used to notify the `send_thread_` that pending messages are available or
  /// that it should shut down.
  std::condition_variable send_cv_;
  /// Boolean set to true to indicate that the `send_thread_` should shut down.
  bool send_shutdown_{false};
  /// Queue of messages to send in the `send_thread_`.
  std::deque<PendingRequest> send_queue_;

  /// An array of pending responses.
  std::array<PendingResponse, kResponseCapacity> pending_responses_{};
  /// A bitset indicating whether the response at the corresponding index is used.
  std::bitset<kResponseCapacity> used_pending_responses_;

  // This `std::thread` is not guarded by `send_mtx_`, but must be the last field as it will access
  // `this` from another thread.

  /// Thread which receives messages to send and sends them through the `websocket_`.
  ///
  /// Ideally we would use the event loop rather than a thread here, but we must ensure that
  /// messages are sent **in order**, which is not guaranteed by the thread pool provided by libuv.
  /// This thread will spend most of the time idle and we likely will have few WebSocket connections
  /// active at a time, so this is fine.
  std::thread send_thread_{&WebSocketRpcEndpoint::RunSendThread, this};
};

WebSocketRpcEndpoint::WebSocketRpcEndpoint(horus_internal::EventLoop& event_loop, std::string&& url,
                                           MessageHandler&& message_handler)
    : RpcEndpoint{},
      invoker_{event_loop},
      url_{std::move(url)},
      message_handler_{std::move(message_handler)},
      websocket_{} {
  assert(message_handler_ != nullptr);
}

/// "Forgets" `value`, taking ownership of it and preventing its destructor from running.
template <class T>
constexpr void Forget(T&& value) noexcept {
  static_assert(std::is_nothrow_move_constructible<T>::value, "");
  static_assert(!std::is_reference<T>::value, "");

  alignas(T) std::array<std::uint8_t, sizeof(T)> leaked{};
  static_cast<void>(new (&leaked) T(std::forward<T>(value)));
}

/// Destroys `to_destroy` in another thread so that there will not be a deadlock when destroying its
/// inner `ix::WebSocket`, which is calling the caller.
template <class T>
void DestroyInAnotherThread(T&& to_destroy) noexcept {
  static_assert(!std::is_reference<T>::value, "");
  try {
    // Spawn a thread to do the cleanup somewhere else.
    std::thread{[unused{std::forward<T>(to_destroy)}]() noexcept {
      // Sleep for a while to lower the chances of this thread completing before the caller
      // destroys its `self`.
      std::this_thread::sleep_for(std::chrono::seconds{1});
    }}.detach();
  } catch (...) {
    // We don't have enough resources to spawn a clean-up thread, so we don't really have a choice
    // and will just leak the shared pointer to prevent its destruction.
    Forget(std::forward<T>(to_destroy));
  }
}

// static
void WebSocketRpcEndpoint::Initialize(std::shared_ptr<WebSocketRpcEndpoint> self_ptr) {
  ix::WebSocket& websocket{self_ptr->websocket_};

  websocket.setUrl(self_ptr->url_);
  websocket.setOnMessageCallback([weak_self{std::weak_ptr<WebSocketRpcEndpoint>{self_ptr}}](
                                     const std::unique_ptr<ix::WebSocketMessage>& message) mutable {
    std::shared_ptr<WebSocketRpcEndpoint> self{weak_self.lock()};
    if (self == nullptr) {
      return;
    }
    const auto self_cleanup = Defer([&self]() noexcept {
      if (self == nullptr) {
        return;
      }
      const horus_internal::EventLoop::Invoker& invoker{self->invoker_};
      auto destroy_self =
          [to_destroy{std::move(self)}](horus_internal::EventLoop& /*loop*/) noexcept {};
      try {
        // `self` was not moved; so we may hold the last strong pointer to it. If so, dropping it
        // could cause a deadlock as destroying `ix::WebSocket` requires locking the same mutex
        // which is locked when the `onMessageCallback` callback is called.
        //
        // We give ownership of `self` to the event loop until the next iteration in case this
        // happens.
        if (!invoker.TryInvoke(destroy_self)) {
          // The event loop is shutting down, so instead we have to do something else with `self`.
          DestroyInAnotherThread(std::move(destroy_self));
        }
      } catch (std::bad_alloc const&) {
        // We couldn't create the state necessary for `TryInvoke()`, and don't really have a choice
        // but to leak `self`.
        Forget(std::move(destroy_self));
      }
    });

    switch (message->type) {
      case ix::WebSocketMessageType::Message: {
        if (!message->binary) {
          // Bad event received.
          DeliverEvent(
              std::move(self),
              ErrorEvent{std::make_exception_ptr(std::runtime_error{"received text message"})});
          return;
        }

        // Deserialize message.
        pb::RpcMessage rpc_message;
        try {
          PbBuffer buffer{std::string{message->str}};
          PbReader reader{PbView{std::move(buffer)}};
          rpc_message.DeserializeFrom(reader);
        } catch (...) {
          DeliverEvent(std::move(self), ErrorEvent{std::current_exception()});
          return;
        }

        // Ignore cancellation requests as we don't have a way to handle them right now.
        if (rpc_message.cancel()) {
          return;
        }

        // Dispatch to message handler if this is a request.
        if (rpc_message.method_id() != kRpcResponseMethodId) {
          HandleRequestMessage(std::move(self), std::move(rpc_message));
          return;
        }

        // Find message handler.
        if (rpc_message.request_id() == kOneWayRpcRequestId ||
            rpc_message.request_id() >= kRequestCapacity) {
          DeliverEvent(std::move(self), ErrorEvent{std::make_exception_ptr(
                                            std::runtime_error{"invalid request ID received"})});
          return;
        }

        // Deliver response.
        PendingResponse response{};
        std::size_t const request_id{rpc_message.request_id() - kTwoWayRpcRequestIdMin};
        {
          const std::unique_lock<std::mutex> lock{self->send_mtx_};
          // NOLINTNEXTLINE(*-constant-array-index)
          response = std::move(self->pending_responses_[request_id]);
          self->used_pending_responses_.reset(request_id);
        }
        StringView const error{rpc_message.error().Str()};
        if (error.empty()) {
          static_cast<void>(response.continuation.ContinueWith(std::move(rpc_message)));
        } else {
          static_cast<void>(response.continuation.FailWith(
              std::make_exception_ptr(RpcInternalError{std::string{error}})));
        }
        break;
      }
      case ix::WebSocketMessageType::Open: {
        self->CancelPending();
        DeliverEvent(std::move(self), ConnectedEvent{});
        break;
      }
      case ix::WebSocketMessageType::Close: {
        self->CancelPending();
        DeliverEvent(std::move(self), DisconnectedEvent{std::move(message->closeInfo.reason)});
        break;
      }
      case ix::WebSocketMessageType::Error: {
        DeliverEvent(
            std::move(self),
            ErrorEvent{std::make_exception_ptr(std::runtime_error{message->errorInfo.reason})});
        break;
      }

      case ix::WebSocketMessageType::Ping:
      case ix::WebSocketMessageType::Pong:
      case ix::WebSocketMessageType::Fragment:
        break;

      default:
        throw horus_internal::UnhandledEnumValueError{message->type};
    }
  });

  // Ideally we would manage reconnections ourselves for better control, but:
  // 1. We can't use run() manually because the only way to stop it is with a variable private to
  //    IXWebSocket, so we have to use start() / stop().
  // 2. You cannot run start() without stop()ing first (a boolean is checked, and start() is noop
  //    if you didn't stop).
  // 3. You also cannot stop() in the handler of the CLOSE callback, as this causes a deadlock.
  //
  // The complexity needed to stop() at precisely the right time (so that we can try to start() in
  // Send() while following some precise reconnection strategy) is just too complex, and getting
  // it wrong leads to infinite waits (due to unfulfilled promises), deadlocks, or UB.
  websocket.enableAutomaticReconnection();

  // DEFLATE is quite slow in this implementation + protobuf serialized format is already well
  // optimized for this to have any meaningful benefit. Disabling DEFLATE is much faster due to
  // less CPU time spent.
  websocket.disablePerMessageDeflate();

  // Enable ping.
  constexpr std::int32_t kPingIntervalSec{45};
  websocket.setPingInterval(kPingIntervalSec);

  websocket.start();
}

// static
template <class F>
void WebSocketRpcEndpoint::InvokeInEventLoop(std::shared_ptr<WebSocketRpcEndpoint>&& self,
                                             F invocable) noexcept {
  static_assert(noexcept(invocable(self, std::declval<horus_internal::EventLoop&>())), "");

  constexpr std::chrono::milliseconds kFailureRetryInterval{100};

  horus_internal::EventLoop::Invoker const& invoker{self->invoker_};
  auto call_invocable = [endpoint{std::move(self)}, owned_invocable{std::move(invocable)}](
                            horus_internal::EventLoop& event_loop) mutable noexcept {
    owned_invocable(endpoint, event_loop);
  };
  for (;;) {
    // Try until we succeed.
    try {
      bool const did_dispatch{invoker.TryInvoke(call_invocable)};
      if (!did_dispatch) {
        // If the event loop was no longer accessible, we just didn't dispatch the event.
        //
        // We do have to be careful; the caller could be the WebSocket thread, which could lead to
        // deadlocks if we have the last reference to it.
        DestroyInAnotherThread(std::move(call_invocable));
      }
      break;
    } catch (const std::bad_alloc&) {
      std::this_thread::sleep_for(kFailureRetryInterval);
    }
  }
}

// static
void WebSocketRpcEndpoint::HandleRequestMessage(std::shared_ptr<WebSocketRpcEndpoint>&& self,
                                                pb::RpcMessage&& request_message) {
  InvokeInEventLoop(std::move(self), [owned_message{std::move(request_message)}](
                                         const std::shared_ptr<WebSocketRpcEndpoint>& endpoint_ptr,
                                         horus_internal::EventLoop& event_loop) mutable noexcept {
    try {
      RpcRequestId const request_id{owned_message.request_id()};
      const RpcContext context{std::shared_ptr<WebSocketRpcEndpoint>{endpoint_ptr}};
      event_loop.SpawnFuture(
          endpoint_ptr->message_handler_(context, std::move(owned_message)) |
          Map([endpoint_ptr, request_id](pb::RpcMessage&& response_message) {
            if (request_id == kOneWayRpcRequestId) {
              assert(response_message.IsEmpty());
              return;
            }
            if (response_message.IsEmpty()) {
              if (endpoint_ptr->on_event_ != nullptr) {
                endpoint_ptr->on_event_(
                    endpoint_ptr->on_event_receiver_,
                    LifecycleEvent{InPlaceType<ErrorEvent>,
                                   ErrorEvent{std::make_exception_ptr(
                                       std::runtime_error{"no handler found"})}});
              }
              response_message = pb::RpcMessage{}
                                     .set_version(pb::RpcMessage::Version::kOne)
                                     .set_request_id(request_id)
                                     .set_error(CowBytes::Borrowed("no handler found"));
            }

            constexpr std::chrono::milliseconds kRetryInterval{200};
            endpoint_ptr->send_queue_.push_back(
                PendingRequest{std::move(response_message), RetryIndefinitely(kRetryInterval),
                               decltype(PendingRequest::continuation){InPlaceType<void>}});
          }) |
          Catch([](const std::exception& exn) {
            Log("exception thrown while executing message handler: ", exn.what());
          }));
    } catch (const std::exception& e) {
      Log("exception thrown while dispatching WebSocket request: ", e.what());
    }
  });
}

template <class T>
AnyFuture<T> WebSocketRpcEndpoint::SendImpl(pb::RpcMessage&& message,
                                            const RpcOptions& options) noexcept(false) {
  RpcRetryPolicy::DeadlineClock::time_point const deadline{options.retry_policy.deadline};

  // Wait until the queue isn't full. This might block the event loop, but there isn't much else
  // we can do here given that we're working with a bound queue. Completion will be notified from
  // another thread, so we won't deadlock.
  std::unique_lock<std::mutex> lock{send_mtx_};
  const bool timed_out{!send_cv_.wait_until(lock, deadline, [this]() noexcept -> bool {
    return send_shutdown_ || send_queue_.size() < kRequestCapacity;
  })};
  if (timed_out) {
    throw CancellationError{};
  }
  if (send_shutdown_) {
    throw RpcEndpointDisconnectedError{};
  }

  // Enqueue request.
  auto future_and_continuation = FromContinuation<T>();

  send_queue_.push_back(PendingRequest{std::move(message), options.retry_policy,
                                       std::move(future_and_continuation.second)});
  send_cv_.notify_one();

  if (options.retry_policy.deadline == RpcRetryPolicy::DeadlineClock::time_point::max()) {
    return std::move(future_and_continuation.first);
  }
  return CancelAt(options.retry_policy.deadline, std::move(future_and_continuation.first));
}

void WebSocketRpcEndpoint::CancelPending() noexcept {
  const std::unique_lock<std::mutex> lock{send_mtx_};
  for (std::size_t i{0}; i < pending_responses_.size(); ++i) {
    if (used_pending_responses_[i]) {
      static_cast<void>(
          // NOLINTNEXTLINE(*-constant-array-index)
          pending_responses_[i].continuation.FailWith(RpcEndpointDisconnectedError{}));
    }
  }
  used_pending_responses_.reset();
}

void WebSocketRpcEndpoint::RunSendThread() noexcept {
  for (;;) {
    PendingRequest to_send{
        {}, {}, decltype(PendingRequest::continuation){InPlaceType<Continuation<void>>}};
    std::size_t req_index{0};
    {
      std::unique_lock<std::mutex> lock{send_mtx_};
      send_cv_.wait(lock,
                    [this]() noexcept -> bool { return send_shutdown_ || !send_queue_.empty(); });
      if (send_shutdown_) {
        break;
      }
      assert(!send_queue_.empty());

      // Obtain request.
      to_send = std::move(send_queue_.front());
      send_queue_.pop_front();

      if (send_queue_.size() == kRequestCapacity - 1) {
        // Notify WebSocket that the queue is no longer full.
        send_cv_.notify_one();
      }

      if (to_send.continuation.Is<Continuation<pb::RpcMessage>>()) {
        send_cv_.wait(lock, [this]() noexcept -> bool {
          return send_shutdown_ || !used_pending_responses_.all();
        });

        if (used_pending_responses_.all()) {
          // Shut down.
          break;
        }

        // Allocate request ID.
        for (std::size_t i{0}; i < used_pending_responses_.size(); ++i) {
          if (!used_pending_responses_[i]) {
            used_pending_responses_[i] = true;
            to_send.message.set_request_id(static_cast<RpcRequestId>(i) + kTwoWayRpcRequestIdMin);
            req_index = i;
            break;
          }
        }
      }
    }

    // Set up retrying logic.
    std::uint16_t retries{0};
    std::chrono::milliseconds retry_interval{to_send.retry_policy.retry_interval};
    bool give_up{false};

    /// Checks whether we should keep retrying to send this message *following a failure*. If false,
    /// this immediately returns. If true, this might suspend the current thread as per the given
    /// `RpcRetryPolicy`.
    const auto keep_retrying = [&policy = to_send.retry_policy, &retries,
                                &retry_interval]() noexcept -> bool {
      if ((retries == policy.max_retries) &&
          (retries == std::numeric_limits<std::uint16_t>::max())) {
        return false;
      }
      if (RpcRetryPolicy::DeadlineClock::now() >= policy.deadline) {
        return false;
      }
      std::this_thread::sleep_for(retry_interval);
      if (retry_interval.count() >= std::chrono::milliseconds::max().count() / 2L) {
        retry_interval = policy.max_retry_interval;
      } else {
        retry_interval = std::min(retry_interval * 2, policy.max_retry_interval);
      }
      return true;
    };

    /// Checks whether we should keep trying to send this message *before* attempting to do so.
    const auto keep_trying = [&to_send]() noexcept -> bool {
      bool result{true};  // Defaults to true for the case where we have no continuation, i.e. this
                          // is a response.
      to_send.continuation.InvokeWithConst(
          [&result](const auto& continuation) noexcept { result = !continuation.WasCancelled(); });
      return result;
    };

    // Serialize request.
    std::string data;
    for (;;) {
      if (!keep_trying()) {
        give_up = true;
        continue;
      }
      try {
        data = to_send.message.SerializeToString();
        break;
      } catch (const std::bad_alloc& e) {
        if (!keep_retrying()) {
          to_send.continuation.InvokeWith(
              [&e](auto& continuation) noexcept { static_cast<void>(continuation.FailWith(e)); });
          give_up = true;
          break;
        }
      }
    }
    if (give_up) {
      continue;
    }

    // Send request.
    for (;;) {
      if (!keep_trying()) {
        give_up = true;
        break;
      }
      try {
        websocket_.sendBinary(data);
        break;
      } catch (const std::exception& e) {
        if (!keep_retrying()) {
          to_send.continuation.InvokeWith(
              [&e](auto& continuation) noexcept { static_cast<void>(continuation.FailWith(e)); });
          give_up = true;
          break;
        }
      }
    }
    if (give_up) {
      continue;
    }

    using ContinuationType = decltype(to_send.continuation);

    switch (to_send.continuation.Tag()) {
      case ContinuationType::kTagFor<Continuation<pb::RpcMessage>>: {
        // If a response is expected, set it up. Note that at this point we allocated a request ID
        // for the response, so we have no choice but to wait for it even if we reached the
        // deadline.
        const std::unique_lock<std::mutex> lock{send_mtx_};
        // NOLINTNEXTLINE(*-constant-array-index)
        pending_responses_[req_index].continuation =
            std::move(to_send.continuation).As<Continuation<pb::RpcMessage>>();
        break;
      }
      case ContinuationType::kTagFor<Continuation<void>>: {
        static_cast<void>(to_send.continuation.As<Continuation<void>>().ContinueWith());
        break;
      }
      case ContinuationType::kTagFor<void>:
        // We are sending a response, and don't need to send a continuation.
        break;
      default: {
        assert(false);
        break;
      }
    }
  }
}

/// Callback given to `SetLifecycleEventCallback()` in `WebSocketConnect() -> AnyFuture`.
class WebSocketConnectLifecycleCallback final {
 public:
  /// Constructs the callback.
  WebSocketConnectLifecycleCallback(
      const std::shared_ptr<RpcEndpoint>& endpoint,
      Continuation<std::shared_ptr<RpcEndpoint>> continuation) noexcept
      : endpoint_{endpoint}, continuation_{std::move(continuation)} {}

  /// Handles a lifecycle event.
  void operator()(RpcEndpoint::LifecycleEvent&& lifecycle_event_rvalue) noexcept {
    constexpr std::uint8_t kMaxFailures{4};

    const RpcEndpoint::LifecycleEvent lifecycle_event{std::move(lifecycle_event_rvalue)};

    switch (lifecycle_event.Tag()) {
      case RpcEndpoint::LifecycleEvent::kTagFor<RpcEndpoint::ConnectedEvent>: {
        static_cast<void>(continuation_.ContinueWith(endpoint_));
        break;
      }
      case RpcEndpoint::LifecycleEvent::kTagFor<RpcEndpoint::DisconnectedEvent>: {
        static_cast<void>(continuation_.FailWith(RpcEndpointDisconnectedError{}));
        break;
      }
      case RpcEndpoint::LifecycleEvent::kTagFor<RpcEndpoint::ErrorEvent>: {
        if (failures_ == kMaxFailures) {
          // We cannot properly connect -> return a disconnected error.
          static_cast<void>(continuation_.FailWith(RpcEndpointDisconnectedError{}));
        } else {
          ++failures_;
          return;
        }
        break;
      }
      case RpcEndpoint::LifecycleEvent::kTagFor<RpcEndpoint::ShutdownEvent>: {
        static_cast<void>(continuation_.FailWith(CancellationError{}));
        break;
      }
      default: {
        assert(false);
        break;
      }
    }
    endpoint_->ClearLifecycleEventCallback();

    std::unique_ptr<WebSocketConnectLifecycleCallback>{this}.reset();
  }

 private:
  /// The endpoint to resolve the `continuation_` with.
  std::shared_ptr<RpcEndpoint> endpoint_;
  /// The continuation of the future.
  Continuation<std::shared_ptr<RpcEndpoint>> continuation_;
  /// The number of consecutive errors we got without getting any other event. At `kMaxFailures`, we
  /// give up.
  std::uint8_t failures_{0};
};

}  // namespace

std::shared_ptr<RpcEndpoint> WebSocketConnect(horus_internal::EventLoop& event_loop,
                                              std::string&& url,
                                              MessageHandler&& message_handler) noexcept(false) {
  std::shared_ptr<WebSocketRpcEndpoint> result{std::make_shared<WebSocketRpcEndpoint>(
      event_loop, std::move(url), std::move(message_handler))};
  WebSocketRpcEndpoint::Initialize(result);
  return result;
}

AnyFuture<std::shared_ptr<RpcEndpoint>> ConnectingWebSocket(
    std::string&& url, MessageHandler&& message_handler) noexcept(false) {
  return FromPoll([owned_url{std::move(url)}, owned_handler{std::move(message_handler)}](
                      PollContext& context) mutable -> PollResult<std::shared_ptr<RpcEndpoint>> {
    return WebSocketConnect(context.Loop(), std::move(owned_url), std::move(owned_handler));
  });
}

AnyFuture<std::shared_ptr<RpcEndpoint>> ConnectedWebSocket(
    std::string&& url, MessageHandler&& message_handler) noexcept(false) {
  // We don't reuse `ConnectingWebSocket()` to avoid the intermediate `AnyFuture`.
  return FromPoll([owned_url{std::move(url)}, owned_handler{std::move(message_handler)}](
                      PollContext& context) mutable -> PollResult<std::shared_ptr<RpcEndpoint>> {
           return WebSocketConnect(context.Loop(), std::move(owned_url), std::move(owned_handler));
         }) |
         Then([](const std::shared_ptr<RpcEndpoint>& endpoint)
                  -> FromContinuationFuture<std::shared_ptr<RpcEndpoint>> {
           auto future_and_continuation = FromContinuation<std::shared_ptr<RpcEndpoint>>();
           std::unique_ptr<WebSocketConnectLifecycleCallback> callback{
               std::make_unique<WebSocketConnectLifecycleCallback>(
                   endpoint, std::move(future_and_continuation.second))};

           WebSocketConnectLifecycleCallback& on_event{*callback.release()};
           endpoint->SetLifecycleEventCallback(on_event);
           if (dynamic_cast<WebSocketRpcEndpoint&>(*endpoint).IsConnected()) {
             on_event(RpcEndpoint::LifecycleEvent{InPlaceType<RpcEndpoint::ConnectedEvent>});
           }

           return std::move(future_and_continuation.first);
         });
}

}  // namespace horus
