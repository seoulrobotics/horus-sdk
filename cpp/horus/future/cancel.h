/// @file
///
/// Futures that throw a `CancellationError` after some delay.

#ifndef HORUS_FUTURE_CANCEL_H_
#define HORUS_FUTURE_CANCEL_H_

#include <chrono>
#include <stdexcept>
#include <tuple>
#include <utility>

#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/future/operator.h"
#include "horus/future/poll.h"
#include "horus/future/time.h"

namespace horus {

/// An error thrown when a `Future` is cancelled.
class CancellationError : public std::runtime_error {
 public:
  /// Constructs a `CancellationError` with a predefined message.
  CancellationError() : std::runtime_error{"operation was cancelled"} {}
};

// MARK: CancelFuture

namespace horus_internal {

/// Future returned by `CancelAt()` and `CancelIn()`.
template <class Cancellation, class F>
class CancelFuture final : public Future<FutureResult<F>> {
 public:
  /// Constructs the future.
  CancelFuture(Cancellation&& cancellation, F&& inner,
               const FutureInfo& info = CurrentFutureInfo()) noexcept
      : cancellation_{std::move(cancellation)}, inner_{std::move(inner)}, info_{info} {}

  /// Polls the future until `timeout_` or `inner_` completes.
  PollResult<FutureResult<F>> UnsafePoll(PollContext& context) final {
    const PollContext::Trace trace{context, "Cancel", info_};

    PollResult<FutureResult<F>> inner_result{CancelFuture::PollFuture(inner_, context)};
    if (!inner_result.template Is<Pending>()) {
      return inner_result;
    }
    if (!CancelFuture::PollFuture(cancellation_, context).template Is<Pending>()) {
      throw CancellationError{};
    }
    return PendingResult<FutureResult<F>>();
  }

 private:
  /// The future which completes at the specified deadline.
  Cancellation cancellation_;
  /// The future to attempt to execute before the timeout is reached.
  F inner_;
  /// Future information.
  FutureInfo info_;
};

template <class Cancellation>
class CancelFuture<Cancellation, void> : public Future<void> {
 public:
  /// Constructs the future.
  explicit CancelFuture(Cancellation&& cancellation,
                        const FutureInfo& info = CurrentFutureInfo()) noexcept
      : cancellation_{std::move(cancellation)}, info_{info} {}

  /// Polls the future until `timeout_` completes.
  PollResult<void> UnsafePoll(PollContext& context) final {
    const PollContext::Trace trace{context, "Cancel", info_};

    if (CancelFuture::PollFuture(cancellation_, context).template Is<Pending>()) {
      return PendingResult<void>();
    }
    throw CancellationError{};
  }

 private:
  /// The future which completes at the specified timeout.
  Cancellation cancellation_;
  /// Future information.
  FutureInfo info_;
};

}  // namespace horus_internal

// MARK: CancelIn()

/// Future returned by `CancelIn()`.
template <class F>
using CancelInFuture = horus_internal::CancelFuture<CompleteInFuture, F>;

/// Returns a future which completes when `future` does, or throws a `CancellationError` when the
/// given `timeout` has passed, whichever happens first.
template <class F, class Rep, class Period>
inline CancelInFuture<F> CancelIn(std::chrono::duration<Rep, Period> timeout, F&& future,
                                  const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return CancelInFuture<F>{CompleteIn(timeout), std::forward<F>(future), info};
}

/// Returns a future which will throw a `CancellationError` when the given `timeout` has passed.
template <class Rep, class Period>
inline CancelInFuture<void> CancelIn(std::chrono::duration<Rep, Period> timeout,
                                     const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return CancelInFuture<void>{CompleteIn(timeout), info};
}

/// `Pipe()` operator which cancels the execution of a `Future` with a `CancellationError` when a
/// `timeout` has passed.
///
/// @see CancelIn()
template <class Rep, class Period>
inline FutureOperator<CancelInFuture, std::tuple<std::chrono::duration<Rep, Period>>> WithTimeout(
    std::chrono::duration<Rep, Period> timeout,
    const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return FutureOperator<CancelInFuture, std::tuple<std::chrono::duration<Rep, Period>>>{timeout,
                                                                                        info};
}

// MARK: CancelAt()

/// Future returned by `CancelAt()`.
template <class F, class Clock>
using CancelAtFuture = horus_internal::CancelFuture<CompleteAtFuture<Clock>, F>;

/// Returns a future which completes when `future` does, or throws a `CancellationError` when the
/// given `deadline` is reached, whichever happens first.
template <class F, class Clock>
inline CancelAtFuture<F, Clock> CancelAt(std::chrono::time_point<Clock> deadline, F&& future,
                                         const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return CancelAtFuture<F, Clock>{CompleteAt(deadline), std::forward<F>(future), info};
}

/// Returns a future which will throw a `CancellationError` when the given `deadline` is reached.
template <class Clock>
inline CancelAtFuture<void, Clock> CancelAt(std::chrono::time_point<Clock> deadline,
                                            const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return CancelAtFuture<void, Clock>{CompleteAt(deadline), info};
}

/// `Pipe()` operator which cancels the execution of a `Future` with a `CancellationError` when a
/// `deadline` is reached.
///
/// @see CancelAt()
template <class Clock>
inline FutureOperator<CancelAtFuture, std::tuple<std::chrono::time_point<Clock>>, Clock>
WithDeadline(std::chrono::time_point<Clock> deadline,
             const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return FutureOperator<CancelAtFuture, std::tuple<std::chrono::time_point<Clock>>>{deadline, info};
}

}  // namespace horus

#endif  // HORUS_FUTURE_CANCEL_H_
