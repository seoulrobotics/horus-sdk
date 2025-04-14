/// @file
///
/// A future which completes when a function is called.

#ifndef HORUS_FUTURE_FROM_CONTINUATION_H_
#define HORUS_FUTURE_FROM_CONTINUATION_H_

#include <cassert>
#include <exception>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "horus/event_loop/waker.h"
#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/future/poll.h"
#include "horus/internal/enum.h"
#include "horus/internal/void.h"
#include "horus/types/in_place.h"
#include "horus/types/one_of.h"
#include "horus/types/scope_guard.h"

namespace horus {

/// Exception thrown by `FromContinuationFuture::UnsafePoll()` when the corresponding `Continuation`
/// has been destroyed before completing.
class BrokenContinuationError final : public std::runtime_error {
 public:
  BrokenContinuationError() : std::runtime_error{"continuation object of future was destroyed"} {}
};

/// The future returned by `FromContinuation()`.
template <class T>
class FromContinuationFuture final : public Future<T> {
 public:
  class Continuation;

  /// Futures cannot be copied.
  FromContinuationFuture(const FromContinuationFuture&) = delete;
  /// Futures cannot be copied.
  FromContinuationFuture& operator=(const FromContinuationFuture&) = delete;

  /// Move-constructs the future.
  FromContinuationFuture(FromContinuationFuture&& other) noexcept
      : shared_state_{std::exchange(other.shared_state_, nullptr)}, info_{other.info_} {}

  /// Move-assigns the future.
  FromContinuationFuture& operator=(FromContinuationFuture&& other) noexcept {
    if (&other != this) {
      std::swap(shared_state_, other.shared_state_);
      std::swap(info_, other.info_);
    }
    return *this;
  }

  /// Destroys the future; if it had not yet completed, future calls to
  /// `Continuation::{Continue,Fail}With()` will return false.
  ~FromContinuationFuture() final;

  /// Polls the future until `Continuation::{Continue,Fail}With()` is called.
  ///
  /// @throws std::exception Any exception reported with `Continuation::FailWith()`.
  /// @throws BrokenContinuationError If the `Continuation` is destroyed before completing.
  PollResult<T> UnsafePoll(PollContext& context) final;

 private:
  /// Variant of `State` when one of its owners has been destroyed.
  struct SoleOwnership {};

  /// The shared state.
  using State = OneOf<Pending, horus_internal::FutureWaker, T, std::exception_ptr, SoleOwnership>;

  /// State shared between the `Continuation` and the `FutureWithContinuation`.
  ///
  /// It will be automatically destroyed when calling `WithState()` if `destroy` is set to true.
  class Shared final {
   public:
    /// Invokes `invocable()` with a reference to the `State`, locking access to it while this
    /// function executes. If the second argument is set to `true`, `this` will be deleted.
    template <class F>
    auto WithState(const F& invocable) noexcept(noexcept(invocable(std::declval<State&>(),
                                                                   std::declval<bool&>())))
        -> decltype(invocable(std::declval<State&>(), std::declval<bool&>())) {
      bool destroy{false};
      const auto scope_guard = Defer([this, &destroy]() noexcept {
        if (destroy) {
          std::unique_ptr<Shared>(this).reset();  // leak
        }
      });
      const std::unique_lock<std::mutex> lock{mutex_};
      return invocable(state_, destroy);
    }

   private:
    /// Mutex which guards access to `state_`.
    std::mutex mutex_;
    /// Actual shared state.
    State state_{InPlaceType<Pending>};
  };

  /// Constructs a `FromContinuationFuture` with owns `shared` alongside a `Continuation`.
  explicit FromContinuationFuture(Shared& shared, const FutureInfo& info) noexcept
      : shared_state_{&shared}, info_{info} {}

  /// State shared with the `Continuation`.
  Shared* shared_state_;
  /// Future information.
  FutureInfo info_;
};

template <class T>
class FromContinuationFuture<T>::Continuation final {
 public:
  /// @see FromContinuation()
  static std::pair<FromContinuationFuture, Continuation> CreatePair(
      const FutureInfo& info) noexcept(false) {
    Shared& shared{*std::make_unique<Shared>().release()};
    return {FromContinuationFuture{shared, info}, Continuation{shared}};
  }

  /// Constructs an empty continuation already considered to be cancelled.
  Continuation() noexcept = default;

  /// Continuations cannot be copied.
  Continuation(const Continuation&) = delete;
  /// Continuations cannot be copied.
  Continuation& operator=(const Continuation&) = delete;

  /// Move-constructs the continuation.
  Continuation(Continuation&& other) noexcept
      : shared_state_{std::exchange(other.shared_state_, nullptr)} {}

  /// Move-assigns the continuation.
  Continuation& operator=(Continuation&& other) noexcept {
    if (&other != this) {
      std::swap(shared_state_, other.shared_state_);
    }
    return *this;
  }

  /// Destroys the continuation.
  ~Continuation() noexcept;

  /// Returns whether the continuation future was destroyed, i.e. the operation it represents was
  /// cancelled.
  ///
  /// Note that this function and `*With()` may return different values if a thread destroys or
  /// completes the other end of the continuation around the time this function is called.
  constexpr bool WasCancelled() const noexcept { return shared_state_ == nullptr; }

  /// Completes the execution of the future with a value `T` constructed with `args`.
  ///
  /// @returns true if the future was notified that it can continue, false if this is not possible
  /// (either because the future was destroyed, or because `{Continue,Fail}With()` has already been
  /// called).
  template <class... Args>
  bool ContinueWith(Args&&... args) noexcept {  // NOLINT(*-missing-std-forward)
    static_assert(std::is_void<T>::value || std::is_nothrow_constructible<T, Args...>::value,
                  "completion must not fail");

    return ContinueOrFailWith([&args...](State& state) noexcept {
      state.template Emplace<T>(std::forward<Args>(args)...);
    });
  }

  /// Completes the execution of the future by making it throw an exception.
  ///
  /// @returns true if the future was notified that it can throw, false if this is not possible
  /// (either because the future was destroyed, or because `{Continue,Fail}With()` has already been
  /// called).
  template <class E, std::enable_if_t<std::is_base_of<std::exception, E>::value>* = nullptr>
  bool FailWith(E exception) noexcept {
    return FailWith(std::make_exception_ptr(exception));
  }

  /// Same as `FailWith(E)`, but directly accepts a `std::exception_ptr`.
  bool FailWith(std::exception_ptr exception) noexcept {
    return ContinueOrFailWith([&exception](State& state) noexcept {
      state.template Emplace<std::exception_ptr>(exception);
    });
  }

  /// Invokes `invocable()`, and calls `ContinueWith()` if it succeeds or `FailWith()` if it fails.
  template <class F,
            std::enable_if_t<!std::is_void<decltype(std::declval<F>()())>::value>* = nullptr>
  bool ContinueWithResultOf(F&& invocable) noexcept {
    try {
      return ContinueWith(std::forward<F>(invocable)());
    } catch (...) {
      return FailWith(std::current_exception());
    }
  }

  /// Invokes `invocable()`, and calls `ContinueWith()` if it succeeds or `FailWith()` if it fails.
  template <class F,
            std::enable_if_t<std::is_void<decltype(std::declval<F>()())>::value>* = nullptr>
  bool ContinueWithResultOf(F&& invocable) noexcept {
    try {
      std::forward<F>(invocable)();
      return ContinueWith();
    } catch (...) {
      return FailWith(std::current_exception());
    }
  }

 private:
  /// Constructs a `Continuation` with owns `shared` alongside a `FromContinuationFuture`.
  explicit Continuation(Shared& shared) noexcept : shared_state_{&shared} {}

  /// Common implementation of `ContinueWith()` and `FailWith()`.
  ///
  /// If the future can be notified of its completion, invokes `complete(State&)`, which must either
  /// set the state to `T` or to `std::exception_ptr`, then returns true. Otherwise, returns false,
  /// destroying the shared state if the continuation has sole ownership of the shared state.
  template <class F>
  bool ContinueOrFailWith(const F& complete) noexcept;

  /// State shared with the `Continuation`.
  Shared* shared_state_;
};

/// An object which can be used to make a `FromContinuationFuture<T>` complete.
template <class T>
using Continuation = typename FromContinuationFuture<T>::Continuation;

/// Returns a `Future<T>` which resolves when `Continuation::ContinueWith()` or
/// `Continuation::FailWith()` is called.
///
/// @throws std::bad_alloc If the shared state cannot be allocated.
template <class T>
inline std::pair<FromContinuationFuture<T>, Continuation<T>> FromContinuation(
    const FutureInfo& info = CurrentFutureInfo()) noexcept(false) {
  return Continuation<T>::CreatePair(info);
}

template <class T>
FromContinuationFuture<T>::~FromContinuationFuture() {
  if (shared_state_ != nullptr) {
    shared_state_->WithState([](State& state, bool& destroy) noexcept {
      switch (state.Tag()) {
        case OneOfTagFor<State, Pending>():
        case OneOfTagFor<State, horus_internal::FutureWaker>(): {
          // Sender is alive, and will destroy the future when it sees the receiver was destroyed.
          state.template Emplace<SoleOwnership>();
          break;
        }

        case OneOfTagFor<State, SoleOwnership>():
        case OneOfTagFor<State, T>():
        case OneOfTagFor<State, std::exception_ptr>(): {
          // Sender was destroyed or gave up ownership by sending the result.
          destroy = true;
          break;
        }

        default: {
          assert(false);
          break;
        }
      }
    });
    shared_state_ = nullptr;
  }
}

template <class T>
PollResult<T> FromContinuationFuture<T>::UnsafePoll(PollContext& context) {
  const PollContext::Trace trace{context, "FromContinuation", info_};
  if (shared_state_ == nullptr) {
    throw PolledAfterCompletionError{};
  }
  return shared_state_->WithState([this, &context](State& state, bool& destroy) -> PollResult<T> {
    switch (state.Tag()) {
      case OneOfTagFor<State, Pending>():
      case OneOfTagFor<State, horus_internal::FutureWaker>(): {
        // Waiting for result.
        static_cast<void>(state.template Emplace<horus_internal::FutureWaker>(context.Waker()));
        break;
      }
      case OneOfTagFor<State, T>(): {
        // Result available.
        shared_state_ = nullptr;
        destroy = true;
        return horus_internal::SwitchVoid<T>(
            /*if_void=*/
            [](const auto& forward) noexcept -> PollResult<T> {
              return ReadyResult<decltype(forward())>();
            }, /*if_not_void=*/
            [&state](const auto& forward) -> PollResult<T> {
              return ReadyResult<T>(std::move(forward(state).template As<T>()));
            });
      }
      case OneOfTagFor<State, std::exception_ptr>(): {
        // Exception available.
        shared_state_ = nullptr;
        destroy = true;
        std::rethrow_exception(state.template As<std::exception_ptr>());
        break;
      }
      case OneOfTagFor<State, SoleOwnership>():
        // Sender was destroyed.
        throw BrokenContinuationError{};
      default:
        throw horus_internal::UnhandledEnumValueError{state.Tag()};
    }
    return PendingResult<T>();
  });
}

template <class T>
FromContinuationFuture<T>::Continuation::~Continuation() noexcept {
  if (shared_state_ != nullptr) {
    shared_state_->WithState([](State& state, bool& destroy) noexcept {
      switch (state.Tag()) {
        case OneOfTagFor<State, Pending>():
        case OneOfTagFor<State, horus_internal::FutureWaker>(): {
          // Receiver is alive and waiting, and we're closing the channel.
          if (state.template Is<horus_internal::FutureWaker>()) {
            state.template As<horus_internal::FutureWaker>().Wake();
          }
          state.template Emplace<SoleOwnership>();
          break;
        }

        case OneOfTagFor<State, SoleOwnership>(): {
          // Receiver was destroyed.
          destroy = true;
          break;
        }

        case OneOfTagFor<State, T>():
        case OneOfTagFor<State, std::exception_ptr>():
        default: {
          // This should not happen as we set `shared_state_` to null when completing the future.
          assert(false);
          break;
        }
      }
    });
    shared_state_ = nullptr;
  }
}

template <class T>
template <class F>
bool FromContinuationFuture<T>::Continuation::ContinueOrFailWith(const F& complete) noexcept {
  static_assert(noexcept(complete(std::declval<State&>())), "");

  Shared* const shared_state{std::exchange(shared_state_, nullptr)};
  if (shared_state == nullptr) {
    return false;
  }
  return shared_state->WithState([&complete](State& state, bool& destroy) noexcept -> bool {
    switch (state.Tag()) {
      case OneOfTagFor<State, Pending>(): {
        // Receiver hasn't been polled yet, but we can still give it its result.
        complete(state);
        return true;
      }
      case OneOfTagFor<State, horus_internal::FutureWaker>(): {
        // Receiver has been polled and is waiting to be woken up.
        horus_internal::FutureWaker& waker{*state.template TryAs<horus_internal::FutureWaker>()};
        waker.Wake();
        complete(state);
        return true;
      }
      case OneOfTagFor<State, SoleOwnership>(): {
        // Receiver was destroyed, and we're the only one alive.
        destroy = true;
        return false;
      }
      case OneOfTagFor<State, T>():
      case OneOfTagFor<State, std::exception_ptr>():
      default: {
        assert(false);  // This should not happen; this was likely a data race as this class is not
                        // thread safe.
        return false;
      }
    }
  });
}

}  // namespace horus

#endif  // HORUS_FUTURE_FROM_CONTINUATION_H_
