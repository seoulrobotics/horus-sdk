/// @file
///
/// The `CompletedFuture` class.

#ifndef HORUS_FUTURE_COMPLETED_H_
#define HORUS_FUTURE_COMPLETED_H_

#include <cassert>
#include <exception>
#include <type_traits>
#include <utility>

#include "horus/future/future.h"
#include "horus/internal/pointer_cast.h"

namespace horus {

/// A future which immediately completes with either a value or an exception.
///
/// @see `ResolvedFuture` for futures which are always resolved.
/// @see `RejectedFuture` for futures which are always rejected.
template <class T>
class CompletedFuture final : public Future<T> {
 public:
  /// Returns a future which immediately completes.
  template <class... Args>
  explicit CompletedFuture(InPlaceTypeT<T> /* in_place */, Args&&... args) noexcept(
      std::is_nothrow_constructible<T, Args...>::value)
      : exception_{nullptr} {
    new (&result_) T(std::forward<Args>(args)...);
  }

  /// Returns a future which immediately throws `exception`.
  template <class E, std::enable_if_t<!std::is_same<E, CompletedFuture>::value>* = nullptr>
  explicit CompletedFuture(const E& exception) noexcept
      : exception_{std::make_exception_ptr(exception)} {}

  /// Returns a future which immediately throws `exception`.
  explicit CompletedFuture(std::exception_ptr exception) noexcept
      : exception_{std::move(exception)} {
    assert(exception_ != nullptr);
  }

  /// Cannot be copied.
  CompletedFuture(const CompletedFuture&) = delete;
  /// Cannot be copied.
  CompletedFuture& operator=(const CompletedFuture&) = delete;

  /// Move-constructs the future.
  CompletedFuture(CompletedFuture&& other) noexcept : exception_{other.exception_} {
    if (exception_ != nullptr) {
      new (&result_) T(std::move(*other.ResultPointer()));
    }
  }

  /// Move-assigns the future.
  CompletedFuture& operator=(CompletedFuture&& other) noexcept {
    if (this != &other) {
      this->~CompletedFuture();
      new (this) CompletedFuture(std::move(other));
    }
    return *this;
  }

  /// Destroys the future.
  ~CompletedFuture() noexcept {
    const T* result{ResultPointer()};
    if (result != nullptr) {
      result->~T();
    }
  }

  /// Returns the given value.
  PollResult<T> UnsafePoll(PollContext& /* context */) final {
    if (exception_ != nullptr) {
      std::rethrow_exception(exception_);
    }
    return PollResult<T>{InPlaceType<T>, std::move(*ResultPointer())};
  }

 private:
  /// Returns a pointer to `result_` if it is initialized, and `nullptr` otherwise.
  T* ResultPointer() noexcept {
    if (exception_ != nullptr) {
      return nullptr;
    }
    return horus_internal::UnsafePointerCast<T>(&result_);
  }

  /// The exception to throw in `UnsafePoll()` if not `nullptr`.
  std::exception_ptr exception_;
  /// The value to complete with if `exception_` is `nullptr`.
  std::aligned_union_t<0, T> result_;
};

/// A future which immediately completes or throws an exception.
template <>
class CompletedFuture<void> final : public Future<void> {
 public:
  /// Returns a future which immediately completes.
  explicit CompletedFuture() noexcept : exception_{nullptr} {}  // NOLINT(*-throw-keyword-missing)

  /// Returns a future which immediately throws `exception`.
  template <class E, std::enable_if_t<!std::is_same<E, CompletedFuture>::value>* = nullptr>
  explicit CompletedFuture(const E& exception) noexcept
      : exception_{std::make_exception_ptr(exception)} {}

  /// Returns a future which immediately throws `exception`.
  explicit CompletedFuture(std::exception_ptr exception) noexcept
      : exception_{std::move(exception)} {}  // NOLINT(*-throw-keyword-missing)

  /// Returns the given value.
  PollResult<void> UnsafePoll(PollContext& /* context */) final {
    if (exception_ != nullptr) {
      std::rethrow_exception(exception_);
    }
    return PollResult<void>{InPlaceType<void>};
  }

 private:
  /// The exception to throw in `UnsafePoll()` if not `nullptr`.
  std::exception_ptr exception_;
};

}  // namespace horus

#endif  // HORUS_FUTURE_COMPLETED_H_
