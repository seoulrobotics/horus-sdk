/// @file
///
/// The `MoveOnlyFunction` class.

#ifndef HORUS_FUNCTIONAL_MOVE_ONLY_FUNCTION_H_
#define HORUS_FUNCTIONAL_MOVE_ONLY_FUNCTION_H_

#include <cassert>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace horus {
namespace horus_internal {

/// Throws a `std::logic_error` as used by `ThrowOnCopy`.
template <class T>
inline T ThrowIllegalCopy() noexcept(false) {
  throw std::logic_error{"illegal copy of ThrowOnCopy object"};
}

/// A wrapper around an object `F` which makes it copyable by throwing an exception.
template <class F>
class ThrowOnCopy final {
  static_assert(std::is_nothrow_move_constructible<F>::value, "");
  static_assert(std::is_nothrow_destructible<F>::value, "");

 public:
  /// Constructs the object.
  constexpr explicit ThrowOnCopy(F&& value) noexcept : value_{std::move(value)} {}

  /// Throws a `std::logic_error`.
  ThrowOnCopy(const ThrowOnCopy& /* other */) noexcept(false) : value_{ThrowIllegalCopy<F>()} {}
  /// Throws a `std::logic_error`.
  ThrowOnCopy& operator=(const ThrowOnCopy& /* other */)  // NOLINT(cert-oop54-cpp)
      noexcept(false) {
    ThrowIllegalCopy<void>();
  }

  /// Move constructor.
  ThrowOnCopy(ThrowOnCopy&&) noexcept = default;
  /// Move assignment operator.
  ThrowOnCopy& operator=(ThrowOnCopy&&) noexcept = default;

  /// Destructor.
  ~ThrowOnCopy() noexcept = default;

  /// Invokes `F`.
  template <class Receiver, class... Args,
            std::enable_if_t<sizeof...(Args) >= 0 && std::is_member_function_pointer<F>::value>* =
                nullptr>
  decltype(auto) operator()(Receiver&& receiver, Args&&... args) {
    return (std::forward<Receiver>(receiver).*value_)(std::forward<Args>(args)...);
  }

  /// Invokes `F`.
  template <class... Args, std::enable_if_t<sizeof...(Args) >= 0 &&
                                            !std::is_member_function_pointer<F>::value>* = nullptr>
  decltype(auto) operator()(Args&&... args) {
    return value_(std::forward<Args>(args)...);
  }

 private:
  /// The underlying value.
  F value_;
};

}  // namespace horus_internal

template <class F>
class MoveOnlyFunction;

/// Defines `value` to be true iff `T` is a `MoveOnlyFunction`.
template <class T>
struct IsMoveOnlyFunction : std::false_type {};
template <class F>
struct IsMoveOnlyFunction<MoveOnlyFunction<F>> : std::true_type {};

/// A wrapper around a `std::function` which can wrap move-only invocables.
///
/// @see https://en.cppreference.com/w/cpp/utility/functional/move_only_function
template <class R, class... Args>
class MoveOnlyFunction<R(Args...)> final {
 public:
  /// Constructs an empty function.
  MoveOnlyFunction() noexcept = default;
  /// Constructs an empty function.
  MoveOnlyFunction(decltype(nullptr) /* nullptr */) noexcept  // NOLINT(*-explicit-*)
      : function_{nullptr} {}

  /// Constructs a function whose target is `std::decay_t<F>`. If `F` is a null function pointer or
  /// null pointer to member value, the resulting function is empty.
  ///
  /// @see
  /// https://en.cppreference.com/w/cpp/utility/functional/move_only_function/move_only_function
  template <class F, std::enable_if_t<!IsMoveOnlyFunction<std::decay_t<F>>::value>* = nullptr>
  MoveOnlyFunction(F&& invocable)  // NOLINT(*-explicit-*)
      : function_(horus_internal::ThrowOnCopy<std::decay_t<F>>{std::forward<F>(invocable)}) {
    static_assert(!std::is_reference<F>::value, "");
    static_assert(!std::is_const<F>::value, "");
  }

  /// Constructs a function whose target is an `std::function`.
  MoveOnlyFunction(std::function<R(Args...)>&& function) noexcept  // NOLINT(*-explicit-*)
      : function_{std::move(function)} {}

  /// Non-copyable.
  MoveOnlyFunction(const MoveOnlyFunction&) = delete;
  /// Non-copyable.
  MoveOnlyFunction& operator=(const MoveOnlyFunction&) = delete;

  /// Move constructor.
  MoveOnlyFunction(MoveOnlyFunction&&) noexcept = default;
  /// Move assignment operator.
  MoveOnlyFunction& operator=(MoveOnlyFunction&&) noexcept = default;

  /// Destructor.
  ~MoveOnlyFunction() noexcept = default;

  /// Invokes the underlying invocable.
  R operator()(Args... args) {
    assert(function_ != nullptr);  // Because `std::function` throws when empty, but
                                   // `std::move_only_function` leads to UB.
    return function_(std::forward<Args>(args)...);
  }

  /// Returns whether the function is empty.
  friend bool operator==(const MoveOnlyFunction& function,
                         decltype(nullptr) /* nullptr */) noexcept {
    return function.function_ == nullptr;
  }
  /// Returns whether the function is non-empty.
  friend bool operator!=(const MoveOnlyFunction& function,
                         decltype(nullptr) /* nullptr */) noexcept {
    return !(function == nullptr);
  }

  /// (Internal use only) Returns a pointer to the stored target if it has type `T`.
  template <class T>
  T* InternalTarget() noexcept {
    return function_.template target<T>();
  }

 private:
  /// The underlying function.
  std::function<R(Args...)> function_;
};

}  // namespace horus

#endif  // HORUS_FUNCTIONAL_MOVE_ONLY_FUNCTION_H_
