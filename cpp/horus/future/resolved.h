/// @file
///
/// The `ResolvedFuture` class.

#ifndef HORUS_FUTURE_RESOLVED_H_
#define HORUS_FUTURE_RESOLVED_H_

#include <type_traits>
#include <utility>

#include "horus/future/future.h"

namespace horus {

// MARK: ResolveWith()

/// A future which immediately completes with a value of type `T`.
///
/// @see `RejectedFuture` for futures which are always rejected.
/// @see `CompletedFuture` for futures which are either resolved or rejected.
template <class T>
// NOLINTNEXTLINE(clang-analyzer-core.uninitialized.*): false positive in implicit move constructor
class ResolvedFuture final : public Future<T> {
 public:
  /// Returns a future which immediately completes with a default-constructed `T` value.
  ResolvedFuture() noexcept(std::is_nothrow_default_constructible<T>::value) : result_{} {}

  /// Returns a future which immediately completes with a value constructed with `args`.
  template <class... Args>
  explicit ResolvedFuture(InPlaceTypeT<T> /* in_place */,
                          Args&&... args) noexcept(std::is_nothrow_constructible<T, Args...>::value)
      : result_(std::forward<Args>(args)...) {}

  /// Returns the given value.
  PollResult<T> UnsafePoll(PollContext& /* context */) final {
    return PollResult<T>{InPlaceType<T>, std::move(result_)};
  }

 private:
  /// The value to complete with.
  T result_;
};

/// A future which immediately completes.
template <>
class ResolvedFuture<void> final : public Future<void> {
 public:
  /// Returns a future which immediately completes.
  explicit ResolvedFuture() noexcept = default;

  /// Returns a future which immediately completes.
  explicit ResolvedFuture(InPlaceTypeT<void> /* in_place */) noexcept {}

  /// Returns a completed `PollResult`.
  PollResult<void> UnsafePoll(PollContext& /* context */) final {
    return PollResult<void>{InPlaceType<void>};
  }
};

/// Returns a future which immediately completes with `value`.
template <class T>
inline ResolvedFuture<T> ResolveWith(T&& value) noexcept {
  return ResolvedFuture<T>(InPlaceType<T>, std::forward<T>(value));
}

}  // namespace horus

#endif  // HORUS_FUTURE_RESOLVED_H_
