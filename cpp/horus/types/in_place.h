/// @file
///
/// The `kInPlace` constant.

#ifndef HORUS_TYPES_IN_PLACE_H_
#define HORUS_TYPES_IN_PLACE_H_

#include <cstddef>

namespace horus {

/// Type of `kInPlace`, which can be used to disambiguate constructors with forwarded arguments.
struct InPlace {};

/// Unique value of `InPlace`.
static constexpr InPlace kInPlace;

namespace horus_internal {

/// Tag type used to implement `InPlaceType<T>`.
template <class T>
struct InPlaceTypeTag {};

/// Tag type used to implement `InPlaceValue<V, v>`.
template <class V, V v>
struct InPlaceValueTag {};

}  // namespace horus_internal

// Note: the marker functions below cannot be `noexcept` as this would make them incompatible with
// the corresponding marker types in C++17. The marker types cannot be marked `noexcept` either
// because that is not valid syntax in C++14.

/// Value used to pass template arguments to constructors.
///
/// @see https://en.cppreference.com/w/cpp/utility/in_place
template <class T>
constexpr void InPlaceType(horus_internal::InPlaceTypeTag<T> /* tag */) {}

/// Marker type for `InPlaceType<T>`.
///
/// @see https://en.cppreference.com/w/cpp/utility/in_place
template <class T>
using InPlaceTypeT = void (*)(horus_internal::InPlaceTypeTag<T>);

/// Value used to pass constant values to constructors or templated functions.
template <class V, V v>
constexpr void InPlaceValue(horus_internal::InPlaceValueTag<V, v> /* tag */) {}

/// Marker type for `InPlaceValue<T, v>`.
template <class V, V v>
using InPlaceValueT = void (*)(horus_internal::InPlaceValueTag<V, v>);

}  // namespace horus

#endif  // HORUS_TYPES_IN_PLACE_H_
