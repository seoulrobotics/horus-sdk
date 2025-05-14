/// @file
///
/// Tag type used by `OneOf`.

#ifndef HORUS_TYPES_ONE_OF_TAG_H_
#define HORUS_TYPES_ONE_OF_TAG_H_

#include <cassert>
#include <type_traits>

#include "horus/types/union.h"

namespace horus {
namespace one_of_internal {

/// Defines `Type` to be `NotVoid::type` if none of `Ts` is void, and to be `void` otherwise.
template <class NotVoid, class... Ts>
struct ReduceVoid;
template <class NotVoid>
struct ReduceVoid<NotVoid> {
  using Type = typename NotVoid::type;
};
template <class NotVoid, class... Rest>
struct ReduceVoid<NotVoid, void, Rest...> {
  using Type = void;
};
template <class NotVoid, class _, class... Rest>
struct ReduceVoid<NotVoid, _, Rest...> : ReduceVoid<NotVoid, Rest...> {};

/// Same as `std::common_type_t<Ts...>`, but yields `void` if any of `Ts...` is `void`.
template <class... Ts>
using CommonType = typename ReduceVoid<std::common_type<Ts...>, Ts...>::Type;

/// Common result type of `F(T)` invoked on any `Ts` in `T`.
template <class F, class... Ts>
using CommonResultType = CommonType<decltype(std::declval<F>()(std::declval<Ts>()))...>;

/// Tag types.
///
/// Tag types are unscoped enums with no underlying type in order to exhaustively switch over them
/// while being compliant with MISRA C++:2023 9.4.2:
///
/// > If the condition of a switch statement is an unscoped enumeration type that does not have a
/// > fixed underlying type, and all the enumerators are listed in case labels, then a default label
/// > is not required.
///
/// @see `TagHelpers<T0, T1>` for a documentation of its members (which can generalize to any number
/// of `Ts...`).
///
/// `Ts...` is expected to have unique types, which is enforced by `OneOf`.
template <class... Ts>
struct TagHelpers;

template <class T0>
struct TagHelpers<T0> {
  enum Tag {
    k0,
  };
  using Union = UnsafeUnion<T0>;

  static constexpr Tag TagFor(T0* /*ptr*/) noexcept { return k0; }

  template <class F>
  static constexpr CommonResultType<F const&, T0*> Dispatch(F const& invocable, Tag tag) {
    switch (tag) {
      case k0:
        return invocable(static_cast<T0*>(nullptr));
      default: {
        assert(false);
        return invocable(static_cast<T0*>(nullptr));
      }
    }
  }
};

template <class T0, class T1>
struct TagHelpers<T0, T1> {
  /// The actual tag enum.
  enum Tag {
    k0,
    k1,
  };
  /// A union which stores `T0` or `T1`.
  using Union = UnsafeUnion<T0, T1>;

  /// Returns the tag for `T0`. This is invoked like `TagFor(static_cast<T0*>(nullptr))`, which
  /// automatically resolves the correct overload.
  static constexpr Tag TagFor(T0* /*ptr*/) noexcept { return k0; }

  /// Returns the tag for `T1`.
  static constexpr Tag TagFor(T1* /*ptr*/) noexcept { return k1; }

  /// Invokes `invocable(static_cast<T*>(nullptr))` with `T` `T0` if `tag == k0` or `T1` if
  /// `tag == k1`.
  template <class F>
  static constexpr CommonResultType<F const&, T0*, T1*> Dispatch(F const& invocable, Tag tag) {
    switch (tag) {
      case k0:
        return invocable(static_cast<T0*>(nullptr));
      case k1:
        return invocable(static_cast<T1*>(nullptr));
      default: {
        assert(false);
        return invocable(static_cast<T0*>(nullptr));
      }
    }
  }
};

template <class T0, class T1, class T2>
struct TagHelpers<T0, T1, T2> {
  enum Tag {
    k0,
    k1,
    k2,
  };
  using Union = UnsafeUnion<T0, T1, T2>;

  static constexpr Tag TagFor(T0* /*ptr*/) noexcept { return k0; }
  static constexpr Tag TagFor(T1* /*ptr*/) noexcept { return k1; }
  static constexpr Tag TagFor(T2* /*ptr*/) noexcept { return k2; }

  template <class F>
  static constexpr CommonResultType<F const&, T0*, T1*, T2*> Dispatch(F const& invocable, Tag tag) {
    switch (tag) {
      case k0:
        return invocable(static_cast<T0*>(nullptr));
      case k1:
        return invocable(static_cast<T1*>(nullptr));
      case k2:
        return invocable(static_cast<T2*>(nullptr));
      default: {
        assert(false);
        return invocable(static_cast<T0*>(nullptr));
      }
    }
  }
};

template <class T0, class T1, class T2, class T3>
struct TagHelpers<T0, T1, T2, T3> {
  enum Tag {
    k0,
    k1,
    k2,
    k3,
  };
  using Union = UnsafeUnion<T0, T1, T2, T3>;

  static constexpr Tag TagFor(T0* /*ptr*/) noexcept { return k0; }
  static constexpr Tag TagFor(T1* /*ptr*/) noexcept { return k1; }
  static constexpr Tag TagFor(T2* /*ptr*/) noexcept { return k2; }
  static constexpr Tag TagFor(T3* /*ptr*/) noexcept { return k3; }

  template <class F>
  static constexpr CommonResultType<F const&, T0*, T1*, T2*, T3*> Dispatch(F const& invocable,
                                                                           Tag tag) {
    switch (tag) {
      case k0:
        return invocable(static_cast<T0*>(nullptr));
      case k1:
        return invocable(static_cast<T1*>(nullptr));
      case k2:
        return invocable(static_cast<T2*>(nullptr));
      case k3:
        return invocable(static_cast<T3*>(nullptr));
      default: {
        assert(false);
        return invocable(static_cast<T0*>(nullptr));
      }
    }
  }
};

template <class T0, class T1, class T2, class T3, class T4>
struct TagHelpers<T0, T1, T2, T3, T4> {
  enum Tag {
    k0,
    k1,
    k2,
    k3,
    k4,
  };
  using Union = UnsafeUnion<T0, T1, T2, T3, T4>;

  static constexpr Tag TagFor(T0* /*ptr*/) noexcept { return k0; }
  static constexpr Tag TagFor(T1* /*ptr*/) noexcept { return k1; }
  static constexpr Tag TagFor(T2* /*ptr*/) noexcept { return k2; }
  static constexpr Tag TagFor(T3* /*ptr*/) noexcept { return k3; }
  static constexpr Tag TagFor(T4* /*ptr*/) noexcept { return k4; }

  template <class F>
  static constexpr CommonResultType<F const&, T0*, T1*, T2*, T3*, T4*> Dispatch(F const& invocable,
                                                                                Tag tag) {
    switch (tag) {
      case k0:
        return invocable(static_cast<T0*>(nullptr));
      case k1:
        return invocable(static_cast<T1*>(nullptr));
      case k2:
        return invocable(static_cast<T2*>(nullptr));
      case k3:
        return invocable(static_cast<T3*>(nullptr));
      case k4:
        return invocable(static_cast<T4*>(nullptr));
      default: {
        assert(false);
        return invocable(static_cast<T0*>(nullptr));
      }
    }
  }
};

}  // namespace one_of_internal
}  // namespace horus

#endif  // HORUS_TYPES_ONE_OF_TAG_H_
