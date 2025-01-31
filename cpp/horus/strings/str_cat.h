/// @file
///
/// The `StrCat()` function.

#ifndef HORUS_STRINGS_STR_CAT_H_
#define HORUS_STRINGS_STR_CAT_H_

#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>

#include "horus/internal/attributes.h"
#include "horus/strings/str_sink.h"
#include "horus/types/string_view.h"

namespace horus {
namespace horus_internal {

/// A `HorusStringify()` sink which only counts the size of all the appended strings.
class ExpectedSizeSink final {
 public:
  /// Returns the size measured so far.
  constexpr std::size_t Size() const noexcept { return size_; }

  /// Adds `string.size()` to the measure size.
  constexpr void Append(StringView string) noexcept { size_ += string.size(); }

 private:
  /// The size measured so far.
  std::size_t size_{0};
};

/// Defines `value` to be true if `HorusStringifySize(const T&)` is defined and evaluates to
/// `std::size_t`.
template <class T, class Enable = void>
struct HasStringifySize : std::false_type {};
template <class T>
struct HasStringifySize<
    T, std::enable_if_t<std::is_same<decltype(HorusStringifySize(std::declval<const T&>())),
                                     std::size_t>::value>>
    : std::integral_constant<bool, noexcept(HorusStringifySize(std::declval<const T&>()))> {};

/// Returns the expected size of `value` when formatting to a string using `HorusStringifySize()`.
template <class T, std::enable_if_t<HasStringifySize<T>::value>* = nullptr>
constexpr std::size_t ComputeExpectedSize(const T& value) noexcept {
  return HorusStringifySize(value);
}

/// Returns the expected size of `value` when formatting to a string using `HorusStringify()`.
template <class T, std::enable_if_t<!HasStringifySize<T>::value &&
                                    noexcept(HorusStringify(std::declval<ExpectedSizeSink&>(),
                                                            std::declval<const T&>()))>* = nullptr>
constexpr std::size_t ComputeExpectedSize(const T& value) noexcept {
  ExpectedSizeSink expected_size_sink;
  HorusStringify(expected_size_sink, value);  // NOLINT(*-decay)
  return expected_size_sink.Size();
}

/// Cannot compute expected size of `T`.
template <class T, std::enable_if_t<!HasStringifySize<T>::value &&
                                    !noexcept(HorusStringify(std::declval<ExpectedSizeSink&>(),
                                                             std::declval<const T&>()))>* = nullptr>
constexpr std::size_t ComputeExpectedSize(const T& /* value */) noexcept {
  return std::string::npos;
}

/// Returns the expected size of `values` when concatenated.
constexpr std::size_t ComputeExpectedSizeSum(std::size_t sum) noexcept { return sum; }

/// Returns the expected size of `values` when concatenated.
template <class T, class... Rest>
HORUS_SDK_NO_SANITIZE("unsigned-integer-overflow")
constexpr std::size_t
    ComputeExpectedSizeSum(std::size_t sum, const T& value, const Rest&... rest) noexcept {
  std::size_t const value_expected_size{ComputeExpectedSize(value)};
  std::size_t const summed{sum + value_expected_size};
  if (summed < sum) {
    // We wrapped around either because `value_expected_size` is -1 or because we computed a value
    // that's too large. Realistically only the former should happen, but we must check for overflow
    // anyway so we might as well do both at the same time.
    return std::string::npos;
  }
  return ComputeExpectedSizeSum(summed, rest...);
}

/// A `HorusStringify()` sink which appends all strings to a `std::string`.
class StringSink final {
 public:
  /// Constructs a `StringSink`.
  explicit StringSink(std::string& output HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept
      : output_{output} {}

  /// Appends the given string to the output string.
  ///
  /// @throws std::bad_alloc
  void Append(StringView string) const noexcept(false) {
    output_.append(string.data(), string.size());
  }

 private:
  /// The output string.
  std::string& output_;
};

}  // namespace horus_internal

/// Computes the expected size of concatenating all the given values into a string using `StrCat()`.
///
/// If any value's size cannot be computed ahead of time (e.g. if it needs to allocate to do so),
/// returns `std::string::npos`.
template <class... Ts>
constexpr std::size_t StrExpectedSize(const Ts&... values) noexcept {
  return horus_internal::ComputeExpectedSizeSum(0, values...);
}

/// Appends all the given values into a `std::string`.
///
/// @throws std::bad_alloc
template <class... Ts>
void StrAppendTo(std::string& string, const Ts&... values) noexcept(false) {
  std::size_t const expected_size{StrExpectedSize(values...)};
  if (expected_size != std::string::npos) {
    string.reserve(string.size() + expected_size);
  }
  horus_internal::StringSink sink{string};
  StrAppendToSink(sink, values...);
}

/// Concatenates all the given values into a `std::string`.
///
/// @throws std::bad_alloc
template <class... Ts>
std::string StrCat(const Ts&... values) noexcept(false) {
  std::string result;
  StrAppendTo(result, values...);
  return result;
}

}  // namespace horus

#endif  // HORUS_STRINGS_STR_CAT_H_
