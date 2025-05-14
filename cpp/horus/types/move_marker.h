/// @file
///
/// The `MoveMarker` class.

#ifndef HORUS_TYPES_MOVE_MARKER_H_
#define HORUS_TYPES_MOVE_MARKER_H_

#include "horus/attributes.h"
namespace horus {

/// An object which stores whether or not it was moved to / from.
class MoveMarker final {
 public:
  /// Constructs a new `MoveMarker` for which `WasMovedFrom()` returns false.
  constexpr MoveMarker() noexcept = default;

  /// Trivial destructor.
  ~MoveMarker() noexcept = default;

  /// Cannot copy a move marker.
  MoveMarker(MoveMarker const&) = delete;
  /// Cannot copy a move marker.
  MoveMarker& operator=(MoveMarker const&) = delete;

  /// Moves `other` into `this`, setting `other`'s `WasMovedFrom()` value to true.
  constexpr MoveMarker(MoveMarker&& other) noexcept : moved_from_(other.moved_from_) {
    other.moved_from_ = true;
  }

  /// Moves `other` into `this`, setting `other`'s `WasMovedFrom()` value to true.
  constexpr MoveMarker& operator=(MoveMarker&& other) & noexcept {
    if (this != &other) {
      moved_from_ = other.moved_from_;
      other.moved_from_ = true;
    }
    return *this;
  }

  /// Returns whether the marker was moved from, i.e. it was used to move-construct or move-assign
  /// another `MoveMarker`.
  ///
  /// @note `WasMovedFrom()` may return true at some point, and then later return false if it
  /// is assigned to from another `MoveMarker`.
  constexpr bool WasMovedFrom() const noexcept { return moved_from_; }

 private:
  /// Value of `WasMovedFrom()`.
  bool moved_from_{false};
};

/// An object which stores a value of type `T` and sets it to `get_moved_from_value()` when moved.
template <class T, T get_moved_from_value()>
class MoveValueWith final {
  // Note: we are given a function which returns a constant `T` value as a template parameter so
  // `T` should be trivial.

 public:
  /// Stores `value` inside the object. Note that nothing prevents `value` from being equal to
  /// `get_moved_from_value()`.
  constexpr explicit MoveValueWith(T value) noexcept : value_(value) {}

  /// Destroys the inner `T`.
  ~MoveValueWith() noexcept = default;

  /// Cannot copy a move marker.
  MoveValueWith(MoveValueWith const&) = delete;
  /// Cannot copy a move marker.
  MoveValueWith& operator=(MoveValueWith const&) = delete;

  /// Moves `other` into `this`, setting `other`'s inner value to `get_moved_from_value()`.
  constexpr MoveValueWith(MoveValueWith&& other) noexcept : value_(other.value_) {
    other.value_ = get_moved_from_value();
  }

  /// Moves `other` into `this`, setting `other`'s inner value to `get_moved_from_value()`.
  constexpr MoveValueWith& operator=(MoveValueWith&& other) & noexcept {
    if (this != &other) {
      value_ = other.value_;
      other.value_ = get_moved_from_value();
    }
    return *this;
  }

  /// Returns a reference to the inner value.
  constexpr T const& Get() const& noexcept HORUS_LIFETIME_BOUND { return value_; }
  /// Returns a reference to the inner value.
  constexpr T& Get() & noexcept HORUS_LIFETIME_BOUND { return value_; }

  /// Returns whether the marker was moved from, i.e. it was used to move-construct or move-assign
  /// another `MoveValueWith<T>` and its value is `get_moved_from_value()`.
  ///
  /// @note `WasMovedFrom()` may return true at some point, and then later return false if it
  /// is assigned to from another `MoveValueWith`.
  ///
  /// @note `WasMovedFrom()` will return true even if not moved from if the value is equal to
  /// `get_moved_from_value()`.
  constexpr bool WasMovedFrom() const noexcept { return value_ == get_moved_from_value(); }

 private:
  /// The underlying value.
  T value_;
};

namespace move_marker_internal {

/// A function which always returns a copy of `constant` used in `MoveValueTo`.
template <class T, T constant>
constexpr T ReturnConstant() noexcept {
  return constant;
}

}  // namespace move_marker_internal

/// An object which stores a value and sets it to `moved_from_value` on move.
template <class T, T moved_from_value>
using MoveValueTo = MoveValueWith<T, move_marker_internal::ReturnConstant<T, moved_from_value>>;

}  // namespace horus

#endif  // HORUS_TYPES_MOVE_MARKER_H_
