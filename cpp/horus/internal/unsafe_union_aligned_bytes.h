/// @file
///
/// The `UnsafeUnion` type (implemented with `AlignedUnion`).

#ifndef HORUS_INTERNAL_UNSAFE_UNION_ALIGNED_BYTES_H_
#define HORUS_INTERNAL_UNSAFE_UNION_ALIGNED_BYTES_H_

#include <array>
#include <cstddef>
#include <cstdint>

#include "horus/internal/type_traits.h"
#include "horus/pointer/unsafe_cast.h"

namespace horus {
namespace horus_internal {

/// Same as `std::aligned_union<>`, but ignoring `void` types.
template <std::size_t kAlignment, std::size_t kSize, class... Ts>
struct AlignedUnion {
  /// Data of the union. We cannot use `std::uint8_t[]` because of A18-1-1, nor
  /// `std::aligned_storage<>` because it is deprecated in C++23.
  alignas(kAlignment) std::array<std::uint8_t, kSize> data;
};
template <std::size_t kAlignment, std::size_t kSize, class T, class... Rest>
struct AlignedUnion<kAlignment, kSize, T, Rest...>
    : AlignedUnion<(alignof(T) > kAlignment ? alignof(T) : kAlignment),
                   (sizeof(T) > kSize ? sizeof(T) : kSize),  // NOLINT(*-sizeof-expression)
                   Rest...> {};
template <std::size_t kAlignment, std::size_t kSize, class... Rest>
struct AlignedUnion<kAlignment, kSize, void, Rest...> : AlignedUnion<kAlignment, kSize, Rest...> {};

/// A union type with no tag.
template <class... Ts>
class UnsafeUnion final {
 public:
  /// Constructs an uninitialized unsafe union.
  constexpr UnsafeUnion() noexcept = default;

  /// An `UnsafeUnion` can neither be copied nor moved.
  UnsafeUnion(const UnsafeUnion&) = delete;
  /// An `UnsafeUnion` can neither be copied nor moved.
  UnsafeUnion& operator=(const UnsafeUnion&) = delete;
  /// An `UnsafeUnion` can neither be copied nor moved.
  UnsafeUnion(UnsafeUnion&&) = delete;
  /// An `UnsafeUnion` can neither be copied nor moved.
  UnsafeUnion& operator=(UnsafeUnion&&) = delete;

  /// Default (noop) destructor.
  ~UnsafeUnion() noexcept = default;

  /// Returns a pointer to the contents of the union interpreted as `T`.
  template <class T>
  T* As() noexcept {
    static_assert(PackIndexOf<0, T, Ts...>::value < sizeof...(Ts), "T must be in Ts...");
    static_assert(sizeof(T*) == sizeof(void*), "T cannot be a virtual class");

    return UnsafePointerCast<T>(&data_);
  }

  /// Returns a pointer to the contents of the union interpreted as `T`.
  template <class T>
  const T* As() const noexcept {
    static_assert(PackIndexOf<0, T, Ts...>::value < sizeof...(Ts), "T must be in Ts...");
    static_assert(sizeof(const T*) == sizeof(void*), "T cannot be a virtual class");

    return UnsafePointerCast<const T>(&data_);
  }

 private:
  /// The underlying data.
  AlignedUnion<1, 1, Ts...> data_{};
};

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_INTERNAL_UNSAFE_UNION_ALIGNED_BYTES_H_
