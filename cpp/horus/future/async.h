/// @file
///
/// The `Async()` function.

#ifndef HORUS_FUTURE_ASYNC_H_
#define HORUS_FUTURE_ASYNC_H_

#include <memory>
#include <thread>
#include <utility>

#include "horus/event_loop/event_loop.h"
#include "horus/future/cancel.h"
#include "horus/future/from_continuation.h"
#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/types/one_of.h"

namespace horus {

/// Object given to the body of an `Async()` call.
class Awaiter final {
 public:
  /// Constructs an `Awaiter`.
  explicit Awaiter(std::weak_ptr<horus_internal::EventLoop::State> loop) noexcept
      : loop_{std::move(loop)} {}

  /// The `Awaiter` can neither be copied nor moved.
  Awaiter(const Awaiter&) = delete;
  /// The `Awaiter` can neither be copied nor moved.
  Awaiter(Awaiter&&) = delete;
  /// The `Awaiter` can neither be copied nor moved.
  Awaiter& operator=(const Awaiter&) = delete;
  /// The `Awaiter` can neither be copied nor moved.
  Awaiter& operator=(Awaiter&&) = delete;

  /// Destroys the `Awaiter`.
  ~Awaiter() noexcept = default;

  /// Waits for the given future to complete, then returns its result.
  template <class Future>
  FutureResult<Future> operator()(Future&& future) const noexcept(false) {
    const std::shared_ptr<horus_internal::EventLoop::State> loop{loop_.lock()};
    if (loop == nullptr) {
      throw CancellationError{};
    }
    return loop->Loop().AwaitFuture(std::forward<Future>(future));
  }

 private:
  /// A (non-null, but possibly expired) pointer to the event loop.
  std::weak_ptr<horus_internal::EventLoop::State> loop_;
};

/// Future returned by `Async()`.
template <class Body>
class AsyncFuture final
    : public Future<decltype(std::declval<Body&&>()(std::declval<Awaiter&>()))> {
 public:
  /// @copydoc Future::Result
  using typename AsyncFuture::Future::Result;

  /// Constructs the future.
  explicit AsyncFuture(Body&& body, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : state_{InPlaceType<Body>, std::move(body)}, info_{info} {}

  /// Polls the future until its background work has completed.
  PollResult<Result> UnsafePoll(PollContext& context) final {
    using Inner = FromContinuationFuture<Result>;

    const PollContext::Trace trace{context, "Async", info_};

    Inner* inner{state_.template TryAs<Inner>()};
    if (inner == nullptr) {
      Body owned_body{std::move(state_.template As<Body>())};
      auto future_and_continuation = FromContinuation<Result>();

      std::thread(
          [loop{std::weak_ptr<horus_internal::EventLoop::State>{context.Loop().InternalState()}}](
              Body&& body_ref, Continuation<Result>&& continuation_ref) noexcept {
            Body body{std::move(body_ref)};
            Continuation<Result> continuation{std::move(continuation_ref)};
            static_cast<void>(continuation.ContinueWithResultOf([&body, &loop]() -> Result {
              const Awaiter awaiter{loop};
              return body(awaiter);
            }));
          },
          std::move(owned_body), std::move(future_and_continuation.second))
          .detach();

      inner = &state_.template Emplace<Inner>(std::move(future_and_continuation.first));
    }
    return inner->UnsafePoll(context);
  }

 private:
  /// The state of the future; if `Body`, then `UnsafePoll()` has not been called yet.
  OneOf<Body, FromContinuationFuture<Result>> state_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a future which executes `body()` in a background thread with the ability to _await_
/// other futures, similarly to `std::async()`.
///
/// `Body` is a function which takes an `const Awaiter&` argument.
///
/// @warning Users should prefer functions that operate in the event loop instead of this function,
/// since it blocks a thread (expensive) and runs outside of the event loop (requires
/// synchronization). Only use this function in high-level code.
template <class Body>
inline AsyncFuture<Body> Async(Body&& body, const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return AsyncFuture<Body>{std::forward<Body>(body), info};
}

}  // namespace horus

#endif  // HORUS_FUTURE_ASYNC_H_
