/// @file
///
/// Pointer arithmetic helpers.

#ifndef HORUS_POINTER_ARITHMETIC_H_
#define HORUS_POINTER_ARITHMETIC_H_

#include <cassert>
#include <cstddef>
#include <type_traits>

namespace horus {

/// Returns whether pointer arithmetic on `T` is allowed.
///
/// This is true if `T` is final (or not a class).
template <class T>
constexpr bool CanDoPointerArithmetic() noexcept {
  return !std::is_class<T>::value || std::is_final<T>::value;
}

/// Returns `ptr + to_add`, ensuring at compile-time that pointer arithmetic on `T` is safe.
template <class T>
constexpr T* PointerAdd(T* ptr, std::size_t to_add) noexcept {
  static_assert(CanDoPointerArithmetic<T>(), "pointer arithmetic on T is not allowed");
  T* const result{ptr + to_add};  // NOLINT(*-pointer-arithmetic)
  assert(result >= ptr && "pointer overflow");
  return result;
}

/// Returns `ptr - to_sub`, ensuring at compile-time that pointer arithmetic on `T` is safe.
template <class T>
constexpr T* PointerSub(T* ptr, std::size_t to_sub) noexcept {
  static_assert(CanDoPointerArithmetic<T>(), "pointer arithmetic on T is not allowed");
  T* const result{ptr - to_sub};  // NOLINT(*-pointer-arithmetic)
  assert(result <= ptr && "pointer overflow");
  return result;
}

/// Returns `lhs - rhs`, ensuring at compile-time that pointer arithmetic on `T` is safe.
template <class T>
constexpr std::ptrdiff_t PointerDiff(T* lhs, T* rhs) noexcept {
  static_assert(CanDoPointerArithmetic<T>(), "pointer arithmetic on T is not allowed");
  return lhs - rhs;
}

/// Returns `end - begin`; `begin` must point to a value _before_ `end`.
template <class T>
constexpr std::size_t PointerDist(T* begin, T* end) noexcept {
  std::ptrdiff_t const diff{PointerDiff(end, begin)};
  assert(diff >= 0);
  return static_cast<std::size_t>(diff);
}

}  // namespace horus

#endif  // HORUS_POINTER_ARITHMETIC_H_
