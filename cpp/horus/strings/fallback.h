/// @file
///
/// The `WithFallback()` function.

#ifndef HORUS_STRINGS_FALLBACK_H_
#define HORUS_STRINGS_FALLBACK_H_

#include <type_traits>
#include <utility>

#include "horus/types/string_view.h"

namespace horus {
namespace horus_internal {

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

}  // namespace horus_internal

/// Defines `value` to be true iff `HorusStringify(Sink&, const T&)` is well-formed and returns
/// `void`.
template <class T, class Enable = void>
struct CanStringify : std::false_type {};
template <class T>
struct CanStringify<T, decltype(HorusStringify(std::declval<horus_internal::DummySink&>(),
                                               std::declval<const T&>()))> : std::true_type {};

/// Indicates that a value should be formatted with a fallback value if it does not support
/// formatting.
template <class T, class Fallback>
struct WithFallbackFormat {
  /// The value to format (if possible).
  const T& value;
  /// The fallback value to format.
  const Fallback& fallback;
};

/// Returns an object which, when formatted, will format `value` if possible and fall back to
/// `fallback`.
template <class T, class Fallback>
constexpr WithFallbackFormat<T, Fallback> WithFallback(const T& value,
                                                       const Fallback& fallback) noexcept {
  return {value, fallback};
}

/// Appends `value.value` to `sink`.
template <class Sink, class T, class Fallback, std::enable_if_t<CanStringify<T>::value>* = nullptr>
constexpr void HorusStringify(Sink& sink, const WithFallbackFormat<T, Fallback>& value) noexcept(
    noexcept(HorusStringify(sink, value.value))) {
  HorusStringify(sink, value.value);
}

/// Appends `value.fallback` to `sink`.
template <class Sink, class T, class Fallback, std::enable_if_t<!CanStringify<T>::value>* = nullptr>
constexpr void HorusStringify(Sink& sink, const WithFallbackFormat<T, Fallback>& value) noexcept(
    noexcept(HorusStringify(sink, value.fallback))) {
  HorusStringify(sink, value.fallback);
}

}  // namespace horus

#endif  // HORUS_STRINGS_FALLBACK_H_
