/// @file
///
/// Pointer casts.

#ifndef HORUS_INTERNAL_POINTER_CAST_H_
#define HORUS_INTERNAL_POINTER_CAST_H_

#include <cassert>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace horus {
namespace horus_internal {

/// Reinterprets `ptr` from `From*` to `T*`.
template <class T, class... PreventExplicitFrom, class From>
T* UnsafePointerCast(From* pointer) noexcept {
  static_assert(sizeof(T*) == sizeof(void*), "fat pointers cannot be cast");
  static_assert(sizeof(T*) == sizeof(From*), "source pointer must be a thin pointer");
  static_assert(
      std::is_const<T>::value || !std::is_const<From>::value,
      "cannot cast from const pointer to mutable pointer; use `UnsafeConstCast()` instead");

  // NOLINTNEXTLINE(*-type-reinterpret-cast): only used in debug mode for asserts
  assert(reinterpret_cast<std::uintptr_t>(pointer) %
             alignof(std::conditional_t<std::is_void<T>::value, char, T>) ==
         0);

  T* result{nullptr};
  // NOLINTNEXTLINE(*-pointer-conversion): https://github.com/llvm/llvm-project/issues/93959
  std::memcpy(static_cast<void*>(&result), static_cast<const void*>(&pointer), sizeof(T*));
  return result;
}

/// Reinterprets `ptr` from `const T*` to `T*`.
template <class T>
std::remove_const_t<T>* UnsafeConstCast(T* pointer) noexcept {
  static_assert(std::is_const<T>::value, "unnecessary const cast: pointer is already mutable");

  std::remove_const_t<T>* result{nullptr};
  // NOLINTNEXTLINE(*-pointer-conversion): https://github.com/llvm/llvm-project/issues/93959
  std::memcpy(static_cast<void*>(&result), static_cast<const void*>(&pointer), sizeof(T*));
  return result;
}

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_INTERNAL_POINTER_CAST_H_
