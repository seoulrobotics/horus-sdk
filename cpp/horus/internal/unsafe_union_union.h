/// @file
///
/// The `UnsafeUnion` type (implemented with a `union`).
///
/// `union`s are *not* Autosar compliant but significantly improve debugging (as unions are
/// displayed as their possible fields in the debugger, rather than as bytes).

#ifndef HORUS_INTERNAL_UNSAFE_UNION_UNION_H_
#define HORUS_INTERNAL_UNSAFE_UNION_UNION_H_

#include <cstddef>
#include <type_traits>

#include "horus/internal/type_traits.h"

namespace horus {
namespace horus_internal {

/// A union of types `Ts...`.
template <class... Ts>
union UnionTypeFor;

/// Defines the constructor/destructor for a `UnionTypeFor`.
///
/// Using macros is typically forbidden by Autosar, but this file is only compiled for
/// ease-of-debugging (and is not compliant due to unions anyway), so it is permitted here.
#define HORUS_SDK_DEFINE_UNION_TYPE_CTOR_DTOR            \
  UnionTypeFor() noexcept {}                             \
  ~UnionTypeFor() noexcept {}                            \
  UnionTypeFor(const UnionTypeFor&) = delete;            \
  UnionTypeFor& operator=(const UnionTypeFor&) = delete; \
  UnionTypeFor(UnionTypeFor&&) = delete;                 \
  UnionTypeFor& operator=(UnionTypeFor&&) = delete

/// An empty union.
template <>
union UnionTypeFor<> {};

/// A union of one type.
template <class A>
union UnionTypeFor<A> {
  HORUS_SDK_DEFINE_UNION_TYPE_CTOR_DTOR;

  /// `A` field.
  A a;

  /// Returns a pointer to `A`.
  constexpr const A* Get(std::integral_constant<std::size_t, 0> /* index */) const& noexcept {
    return &a;
  }
  /// Returns a pointer to `A`.
  constexpr A* Get(std::integral_constant<std::size_t, 0> /* index */) & noexcept { return &a; }
};

/// A union of two types.
template <class A, class B>
union UnionTypeFor<A, B> {
  HORUS_SDK_DEFINE_UNION_TYPE_CTOR_DTOR;

  /// `A` field.
  A a;
  /// `B` field.
  B b;

  /// Returns a pointer to `A`.
  constexpr const A* Get(std::integral_constant<std::size_t, 0> /* index */) const& noexcept {
    return &a;
  }
  /// Returns a pointer to `A`.
  constexpr A* Get(std::integral_constant<std::size_t, 0> /* index */) & noexcept { return &a; }

  /// Returns a pointer to `B`.
  constexpr const B* Get(std::integral_constant<std::size_t, 1> /* index */) const& noexcept {
    return &b;
  }
  /// Returns a pointer to `B`.
  constexpr B* Get(std::integral_constant<std::size_t, 1> /* index */) & noexcept { return &b; }
};

/// A union of three types.
template <class A, class B, class C>
union UnionTypeFor<A, B, C> {
  HORUS_SDK_DEFINE_UNION_TYPE_CTOR_DTOR;

  /// `A` field.
  A a;
  /// `B` field.
  B b;
  /// `C` field.
  C c;

  /// Returns a pointer to `A`.
  constexpr const A* Get(std::integral_constant<std::size_t, 0> /* index */) const& noexcept {
    return &a;
  }
  /// Returns a pointer to `A`.
  constexpr A* Get(std::integral_constant<std::size_t, 0> /* index */) & noexcept { return &a; }

  /// Returns a pointer to `B`.
  constexpr const B* Get(std::integral_constant<std::size_t, 1> /* index */) const& noexcept {
    return &b;
  }
  /// Returns a pointer to `B`.
  constexpr B* Get(std::integral_constant<std::size_t, 1> /* index */) & noexcept { return &b; }

  /// Returns a pointer to `C`.
  constexpr const C* Get(std::integral_constant<std::size_t, 2> /* index */) const& noexcept {
    return &c;
  }
  /// Returns a pointer to `C`.
  constexpr C* Get(std::integral_constant<std::size_t, 2> /* index */) & noexcept { return &c; }
};

/// A union of four or more types.
template <class A, class B, class C, class... Rest>
union UnionTypeFor<A, B, C, Rest...> {
  HORUS_SDK_DEFINE_UNION_TYPE_CTOR_DTOR;

  /// `A` field.
  A a;
  /// `B` field.
  B b;
  /// `C` field.
  C c;

  /// Other fields.
  UnionTypeFor<Rest...> other;

  /// Returns a pointer to `A`.
  constexpr const A* Get(std::integral_constant<std::size_t, 0> /* index */) const& noexcept {
    return &a;
  }
  /// Returns a pointer to `A`.
  constexpr A* Get(std::integral_constant<std::size_t, 0> /* index */) & noexcept { return &a; }

  /// Returns a pointer to `B`.
  constexpr const B* Get(std::integral_constant<std::size_t, 1> /* index */) const& noexcept {
    return &b;
  }
  /// Returns a pointer to `B`.
  constexpr B* Get(std::integral_constant<std::size_t, 1> /* index */) & noexcept { return &b; }

  /// Returns a pointer to `C`.
  constexpr const C* Get(std::integral_constant<std::size_t, 2> /* index */) const& noexcept {
    return &c;
  }
  /// Returns a pointer to `C`.
  constexpr C* Get(std::integral_constant<std::size_t, 2> /* index */) & noexcept { return &c; }

  /// Returns a pointer to some other type.
  template <std::size_t Index>
  constexpr const auto* Get(
      std::integral_constant<std::size_t, Index> /* index */) const& noexcept {
    return other.Get(std::integral_constant<std::size_t, Index - 3>{});
  }
  /// Returns a pointer to some other type.
  template <std::size_t Index>
  constexpr auto* Get(std::integral_constant<std::size_t, Index> /* index */) & noexcept {
    return other.Get(std::integral_constant<std::size_t, Index - 3>{});
  }
};

#undef HORUS_SDK_DEFINE_UNION_TYPE_CTOR_DTOR

/// A union type with no tag.
template <class... Ts>
class UnsafeUnion final {
 public:
  /// Constructs an uninitialized unsafe union.
  UnsafeUnion() noexcept = default;

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

    return data_.Get(std::integral_constant<std::size_t, PackIndexOf<0, T, Ts...>::value>{});
  }

  /// Returns a pointer to the contents of the union interpreted as `T`.
  template <class T>
  const T* As() const noexcept {
    static_assert(PackIndexOf<0, T, Ts...>::value < sizeof...(Ts), "T must be in Ts...");
    static_assert(sizeof(const T*) == sizeof(void*), "T cannot be a virtual class");

    return data_.Get(std::integral_constant<std::size_t, PackIndexOf<0, T, Ts...>::value>{});
  }

 private:
  /// The underlying data.
  UnionTypeFor<std::conditional_t<std::is_void<Ts>::value, std::nullptr_t, Ts>...> data_;
};

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_INTERNAL_UNSAFE_UNION_UNION_H_
