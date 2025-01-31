/// @file
///
/// The `PadLeft()` and `PadRight()` functions.

#ifndef HORUS_STRINGS_PAD_H_
#define HORUS_STRINGS_PAD_H_

#include <algorithm>
#include <array>
#include <cstddef>

#include "horus/internal/attributes.h"
#include "horus/strings/str_cat.h"
#include "horus/strings/str_sink.h"
#include "horus/types/span.h"
#include "horus/types/string_view.h"

namespace horus {

/// Indicates that a value should be formatted with some padding.
template <class T>
struct PadFormat {
  /// The value to format.
  const T& value;
  /// The full width requested to format the value.
  std::size_t full_width;
  /// Whether the padding is _prepended_ to the output.
  bool before;
  /// The character to pad with.
  char pad_with;
};

/// Returns an object which, when formatted, will pad `value` left to make sure at least
/// `full_width` characters are written.
template <class T>
constexpr PadFormat<T> PadLeftBy(std::size_t full_width,
                                 const T& value HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND,
                                 char pad_with = ' ') noexcept {
  return {value, full_width, /*before=*/true, pad_with};
}

/// Returns an object which, when formatted, will pad `value` right to make sure at least
/// `full_width` characters are written.
template <class T>
constexpr PadFormat<T> PadRightBy(std::size_t full_width,
                                  const T& value HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND,
                                  char pad_with = ' ') noexcept {
  return {value, full_width, /*before=*/false, pad_with};
}

/// Returns the expected size of formatting `value`.
template <class T>
constexpr void HorusStringifySize(const PadFormat<T>& value) noexcept {
  return std::max(value.padding, StrExpectedSize(value.value));
}

namespace horus_internal {

/// Appends `count` characters `pad` to the given `sink`.
template <class Sink>
constexpr void AppendPadding(Sink& sink, std::size_t count,
                             char pad) noexcept(IsNoexceptSink<Sink>()) {
  std::array<char, 8> buffer{};
  for (char& chr : Span<char>{buffer}) {
    chr = pad;
  }
  const StringView chunk{buffer.data(), buffer.size()};

  std::size_t const chunks{count / buffer.size()};
  std::size_t const remains{count % buffer.size()};
  for (std::size_t i{0}; i < chunks; ++i) {
    sink.Append(chunk);
  }
  sink.Append(chunk.substr(0, remains));
}

}  // namespace horus_internal

/// Stringifies the value with padding.
template <class Sink, class T>
constexpr void HorusStringify(Sink& sink, const PadFormat<T>& value) noexcept(
    noexcept(HorusStringify(sink, value.value))) {
  std::size_t const size{StrExpectedSize(value.value)};
  // If `size` cannot be determined, it will always be greater than `value.full_width`.
  if (size < value.full_width && value.before) {
    horus_internal::AppendPadding(sink, value.full_width - size, value.pad_with);
  }
  StrAppendToSink(sink, value.value);
  if (size < value.full_width && !value.before) {
    horus_internal::AppendPadding(sink, value.full_width - size, value.pad_with);
  }
}

}  // namespace horus

#endif  // HORUS_STRINGS_PAD_H_
