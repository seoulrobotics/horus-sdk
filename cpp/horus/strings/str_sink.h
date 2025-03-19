/// @file
///
/// The `StrCat()` function.

#ifndef HORUS_STRINGS_STR_SINK_H_
#define HORUS_STRINGS_STR_SINK_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "horus/internal/type_traits.h"
#include "horus/types/span.h"
#include "horus/types/string_view.h"

namespace horus {

/// Returns whether the given `HorusStringify()` can be used to append values in a `noexcept`
/// manner.
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
                              const std::string& value) noexcept(IsNoexceptSink<Sink>()) {
  sink.Append(StringView{value});
}

/// Appends `value` into `sink`.
template <class Sink>
constexpr void HorusStringify(Sink& sink, const char* value) noexcept(IsNoexceptSink<Sink>()) {
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

/// Computes the decimal or hexadecimal size of an integer of type `T`.
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
  return ComputeIntSize<T>(dec, std::is_signed<T>::value ? std::numeric_limits<T>::min()
                                                         : std::numeric_limits<T>::max());
}

/// Appends `value` to `buffer` as a decimal string.
template <class T, bool kDec>
constexpr StringView IntToString(std::array<char, ComputeMaxIntSize<T>(kDec)>& buffer,
                                 T value) noexcept {
  constexpr StringView kBuf{kDec ? "0123456789" : "0123456789abcdef"};
  constexpr T kBase{static_cast<T>(kBuf.size())};
  constexpr T kZero{0};

  char& first_ch{std::get<0>(buffer)};
  if (value == kZero) {
    first_ch = '0';
    return StringView{&first_ch, 1};
  }
  const Span<char> span{&first_ch, ComputeIntSize(kDec, value)};
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
template <class Sink, class T,
          std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= 8>* = nullptr>
constexpr void HorusStringify(Sink& sink, DecFormat<T> value) noexcept(IsNoexceptSink<Sink>()) {
  horus_internal::StringifyIntegerTo</*kDec=*/true>(sink, value);
}

/// Appends `value` into `sink` as a decimal number.
template <class Sink, class T, std::enable_if_t<std::is_enum<T>::value>* = nullptr>
constexpr void HorusStringify(Sink& sink, DecFormat<T> value) noexcept(IsNoexceptSink<Sink>()) {
  HorusStringify(sink, Dec(static_cast<std::underlying_type_t<T>>(value.value)));
}

/// Appends `value` into `sink` as a hexadecimal number.
template <class Sink, class T,
          std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= 8>* = nullptr>
constexpr void HorusStringify(Sink& sink, HexFormat<T> value) noexcept(IsNoexceptSink<Sink>()) {
  horus_internal::StringifyIntegerTo</*kDec=*/false>(sink, value);
}

/// Appends `value` into `sink` as a hexadecimal number.
template <class Sink, class T, std::enable_if_t<std::is_enum<T>::value>* = nullptr>
constexpr void HorusStringify(Sink& sink, HexFormat<T> value) noexcept(IsNoexceptSink<Sink>()) {
  HorusStringify(sink, Hex(static_cast<std::underlying_type_t<T>>(value.value)));
}

/// Appends `value` into `sink` as a decimal number.
template <class Sink, class T,
          std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= 8>* = nullptr>
constexpr void HorusStringify(Sink& sink, T value) noexcept(IsNoexceptSink<Sink>()) {
  horus_internal::StringifyIntegerTo</*kDec=*/true>(sink, value);
}

template <class T, std::enable_if_t<std::is_integral<T>::value && sizeof(T) <= 8>* = nullptr>
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

namespace horus_internal {

/// The maximum size of a pointer when formatted as an hexadecimal value.
constexpr std::size_t kPointerMaxStrSize{std::numeric_limits<std::uintptr_t>::digits / 4};
/// `kPointerStrSize`, plus the size necessary to prepend `0x` to it.
///
/// Does not include null byte required by `StringifyPointerTo()`.
constexpr std::size_t kPointerMaxStrSizeWithPrefix{kPointerMaxStrSize + 2};

/// Writes a pointer value to the given `buffer`, then returns the number of written bytes.
std::size_t StringifyPointerTo(std::array<char, kPointerMaxStrSizeWithPrefix + 1>& buffer,
                               const void* value) noexcept;

}  // namespace horus_internal

/// Appends `value` into `sink` as a pointer (e.g. 0xABCD).
template <class Sink>
void HorusStringify(Sink& sink, const void* value) noexcept(IsNoexceptSink<Sink>()) {
  if (value == nullptr) {
    sink.Append("null");
    return;
  }

  // Autosar forbids casts from pointers to integers, so we don't have any other choice but to rely
  // on existing formatting helpers.

  std::array<char, horus_internal::kPointerMaxStrSizeWithPrefix + 1> buffer{};
  std::size_t const written{horus_internal::StringifyPointerTo(buffer, value)};
  sink.Append(StringView{buffer.data(), written});
}

/// Appends `value` into `sink` as a pointer (e.g. 0xABCD).
template <class Sink, class T, std::enable_if_t<sizeof(const T*) == sizeof(void*)>* = nullptr>
constexpr void HorusStringify(Sink& sink, const T* value) noexcept(IsNoexceptSink<Sink>()) {
  HorusStringify(sink, static_cast<const void*>(value));
}

/// Appends `value` into `sink` as a hexadecimal number.
template <class Sink, class T, std::enable_if_t<std::is_pointer<T>::value>* = nullptr>
constexpr void HorusStringify(Sink& sink, HexFormat<T> value) noexcept(IsNoexceptSink<Sink>()) {
  HorusStringify(sink, value.value);
}

// MARK: Sink functions

/// Base recursive case of `StrAppendToSink()`.
template <class Sink>
constexpr void StrAppendToSink(Sink& sink) noexcept {
  static_cast<void>(sink);
}

/// Stringifies all the given values to the given `sink`.
template <class Sink, class T, class... Rest>
constexpr void StrAppendToSink(Sink& sink, const T& value, const Rest&... rest) noexcept(
    noexcept(HorusStringify(sink, value)) &&
    horus_internal::Conjunction<
        std::integral_constant<bool, noexcept(HorusStringify(sink, rest))>...>::value) {
  HorusStringify(sink, value);  // NOLINT(*-decay)
  StrAppendToSink(sink, rest...);
}

}  // namespace horus

#endif  // HORUS_STRINGS_STR_SINK_H_
