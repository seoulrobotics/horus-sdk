/// @file
///
/// Helpers related to `void`.

#ifndef HORUS_INTERNAL_VOID_H_
#define HORUS_INTERNAL_VOID_H_

#include <type_traits>
#include <utility>

namespace horus {
namespace horus_internal {

/// Function object given to `SwichVoid()` callbacks to prevent the compiler from eagerly
/// type-checking function bodies which may never be evaluated.
class Forwarder final {
 public:
  /// Does nothing.
  ///
  /// This can be used in `SwitchVoid()` functions to create a dependently-typed `void`.
  constexpr void operator()() const noexcept {}

  /// Forwards `value`.
  ///
  /// This can be used in `SwitchVoid()` functions to create a dependently-typed `T`.
  template <class T>
  constexpr T&& operator()(T&& value) const noexcept {
    return std::forward<T>(value);
  }
};

/// Evaluates to `if_void(forwarder)` if `MaybeVoid` is `void`, and to `if_not_void(forwarder)`
/// otherwise.
template <class MaybeVoid, class IfVoid, class IfNotVoid,
          std::enable_if_t<std::is_void<MaybeVoid>::value>* = nullptr>
auto SwitchVoid(IfVoid&& if_void,
                IfNotVoid&& /* if_not_void */) noexcept(noexcept(if_void(Forwarder{})))
    -> decltype(if_void(Forwarder{})) {
  return std::forward<IfVoid>(if_void)(Forwarder{});
}

/// Evaluates to `if_void(forwarder)` if `MaybeVoid` is `void`, and to `if_not_void(forwarder)`
/// otherwise.
template <class MaybeVoid, class IfVoid, class IfNotVoid,
          std::enable_if_t<!std::is_void<MaybeVoid>::value>* = nullptr>
auto SwitchVoid(IfVoid&& /* if_void */,
                IfNotVoid&& if_not_void) noexcept(noexcept(if_not_void(Forwarder{})))
    -> decltype(if_not_void(Forwarder{})) {
  return std::forward<IfNotVoid>(if_not_void)(Forwarder{});
}

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_INTERNAL_VOID_H_
