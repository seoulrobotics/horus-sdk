/// @file
///
/// The `SharedFuture<T>` class and related symbols.

#ifndef HORUS_FUTURE_SHARED_H_
#define HORUS_FUTURE_SHARED_H_

#include <cassert>
#include <exception>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <utility>

#include "horus/future/future.h"
#include "horus/future/poll.h"
#include "horus/types/one_of.h"

namespace horus {

/// `SharedFuture` wraps a move-only `Future` in a `Future` which can be copied. The `Result` of the
/// `Future` must be copyable.
///
/// It caches the resolved value (or exception), and either serves the already resolved value (or
/// throws), or polls the future.
template <class F>
class SharedFuture final : public Future<FutureResult<F>> {
 public:
  /// @copydoc FutureResult. Must be copyable.
  using Result = FutureResult<F>;

  /// Constructs a copyable `SharedFuture` from a move-only future `F`.
  explicit SharedFuture(F&& fut)
      : future_and_result_{std::make_shared<FutureAndResult>(std::move(fut))} {}

  /// Builds a `SharedFuture` by performing a cheap reference counted copy.
  /// `future_and_result` cannot be `nullptr`.
  SharedFuture(const SharedFuture& other) noexcept : future_and_result_(other.future_and_result_) {
    assert(future_and_result_ != nullptr);
  }
  /// SharedFutures can be copied.
  SharedFuture& operator=(const SharedFuture&) noexcept = default;

  /// SharedFutures can be moved.
  SharedFuture(SharedFuture&&) noexcept = default;
  /// SharedFutures can be moved.
  SharedFuture& operator=(SharedFuture&&) noexcept = default;

  /// Destroys the shared future. If it is the last reference to its inner future, it will be
  /// destroyed (and cancelled).
  ~SharedFuture() final = default;

  /// Polls the underlying future and returns its `PollResult`.
  /// If the underlying future already resolved, serves a copy of its result.
  /// If the underlying future already failed, re-throws the exception.
  PollResult<Result> UnsafePoll(PollContext& context) final {
    assert(future_and_result_ != nullptr);

    Result* const result{future_and_result_->template TryAs<Result>()};
    if (result != nullptr) {
      // Future has already been resolved: return the cached result.
      return ReadyResult<Result>(*result);
    }

    std::exception_ptr* const exception{future_and_result_->template TryAs<std::exception_ptr>()};
    if (exception != nullptr) {
      // Future has already been rejected: rethrow the cached exception.
      std::rethrow_exception(*exception);
    }

    try {
      // Future has not been resolved yet: poll it and returns the poll result.
      F* const future{future_and_result_->template TryAs<F>()};
      PollResult<Result> resolved{F::PollFuture(*future, context)};

      Result* const polled_result{resolved.template TryAs<Result>()};
      if (polled_result != nullptr) {
        // The PollResult is actually a Result, we store it in future_and_result_.
        static_cast<void>(future_and_result_->template Emplace<Result>(*polled_result));
      }

      return resolved;
    } catch (...) {
      // If any error is thrown when polling the future, we store it.
      static_cast<void>(
          future_and_result_->template Emplace<std::exception_ptr>(std::current_exception()));
      throw;
    }
  }

 private:
  /// Contains either a future, its result, or the exception it threw on poll.
  using FutureAndResult = OneOf<F, Result, std::exception_ptr>;

  /// Holds the underlying future and its result.
  std::shared_ptr<FutureAndResult> future_and_result_;
};

/// Builds a `SharedFuture` from the given future.
template <class F>
SharedFuture<F> Share(F&& fut) {
  return SharedFuture<F>{std::forward<F>(fut)};
}

}  // namespace horus

#endif  // HORUS_FUTURE_SHARED_H_
