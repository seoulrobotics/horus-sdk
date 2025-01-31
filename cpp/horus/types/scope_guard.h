/// @file
///
/// `Defer()` and the `ScopeGuard` class.

#ifndef HORUS_TYPES_SCOPE_GUARD_H_
#define HORUS_TYPES_SCOPE_GUARD_H_

#include <type_traits>
#include <utility>

#include "horus/types/move_marker.h"

namespace horus {

/// An object which executes a given function when it goes out of scope.
///
/// Example:
///
/// ```
/// foo_++;
/// const auto guard = Defer([this]() noexcept { foo_--; });
/// ```
///
/// Which is equivalent to:
///
/// ```
/// foo_++;
/// try { /* rest of block */ }
/// catch (...) { foo_--; throw; }
/// foo_--;
/// ```
///
/// `ScopeGuard`s can be moved, though it's generally recommended _not_ to move them as they are
/// supposed to represent a lexical scope. However, making them movable allows them to be used as
/// "destructors" for lambdas, which is often useful.
///
/// @tparam F The type of the function; for automatic deduction, use `Defer()`.
template <class F>
class ScopeGuard final {
  static_assert(noexcept(std::declval<const F&>()()), "ScopeGuard handler must be noexcept");
  static_assert(std::is_nothrow_move_constructible<F>::value,
                "ScopeGuard handler must be noexcept-move-constructible");

 public:
  /// Creates a `ScopeGuard` which invokes `to_invoke()` on destruction.
  explicit constexpr ScopeGuard(F&& to_invoke) noexcept : f_(std::move(to_invoke)) {}

  /// Cannot copy a `ScopeGuard`.
  ScopeGuard(const ScopeGuard&) = delete;
  /// Cannot copy a `ScopeGuard`.
  ScopeGuard& operator=(const ScopeGuard&) = delete;

  /// Moves the `ScopeGuard`. `other` will be deactivated.
  constexpr ScopeGuard(ScopeGuard&& other) noexcept = default;
  /// Cannot move-assign a `ScopeGuard`.
  constexpr ScopeGuard& operator=(ScopeGuard&& other) & noexcept = delete;

  /// Disengages the scope guard, ensuring that it won't be called.
  void Disengage() noexcept { static_cast<void>(MoveMarker{std::move(move_marker_)}); }

  /// Destroys the `ScopeGuard`, invoking `F`.
  ~ScopeGuard() {
    if (!move_marker_.WasMovedFrom()) {
      f_();
    }
  }

 private:
  /// The function to invoke.
  F f_;
  /// A marker used to determine if the `ScopeGuard` was moved from, in which case `f_()` should not
  /// be called on destruction.
  MoveMarker move_marker_;
};

/// Returns a `ScopeGuard` which executes `to_invoke()` on destruction.
///
/// ```
/// const auto guard = Defer([&foo]() noexcept { foo.close(); });
/// ```
template <class F>
constexpr ScopeGuard<F> Defer(F to_invoke) noexcept {
  static_assert(std::is_nothrow_move_constructible<F>::value, "");
  return ScopeGuard<F>{std::move(to_invoke)};
}

}  // namespace horus

#endif  // HORUS_TYPES_SCOPE_GUARD_H_
