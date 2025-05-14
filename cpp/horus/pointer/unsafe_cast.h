/// @file
///
/// Unsafe pointer casts.

#ifndef HORUS_POINTER_UNSAFE_CAST_H_
#define HORUS_POINTER_UNSAFE_CAST_H_

#include <cassert>
#include <cstdint>
#include <type_traits>

namespace horus {

/// Reinterprets `ptr` from `From*` to `T*`.
///
/// This function should be used exceedingly rarely, and when it _is_ used it should be paired with
/// a comment explaining its safety.
///
/// Consider using `SafePointerCast()` in `cast.h` and `ErasedPointer` in `erased.h` instead.
template <class T, class... PreventExplicitFrom, class From>
T* UnsafePointerCast(From* pointer) noexcept {
  static_assert(
      std::is_const<T>::value || !std::is_const<From>::value,
      "cannot cast from const pointer to mutable pointer; use `UnsafeConstCast()` instead");

  // NOLINTNEXTLINE(*-reinterpret-cast): only used in debug mode for asserts
  assert(reinterpret_cast<std::uintptr_t>(pointer) %
             alignof(std::conditional_t<std::is_void<T>::value, char, T>) ==
         0);

  // NOLINTNEXTLINE(*-reinterpret-cast): whole point of the function
  return reinterpret_cast<T*>(pointer);
}

/// Reinterprets `ptr` from `const T*` to `T*`.
///
/// This function should be used exceedingly rarely, and when it _is_ used it should be paired with
/// a comment explaining its safety.
///
/// Consider using `ErasedPointer` in `erased.h` instead.
template <class T>
std::remove_const_t<T>* UnsafeConstCast(T* pointer) noexcept {
  static_assert(std::is_const<T>::value, "unnecessary const cast: pointer is already mutable");

  // NOLINTNEXTLINE(*-const-cast): whole point of the function
  return const_cast<std::remove_const_t<T>*>(pointer);
}

}  // namespace horus

#endif  // HORUS_POINTER_UNSAFE_CAST_H_
