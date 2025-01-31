/// @file
///
/// The `MoveMarker` class.

#ifndef HORUS_TYPES_MOVE_MARKER_H_
#define HORUS_TYPES_MOVE_MARKER_H_

namespace horus {

/// An object which stores whether or not it was moved to / from.
class MoveMarker final {
 public:
  /// Constructs a new `MoveMarker` for which `WasMovedFrom()` returns false.
  constexpr MoveMarker() noexcept = default;

  /// Trivial destructor.
  ~MoveMarker() noexcept = default;

  /// Cannot copy a move marker.
  MoveMarker(const MoveMarker&) = delete;
  /// Cannot copy a move marker.
  MoveMarker& operator=(const MoveMarker&) = delete;

  /// Moves `other` into `this`, setting `other`'s `WasMovedFrom()` value to true.
  constexpr MoveMarker(MoveMarker&& other) noexcept : moved_from_(other.moved_from_) {
    other.moved_from_ = true;
  }

  /// Moves `other` into `this`, setting `other`'s `WasMovedFrom()` value to true.
  constexpr MoveMarker& operator=(MoveMarker&& other) noexcept {
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

}  // namespace horus

#endif  // HORUS_TYPES_MOVE_MARKER_H_
