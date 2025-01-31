/// @file
///
/// The `RejectedFuture` class.

#ifndef HORUS_FUTURE_REJECTED_H_
#define HORUS_FUTURE_REJECTED_H_

#include <exception>
#include <type_traits>
#include <utility>

#include "horus/future/future.h"

namespace horus {

/// A future which immediately completes by throwing an exception.
///
/// @see `ResolvedFuture` for futures which are always resolved.
/// @see `CompletedFuture` for futures which are either resolved or rejected.
template <class T>
class RejectedFuture final : public Future<T> {
 public:
  /// Returns a future which immediately throws `exception`.
  template <class E, std::enable_if_t<!std::is_same<E, RejectedFuture>::value>* = nullptr>
  explicit RejectedFuture(const E& exception) noexcept
      : exception_{std::make_exception_ptr(exception)} {}

  /// Returns a future which immediately throws `exception`.
  explicit RejectedFuture(std::exception_ptr exception) noexcept
      : exception_{std::move(exception)} {}

  /// Throws the given exception.
  PollResult<T> UnsafePoll(PollContext& /* context */) final { std::rethrow_exception(exception_); }

 private:
  /// The exception to throw in `UnsafePoll()`.
  std::exception_ptr exception_;
};

}  // namespace horus

#endif  // HORUS_FUTURE_REJECTED_H_
