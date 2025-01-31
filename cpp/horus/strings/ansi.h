/// @file
///
/// Helpers for printing colored strings.

#ifndef HORUS_STRINGS_ANSI_H_
#define HORUS_STRINGS_ANSI_H_

#include <cstddef>

#include "horus/strings/str_cat.h"
#include "horus/types/string_view.h"

namespace horus {

// MARK: Colored() and helpers

/// Returns whether colors should be displayed on the terminal. This respects the `NO_COLOR`
/// environment variable.
bool DisplayTerminalColors() noexcept;

/// Indicates that a value should be formatted with some colors.
template <class T>
struct ColoredFormat {
  /// The value to format.
  const T& value;
  /// The ANSI escape sequence which specifies how the value should be formatted, excluding the `\e`
  /// prefix and `m` suffix.
  StringView escape_sequence;
};

/// Returns an object which, when formatted, will add colors as specified. The returned value
/// automatically disables colors if `DisplayTerminalColors()` is false.
template <class T>
constexpr ColoredFormat<T> Colored(StringView escape_sequence, const T& value) noexcept {
  return {value, escape_sequence};
}

/// Returns the expected size of formatting `value`.
template <class T>
constexpr void HorusStringifySize(const ColoredFormat<T>& value) noexcept {
  // Assuming that colors are enabled...
  constexpr std::size_t kFormattingSize{2 * 2 + 2};  // "\e[" x2, "0m"
  return kFormattingSize + value.escape_sequence.size() + StrExpectedSize(value.value);
}

/// Appends `count` characters `pad` to the given `sink`.
template <class Sink, class T>
constexpr void HorusStringify(Sink& sink, const ColoredFormat<T>& value) noexcept(
    noexcept(HorusStringify(sink, value.value))) {
  if (!DisplayTerminalColors()) {
    HorusStringify(sink, value.value);
    return;
  }
  sink.Append("\x1b[");
  sink.Append(value.escape_sequence);
  sink.Append("m");
  HorusStringify(sink, value.value);
  sink.Append("\x1b[0m");
}

// MARK: Specific colors

/// Returns `Colored(<black color>, value)`.
template <class T>
constexpr ColoredFormat<T> BlackColored(const T& value) noexcept {
  return Colored("30", value);
}
/// Returns `Colored(<red color>, value)`.
template <class T>
constexpr ColoredFormat<T> RedColored(const T& value) noexcept {
  return Colored("31", value);
}
/// Returns `Colored(<green color>, value)`.
template <class T>
constexpr ColoredFormat<T> GreenColored(const T& value) noexcept {
  return Colored("32", value);
}
/// Returns `Colored(<yellow color>, value)`.
template <class T>
constexpr ColoredFormat<T> YellowColored(const T& value) noexcept {
  return Colored("33", value);
}
/// Returns `Colored(<blue color>, value)`.
template <class T>
constexpr ColoredFormat<T> BlueColored(const T& value) noexcept {
  return Colored("34", value);
}
/// Returns `Colored(<magenta color>, value)`.
template <class T>
constexpr ColoredFormat<T> MagentaColored(const T& value) noexcept {
  return Colored("35", value);
}
/// Returns `Colored(<cyan color>, value)`.
template <class T>
constexpr ColoredFormat<T> CyanColored(const T& value) noexcept {
  return Colored("36", value);
}
/// Returns `Colored(<white color>, value)`.
template <class T>
constexpr ColoredFormat<T> WhiteColored(const T& value) noexcept {
  return Colored("37", value);
}

}  // namespace horus

#endif  // HORUS_STRINGS_ANSI_H_
