/// @file
///
/// Safe pointer casts.

#ifndef HORUS_POINTER_CAST_H_
#define HORUS_POINTER_CAST_H_

#include <cstdint>
#include <type_traits>

namespace horus {

/// Defines `value` to be true if `T*` is a valid `SafePointerCast()` target.
template <class T>
struct IsSafePointerCastTarget : std::false_type {};
template <>
struct IsSafePointerCastTarget<void> : std::true_type {};
template <>
struct IsSafePointerCastTarget<char> : std::true_type {};
template <>
struct IsSafePointerCastTarget<unsigned char> : std::true_type {};

/// Casts `pointer` from `From*` to `To*`. This function is only defined when such a cast is safe
/// (as per MISRA C++:2023 8.2.5).
template <class To, class From, std::enable_if_t<IsSafePointerCastTarget<To>::value>* = nullptr>
To* SafePointerCast(From* pointer) noexcept {
  return reinterpret_cast<To*>(pointer);  // NOLINT(*-reinterpret-cast)
}

/// Casts `pointer` from `From const*` to `To const*`. This function is only defined when such a
/// cast is safe (as per MISRA C++:2023 8.2.5).
template <class To, class From, std::enable_if_t<IsSafePointerCastTarget<To>::value>* = nullptr>
To const* SafePointerCast(From const* pointer) noexcept {
  return reinterpret_cast<To const*>(pointer);  // NOLINT(*-reinterpret-cast)
}

/// Casts `pointer` to a `std::uintptr_t`.
inline std::uintptr_t PointerCastToUintptr(void const* pointer) noexcept {
  // MISRA C++:2023 8.2.5 allows a cast to `std::uintptr_t` from any pointer type.
  return reinterpret_cast<std::uintptr_t>(pointer);  // NOLINT(*-reinterpret-cast)
}

}  // namespace horus

#endif  // HORUS_POINTER_CAST_H_
