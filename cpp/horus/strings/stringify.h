/// @file
///
/// Core Horus formatting functions and helpers.

#ifndef HORUS_STRINGS_STRINGIFY_H_
#define HORUS_STRINGS_STRINGIFY_H_

// IWYU wants to include <tuple> for std::array..?
// IWYU pragma: no_include <tuple>

#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "horus/attributes.h"
#include "horus/pointer/cast.h"
#include "horus/strings/string_view.h"
#include "horus/type_traits/conjunction.h"
#include "horus/types/span.h"

namespace horus {

/// Returns whether `Sink::Append()` (as used by Horus formatting functions) is `noexcept`.
template <class Sink>
constexpr bool IsNoexceptSink() noexcept {
  return noexcept(std::declval<Sink&>().Append(std::declval<StringView>()));
}

// MARK: HorusStringify() modifiers

/// Indicates that an integer should be formatted as a decimal number (the default).
template <class T>
struct DecFormat {
  /// The integer value.
  T value;
};

/// Returns a value which stringifies `value` as a decimal integer.
template <class T>
constexpr DecFormat<T> Dec(T value) noexcept {
  return DecFormat<T>{value};
}

/// Indicates that an integer should be formatted as a hexadecimal number.
template <class T>
struct HexFormat {
  /// The integer value.
  T value;
};

/// Returns a value which stringifies `value` as a hexadecimal integer.
template <class T>
constexpr HexFormat<T> Hex(T value) noexcept {
  return HexFormat<T>{value};
}

// MARK: HorusStringify() for basic types

/// Appends `value` into `sink`.
template <class Sink>
constexpr void HorusStringify(Sink& sink, StringView value) noexcept(IsNoexceptSink<Sink>()) {
  sink.Append(value);
}

/// Appends `value` into `sink`.
template <class Sink>
constexpr void HorusStringify(Sink& sink,
                              std::string const& value) noexcept(IsNoexceptSink<Sink>()) {
  sink.Append(StringView{value});
}

/// Appends `value` into `sink`.
template <class Sink>
constexpr void HorusStringify(Sink& sink, char const* value) noexcept(IsNoexceptSink<Sink>()) {
  sink.Append(StringView{value});
}

// MARK: HorusStringify() for numeric types

/// Appends `value` into `sink` as a boolean literal ("true" or "false").
template <class Sink, class T>
constexpr void HorusStringify(Sink& sink, bool value) noexcept(IsNoexceptSink<Sink>()) {
  const StringView string{value ? "true" : "false"};
  sink.Append(string);
}

/// Indicates that a character should be formatted as a character, rather than a number.
struct Char {
  /// The character value.
  char value;
};

/// Appends `value` into `sink` as a character.
template <class Sink>
constexpr void HorusStringify(Sink& sink, Char value) noexcept(IsNoexceptSink<Sink>()) {
  sink.Append(StringView{&value.value, 1});
}

namespace horus_internal {

/// Computes the decimal or hexadecimal representation size of an integer of type `T`.
template <class T>
constexpr std::size_t ComputeIntSize(bool dec, T value) noexcept {
  if (value == 0) {
    return 1;
  }
  T const base{dec ? T{10} : T{16}};
  std::size_t size{value < T{0} ? 1UL : 0UL};  // Add 1 for minus sign.
  while (value != 0) {
    value = static_cast<T>(value / base);
    ++size;
  }
  return size;
}

/// Computes the max decimal or hexadecimal size of an integer of type `T`.
template <class T>
constexpr std::size_t ComputeMaxIntSize(bool dec) noexcept {
  return ComputeIntSize<T>(
      dec,
      std::is_signed<T>::value ? std::numeric_limits<T>::min() : std::numeric_limits<T>::max());
}

/// Appends `value` to `buffer` as a decimal string.
template <class T, bool kDec>
constexpr StringView IntToString(
    std::array<char, ComputeMaxIntSize<T>(kDec)>& buffer HORUS_LIFETIME_BOUND, T value) noexcept {
  constexpr StringView kBuf{kDec ? "0123456789" : "0123456789abcdef"};
  constexpr T kBase{static_cast<T>(kBuf.size())};
  constexpr T kZero{0};

  char& first_ch{std::get<0>(buffer)};
  if (value == kZero) {
    first_ch = '0';
    return StringView{&first_ch, 1};
  }
  Span<char> const span{&first_ch, ComputeIntSize(kDec, value)};
  std::size_t offset{span.size() - 1};
  while (value != kZero) {
    span[offset] = kBuf[static_cast<std::size_t>(value % kBase)];
    value = static_cast<T>(value / kBase);
    --offset;
  }
  if (offset == 1) {
    span[0] = '-';
  }
  return StringView{span.data(), span.size()};
}

/// Appends `value` to `buffer` as a decimal or hexadecimal string.
template <bool kDec, class Sink, class T>
constexpr void StringifyIntegerTo(Sink& sink, T value) noexcept(IsNoexceptSink<Sink>()) {
  std::array<char, ComputeMaxIntSize<T>(kDec)> buffer{};
  sink.Append(IntToString<T, kDec>(buffer, value));
}

}  // namespace horus_internal

/// Appends `value` into `sink` as a decimal number.
template <
    class Sink,
    class T,
    std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(std::size_t)>* = nullptr>
constexpr void HorusStringify(Sink& sink, DecFormat<T> value) noexcept(IsNoexceptSink<Sink>()) {
  horus_internal::StringifyIntegerTo</*kDec=*/true>(sink, value);
}

/// Appends `value` into `sink` as a decimal number.
template <class Sink, class T, std::enable_if_t<std::is_enum<T>::value>* = nullptr>
constexpr void HorusStringify(Sink& sink, DecFormat<T> value) noexcept(IsNoexceptSink<Sink>()) {
  HorusStringify(sink, Dec(static_cast<std::underlying_type_t<T>>(value.value)));
}

/// Appends `value` into `sink` as a hexadecimal number.
template <
    class Sink,
    class T,
    std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(std::size_t)>* = nullptr>
constexpr void HorusStringify(Sink& sink, HexFormat<T> value) noexcept(IsNoexceptSink<Sink>()) {
  horus_internal::StringifyIntegerTo</*kDec=*/false>(sink, value);
}

/// Appends `value` into `sink` as a hexadecimal number.
template <class Sink, class T, std::enable_if_t<std::is_enum<T>::value>* = nullptr>
constexpr void HorusStringify(Sink& sink, HexFormat<T> value) noexcept(IsNoexceptSink<Sink>()) {
  HorusStringify(sink, Hex(static_cast<std::underlying_type_t<T>>(value.value)));
}

/// Appends `value` into `sink` as a decimal number.
template <
    class Sink,
    class T,
    std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(std::size_t)>* = nullptr>
constexpr void HorusStringify(Sink& sink, T value) noexcept(IsNoexceptSink<Sink>()) {
  horus_internal::StringifyIntegerTo</*kDec=*/true>(sink, value);
}

template <
    class T,
    std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= sizeof(std::size_t)>* = nullptr>
constexpr std::size_t HorusStringifySize(T value) noexcept {
  return horus_internal::ComputeIntSize(/*dec=*/true, value);
}

namespace horus_internal {

// #include <cmath>
// #include <cfloat>
// #include <cstdio>
//
// int main() {
//   fprintf(stderr, "%g\n", 0.);
//   fprintf(stderr, "%g\n", 100e100);
//   fprintf(stderr, "%g\n", -100e100);
//   fprintf(stderr, "%g\n", DBL_MAX);
//   fprintf(stderr, "%g\n", -DBL_MAX);
//   fprintf(stderr, "%g\n", DBL_MIN);
//   fprintf(stderr, "%g\n", DBL_EPSILON);
//   fprintf(stderr, "%g\n", DBL_TRUE_MIN);
//   fprintf(stderr, "%g\n", INFINITY);
//   fprintf(stderr, "%g\n", -INFINITY);
// }
//
// 0
// 1e+102
// -1e+102
// 1.79769e+308
// -1.79769e+308
// 2.22507e-308
// 2.22045e-16
// 4.94066e-324
// inf
// -inf

/// The maximum size of a `double` when formatted with `StringifyDoubleTo()`.
///
/// Does not include null byte required by `StringifyDoubleTo()`.
static constexpr std::size_t kDoubleMaxStrSize{13};

/// Writes a double `value` to the given `buffer`, then returns the number of written bytes.
std::size_t StringifyDoubleTo(std::array<char, kDoubleMaxStrSize + 1>& buffer,
                              double value) noexcept;

}  // namespace horus_internal

/// Appends `value` into `sink`.
template <class Sink>
void HorusStringify(Sink& sink, double value) noexcept(IsNoexceptSink<Sink>()) {
  std::array<char, horus_internal::kDoubleMaxStrSize + 1> buffer{};
  std::size_t const written{horus_internal::StringifyDoubleTo(buffer, value)};
  sink.Append(StringView{buffer.data(), written});
}

/// Appends `value` into `sink`.
template <class Sink>
void HorusStringify(Sink& sink, float value) noexcept(IsNoexceptSink<Sink>()) {
  HorusStringify(sink, static_cast<double>(value));
}

/// Appends `value` into `sink` as a pointer (e.g. 0xabcd).
template <class Sink>
constexpr void HorusStringify(Sink& sink, void const* value) noexcept(IsNoexceptSink<Sink>()) {
  if (value == nullptr) {
    sink.Append("null");
    return;
  }

  /// Size of a pointer in bytes on a 64-bit system.
  constexpr std::size_t k64BitPointerSize{8};
  static_assert(sizeof(void const*) == k64BitPointerSize, "logic below assumes 64-bit pointers");

  /// The maximum size of a pointer when formatted as an hexadecimal value.
  constexpr std::size_t kPointerMaxStrSize{std::numeric_limits<std::uintptr_t>::digits / 4};
  /// `kPointerStrSize`, plus the size necessary to prepend `0x` to it.
  ///
  /// Does not include null byte required by `StringifyPointerTo()`.
  constexpr std::size_t kPointerMaxStrSizeWithPrefix{kPointerMaxStrSize + 2};

  /// Mask used to extract the part of `bits` that we're converting to a hexadecimal digit.
  constexpr std::uintptr_t kHexMask{0b1111};
  /// The hexadecimal digits.
  constexpr StringView kHex{"0123456789abcdef"};

  std::uintptr_t bits{PointerCastToUintptr(value)};
  std::array<char, kPointerMaxStrSizeWithPrefix> buffer{};
  std::size_t index{buffer.size() - 1};
  while (bits != 0) {
    // NOLINTNEXTLINE(*-constant-array-index)
    buffer[index] = kHex[static_cast<std::size_t>(bits & kHexMask)];
    bits >>= 4U;
    --index;
  }
  buffer[index] = 'x';                                        // NOLINT(*-constant-array-index)
  buffer[index - 1] = '0';                                    // NOLINT(*-constant-array-index)
  sink.Append(StringView{&buffer[index - 1], buffer.end()});  // NOLINT(*-constant-array-index)
}

/// Appends `value` into `sink` as a pointer (e.g. 0xabcd).
template <class Sink, class T>
constexpr void HorusStringify(Sink& sink, T const* value) noexcept(IsNoexceptSink<Sink>()) {
  HorusStringify(sink, static_cast<void const*>(value));
}

/// Appends `value` into `sink` as a pointer (e.g. 0xabcd).
template <class Sink, class T>
constexpr void HorusStringify(Sink& sink, HexFormat<T*> value) noexcept(IsNoexceptSink<Sink>()) {
  HorusStringify(sink, value.value);
}

// MARK: Sink functions

/// Base recursive case of `StringifyTo()`.
template <class Sink>
constexpr void StringifyTo(Sink& sink) noexcept {
  static_cast<void>(sink);
}

/// Stringifies all the given values to the given `sink`.
template <class Sink, class T, class... Rest>
constexpr void StringifyTo(Sink& sink, T const& value, Rest const&... rest) noexcept(
    noexcept(HorusStringify(sink, value)) &&
    ConjunctionOf({noexcept(HorusStringify(sink, rest))...})) {
  HorusStringify(sink, value);  // NOLINT(*-decay)
  StringifyTo(sink, rest...);
}

}  // namespace horus

#endif  // HORUS_STRINGS_STRINGIFY_H_
