/// @file
///
/// The `WithFallback()` function.

#ifndef HORUS_STRINGS_FALLBACK_H_
#define HORUS_STRINGS_FALLBACK_H_

#include <type_traits>
#include <utility>

#include "horus/attributes.h"
#include "horus/strings/string_view.h"

namespace horus {
namespace fallback_internal {

/// A dummy `HorusStringify()` sink object used in `CanStringify`.
class DummySink final {
 public:
  /// Cannot construct dummy class.
  DummySink() = delete;

  /// No-op.
  constexpr void Append(StringView string) noexcept {
    static_cast<void>(this);
    static_cast<void>(string);
  }
};

}  // namespace fallback_internal

/// Defines `value` to be true iff `HorusStringify(Sink&, const T&)` is well-formed and returns
/// `void`.
template <class T, class Enable = void>
struct CanStringify : std::false_type {};
template <class T>
struct CanStringify<T,
                    decltype(HorusStringify(std::declval<fallback_internal::DummySink&>(),
                                            std::declval<T const&>()))> : std::true_type {};

/// Indicates that a value should be formatted with a fallback value if it does not support
/// formatting.
template <class T, class Fallback>
struct WithFallbackFormat {
  /// The value to format (if possible).
  T const& value;
  /// The fallback value to format.
  Fallback const& fallback;
};

/// Returns an object which, when formatted, will format `value` if possible and fall back to
/// `fallback`.
template <class T, class Fallback>
constexpr WithFallbackFormat<T, Fallback> WithFallback(
    T const& value HORUS_LIFETIME_BOUND, Fallback const& fallback HORUS_LIFETIME_BOUND) noexcept {
  return {value, fallback};
}

/// Appends `value.value` to `sink`.
template <class Sink, class T, class Fallback, std::enable_if_t<CanStringify<T>::value>* = nullptr>
constexpr void HorusStringify(Sink& sink, WithFallbackFormat<T, Fallback> value) noexcept(
    noexcept(HorusStringify(sink, value.value))) {
  HorusStringify(sink, value.value);
}

/// Appends `value.fallback` to `sink`.
template <class Sink, class T, class Fallback, std::enable_if_t<!CanStringify<T>::value>* = nullptr>
constexpr void HorusStringify(Sink& sink, WithFallbackFormat<T, Fallback> value) noexcept(
    noexcept(HorusStringify(sink, value.fallback))) {
  HorusStringify(sink, value.fallback);
}

}  // namespace horus

#endif  // HORUS_STRINGS_FALLBACK_H_
