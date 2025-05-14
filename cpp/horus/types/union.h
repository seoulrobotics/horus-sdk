/// @file
///
/// The `UnsafeUnion<Ts...>` type.

#ifndef HORUS_TYPES_UNION_H_
#define HORUS_TYPES_UNION_H_

/// Defines the constructor/destructor for a `UnionTypeFor`.
#define HORUS_DEFINE_UNION_TYPE_CTOR_DTOR                \
  UnionTypeFor() noexcept {}                             \
  ~UnionTypeFor() noexcept {}                            \
  UnionTypeFor(const UnionTypeFor&) = delete;            \
  UnionTypeFor& operator=(const UnionTypeFor&) = delete; \
  UnionTypeFor(UnionTypeFor&&) = delete;                 \
  UnionTypeFor& operator=(UnionTypeFor&&) = delete

namespace horus {
namespace union_internal {

// NOLINTBEGIN(*-union-access): we don't want variant, we want a union

/// A union of types `Ts...`.
template <class... Ts>
union UnionTypeFor;

/// An empty union.
template <>
union UnionTypeFor<> final {};

/// A union of one type.
template <class A>
union UnionTypeFor<A> final {
  HORUS_DEFINE_UNION_TYPE_CTOR_DTOR;

  /// `A` field.
  A a;

  /// Returns a pointer to `A`.
  constexpr A const* Get(A* /*ptr*/) const& noexcept { return &a; }
  /// Returns a pointer to `A`.
  constexpr A* Get(A* /*ptr*/) & noexcept { return &a; }
  /// Deleted to prevent use-after-frees.
  constexpr void Get(A* /*ptr*/) && noexcept = delete;
};

/// A union of two types.
template <class A, class B>
union UnionTypeFor<A, B> final {
  HORUS_DEFINE_UNION_TYPE_CTOR_DTOR;

  /// `A` field.
  A a;
  /// `B` field.
  B b;

  /// Returns a pointer to `A`.
  constexpr A const* Get(A* /*ptr*/) const& noexcept { return &a; }
  /// Returns a pointer to `A`.
  constexpr A* Get(A* /*ptr*/) & noexcept { return &a; }
  /// Deleted to prevent use-after-frees.
  constexpr void Get(A* /*ptr*/) && noexcept = delete;

  /// Returns a pointer to `B`.
  constexpr B const* Get(B* /*ptr*/) const& noexcept { return &b; }
  /// Returns a pointer to `B`.
  constexpr B* Get(B* /*ptr*/) & noexcept { return &b; }
  /// Deleted to prevent use-after-frees.
  constexpr void Get(B* /*ptr*/) && noexcept = delete;
};

/// A union of three types.
template <class A, class B, class C>
union UnionTypeFor<A, B, C> final {
  HORUS_DEFINE_UNION_TYPE_CTOR_DTOR;

  /// `A` field.
  A a;
  /// `B` field.
  B b;
  /// `C` field.
  C c;

  /// Returns a pointer to `A`.
  constexpr A const* Get(A* /*ptr*/) const& noexcept { return &a; }
  /// Returns a pointer to `A`.
  constexpr A* Get(A* /*ptr*/) & noexcept { return &a; }
  /// Deleted to prevent use-after-frees.
  constexpr void Get(A* /*ptr*/) && noexcept = delete;

  /// Returns a pointer to `B`.
  constexpr B const* Get(B* /*ptr*/) const& noexcept { return &b; }
  /// Returns a pointer to `B`.
  constexpr B* Get(B* /*ptr*/) & noexcept { return &b; }
  /// Deleted to prevent use-after-frees.
  constexpr void Get(B* /*ptr*/) && noexcept = delete;

  /// Returns a pointer to `C`.
  constexpr C const* Get(C* /*ptr*/) const& noexcept { return &c; }
  /// Returns a pointer to `C`.
  constexpr C* Get(C* /*ptr*/) & noexcept { return &c; }
  /// Deleted to prevent use-after-frees.
  constexpr void Get(C* /*ptr*/) && noexcept = delete;
};

/// A union of four or more types.
///
/// In theory we could use this "trick" with an inner union for specializations above, but we don't
/// to improve the debugging experience, as "flattened" unions are easier to inspect.
template <class A, class B, class C, class... Rest>
union UnionTypeFor<A, B, C, Rest...> final {
  HORUS_DEFINE_UNION_TYPE_CTOR_DTOR;

  /// `A` field.
  A a;
  /// `B` field.
  B b;
  /// `C` field.
  C c;

  /// Other fields.
  UnionTypeFor<Rest...> other;

  /// Returns a pointer to `A`.
  constexpr A const* Get(A* /*ptr*/) const& noexcept { return &a; }
  /// Returns a pointer to `A`.
  constexpr A* Get(A* /*ptr*/) & noexcept { return &a; }
  /// Deleted to prevent use-after-frees.
  constexpr void Get(A* /*ptr*/) && noexcept = delete;

  /// Returns a pointer to `B`.
  constexpr B const* Get(B* /*ptr*/) const& noexcept { return &b; }
  /// Returns a pointer to `B`.
  constexpr B* Get(B* /*ptr*/) & noexcept { return &b; }
  /// Deleted to prevent use-after-frees.
  constexpr void Get(B* /*ptr*/) && noexcept = delete;

  /// Returns a pointer to `C`.
  constexpr C const* Get(C* /*ptr*/) const& noexcept { return &c; }
  /// Returns a pointer to `C`.
  constexpr C* Get(C* /*ptr*/) & noexcept { return &c; }
  /// Deleted to prevent use-after-frees.
  constexpr void Get(C* /*ptr*/) && noexcept = delete;

  /// Returns a pointer to some other type.
  template <class T>
  constexpr T const* Get(T* ptr) const& noexcept {
    return other.Get(ptr);
  }
  /// Returns a pointer to some other type.
  template <class T>
  constexpr T* Get(T* ptr) & noexcept {
    return other.Get(ptr);
  }
  /// Deleted to prevent use-after-frees.
  template <class T>
  constexpr void Get(T* /*ptr*/) && noexcept = delete;
};

#undef HORUS_DEFINE_UNION_TYPE_CTOR_DTOR

// NOLINTEND(*-union-access)

/// An empty struct.
struct Empty {};

/// Defines `Type = T` if `T` is not `void`, and `Type = Empty` otherwise.
template <class T>
struct VoidToEmptyImpl {
  using Type = T;
};
template <>
struct VoidToEmptyImpl<void> {
  using Type = Empty;
};
template <class T>
using VoidToEmpty = typename VoidToEmptyImpl<T>::Type;

}  // namespace union_internal

/// A union type with no tag.
template <class... Ts>
class UnsafeUnion final {
 public:
  /// Constructs an uninitialized unsafe union.
  UnsafeUnion() noexcept = default;

  /// An `UnsafeUnion` can neither be copied nor moved.
  UnsafeUnion(UnsafeUnion const&) = delete;
  /// An `UnsafeUnion` can neither be copied nor moved.
  UnsafeUnion& operator=(UnsafeUnion const&) = delete;
  /// An `UnsafeUnion` can neither be copied nor moved.
  UnsafeUnion(UnsafeUnion&&) = delete;
  /// An `UnsafeUnion` can neither be copied nor moved.
  UnsafeUnion& operator=(UnsafeUnion&&) = delete;

  /// Default (noop) destructor.
  ~UnsafeUnion() noexcept = default;

  /// Returns a pointer to the contents of the union interpreted as `T`.
  template <class T>
  T* As() & noexcept {
    // NOLINTNEXTLINE(*-union-access): we are the wrapper for the union, so we must access it
    return data_.Get(static_cast<T*>(nullptr));
  }

  /// Returns a pointer to the contents of the union interpreted as `T`.
  template <class T>
  T const* As() const& noexcept {
    // NOLINTNEXTLINE(*-union-access): we are the wrapper for the union, so we must access it
    return data_.Get(static_cast<T*>(nullptr));
  }

 private:
  /// The underlying data.
  union_internal::UnionTypeFor<union_internal::VoidToEmpty<Ts>...> data_;
};

}  // namespace horus

#endif  // HORUS_TYPES_UNION_H_
