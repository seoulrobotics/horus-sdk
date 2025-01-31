/// @file
///
/// The `StringView` type.

#ifndef HORUS_TYPES_STRING_VIEW_H_
#define HORUS_TYPES_STRING_VIEW_H_

// We only include <iosfwd> in the header to avoid the heavy <ostream>.
//
// IWYU pragma: no_include <ostream>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iosfwd>  // IWYU pragma: keep
#include <stdexcept>
#include <string>

#include "horus/internal/attributes.h"
#include "horus/internal/pointer_arithmetic.h"
#include "horus/internal/pointer_cast.h"

namespace horus {

/// A minimal string view.
///
/// @see https://en.cppreference.com/w/cpp/string/basic_string_view
class StringView final {
 public:
  /// Type of `size()`.
  using size_type = std::string::size_type;
  /// Type of `begin()` and `end()`.
  using const_iterator = const char*;
  /// Same as `const_iterator`.
  using iterator = const_iterator;
  /// Same as `std::string::difference_type`.
  using difference_type = std::string::difference_type;

  /// @see https://en.cppreference.com/w/cpp/string/basic_string_view/npos
  static constexpr size_type npos{std::string::npos};

  /// Contructs an empty string view.
  constexpr StringView() noexcept : data_{nullptr}, size_{0} {}

  /// Constructs a string view starting at `begin` (inclusive) and ending at `end` (exclusive).
  constexpr StringView(const char* begin, const char* end) noexcept
      : data_{begin}, size_{horus_internal::PointerDist(begin, end)} {}

  /// Constructs a string view pointing to a `string` of the given `size`.
  constexpr StringView(const char* string, size_type size) noexcept : data_{string}, size_{size} {
    assert(string != nullptr || size == 0);
  }

  /// Constructs a string view pointing to a null-terminated `string`.
  constexpr StringView(const char* string) noexcept  // NOLINT(*-explicit-*)
      : data_{string}, size_{Strlen(string)} {}

  /// Constructs a string view pointing to a `string` of the given `size`.
  StringView(std::uint8_t const* string, size_type size) noexcept
      : StringView{horus_internal::UnsafePointerCast<const char>(string), size} {}

  /// Constructs a string view pointing to a `std::string`.
  StringView(  // NOLINT(*-explicit-*)
      const std::string& string HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept
      : data_{string.data()}, size_{string.size()} {}

  /// Cannot construct from `nullptr`.
  StringView(std::nullptr_t /* nullptr */) = delete;

  /// Converts the string to a `std::string`.
  operator std::string() const noexcept(false) {  // NOLINT(*-explicit-*)
    return {data(), size()};
  }

  /// Returns a pointer to the beginning of the string.
  constexpr const_iterator begin() const noexcept { return data_; }
  /// Returns a pointer to the first character past the string.
  constexpr const_iterator end() const noexcept { return horus_internal::PointerAdd(data_, size_); }

  /// Pointer to the beginning of the string; may be null iff `empty()`.
  constexpr const char* data() const noexcept { return data_; }
  /// Size of the string, excluding nul byte (if any).
  constexpr size_type size() const noexcept { return size_; }
  /// Whether the string is empty (i.e. its size is 0).
  constexpr bool empty() const noexcept { return size_ == 0; }

  /// Returns a reference to the first character in the view.
  constexpr const char& front() const noexcept { return operator[](0); }
  /// Returns a reference to the last character in the view.
  constexpr const char& back() const noexcept {
    assert(!empty());
    return operator[](size() - 1);
  }

  /// Returns the character at the given offset from `data()`.
  constexpr const char& operator[](size_type index) const noexcept {
    assert(index < size());
    return *horus_internal::PointerAdd(data_, index);
  }

  /// @see https://en.cppreference.com/w/cpp/string/basic_string_view/substr
  constexpr StringView substr(size_type pos = 0, size_type count = npos) const {
    if (pos > size()) {
      throw std::out_of_range{"substr() pos out of range"};
    }
    // `operator[]` checks that `index < size()`, but here `pos == size()` is valid.
    return StringView{horus_internal::PointerAdd(data_, pos),
                      horus_internal::PointerAdd(data_, pos + Min(count, size() - pos))};
  }

  /// @see https://en.cppreference.com/w/cpp/string/basic_string_view/compare
  constexpr std::int32_t compare(StringView other) const noexcept {
    size_type const size{Min(size_, other.size_)};
    for (size_type i{0}; i < size; ++i) {
      if (operator[](i) < other[i]) {
        return -1;
      }
      if (operator[](i) > other[i]) {
        return 1;
      }
    }
    if (size_ < other.size_) {
      return -1;
    }
    if (size_ > other.size_) {
      return 1;
    }
    return 0;
  }

  /// Returns the first index of `needle` in the string, or `npos` if it cannot be found.
  ///
  /// @see https://en.cppreference.com/w/cpp/string/basic_string_view/find
  constexpr size_type find(char needle) const noexcept {
    for (size_type i{0}; i < size(); ++i) {
      if (operator[](i) == needle) {
        return i;
      }
    }
    return npos;
  }

  /// Returns the first index of `needle` in the string, or `npos` if it cannot be found.
  ///
  /// @see https://en.cppreference.com/w/cpp/string/basic_string_view/find
  constexpr size_type find(StringView needle) const noexcept {
    if (needle.size() > size()) {
      return npos;
    }
    size_type const len{size() - needle.size()};
    for (size_type i{0}; i <= len; ++i) {
      bool found{true};
      for (size_type j{0}; j < needle.size(); ++j) {
        if (operator[](i + j) != needle[j]) {
          found = false;
          break;
        }
      }
      if (found) {
        return i;
      }
    }
    return npos;
  }

  /// Returns the last index of `needle` in the string, or `npos` if it cannot be found.
  ///
  /// @see https://en.cppreference.com/w/cpp/string/basic_string_view/find
  constexpr size_type rfind(char needle) const noexcept { return rfind(StringView{&needle, 1}); }

  /// Returns the last index of `needle` in the string, or `npos` if it cannot be found.
  ///
  /// @see https://en.cppreference.com/w/cpp/string/basic_string_view/find
  constexpr size_type rfind(StringView needle) const noexcept {
    if (needle.size() > size()) {
      return npos;
    }
    for (size_type i{size() - needle.size() + 1}; i > 0; --i) {
      size_type const ofs{i - 1};
      bool found{true};
      for (size_type j{0}; j < needle.size(); ++j) {
        if (operator[](ofs + j) != needle[j]) {
          found = false;
          break;
        }
      }
      if (found) {
        return ofs;
      }
    }
    return npos;
  }

  /// Returns whether `needle` is in the string.
  ///
  /// https://en.cppreference.com/w/cpp/string/basic_string_view/contains
  constexpr bool contains(char needle) const noexcept { return find(needle) != npos; }

  /// Returns whether `needle` is in the string.
  ///
  /// https://en.cppreference.com/w/cpp/string/basic_string_view/contains
  constexpr bool contains(StringView needle) const noexcept { return find(needle) != npos; }

 private:
  /// `constexpr` implementation of `std::strlen()`.
  static constexpr size_type Strlen(const char* string) noexcept {
    assert(string != nullptr);
    size_type len{0};
    while (*horus_internal::PointerAdd(string, len) != '\0') {
      ++len;
    }
    return len;
  }

  /// Returns the minimum value between `lhs` and `rhs`.
  ///
  /// We don't use `std::min()` to avoid a fairly large include.
  static constexpr size_type Min(size_type lhs, size_type rhs) noexcept {
    return lhs < rhs ? lhs : rhs;
  }

  /// @see data()
  const char* data_;
  /// @see size()
  size_type size_;
};

/// Lexicographic `==`.
constexpr bool operator==(StringView lhs, StringView rhs) noexcept { return lhs.compare(rhs) == 0; }
/// Lexicographic `!=`.
constexpr bool operator!=(StringView lhs, StringView rhs) noexcept { return lhs.compare(rhs) != 0; }
/// Lexicographic `<`.
constexpr bool operator<(StringView lhs, StringView rhs) noexcept { return lhs.compare(rhs) == -1; }
/// Lexicographic `<=`.
constexpr bool operator<=(StringView lhs, StringView rhs) noexcept { return lhs.compare(rhs) <= 0; }
/// Lexicographic `>`.
constexpr bool operator>(StringView lhs, StringView rhs) noexcept { return lhs.compare(rhs) == 1; }
/// Lexicographic `>=`.
constexpr bool operator>=(StringView lhs, StringView rhs) noexcept { return lhs.compare(rhs) >= 0; }

/// Prints `string` to the given output stream.
std::ostream& operator<<(std::ostream& out, StringView string);

/// Returns whether `string` starts with `prefix`.
constexpr bool StartsWith(StringView string, StringView prefix) noexcept {
  return string.substr(0, prefix.size()) == prefix;
}
/// Returns whether `string` ends with `suffix`.
constexpr bool EndsWith(StringView string, StringView suffix) noexcept {
  return (string.size() >= suffix.size()) &&
         (string.substr(string.size() - suffix.size()) == suffix);
}

/// Returns `string` with `prefix` removed if `string` starts with `prefix`.
// NOLINTNEXTLINE(*-exception-escape): substr() won't fail after checking `StartsWith()`
constexpr StringView StripPrefix(StringView string, StringView prefix) noexcept {
  return StartsWith(string, prefix) ? string.substr(prefix.size()) : string;
}
/// Returns `string` with `suffix` removed if `string` ends with `suffix`.
// NOLINTNEXTLINE(*-exception-escape): substr() won't fail after checking `EndsWith()`
constexpr StringView StripSuffix(StringView string, StringView suffix) noexcept {
  return EndsWith(string, suffix) ? string.substr(0, string.size() - suffix.size()) : string;
}

}  // namespace horus

template <>
struct std::hash<horus::StringView> {
  /// Returns the hash of the given string.
  std::size_t operator()(horus::StringView string) const noexcept;
};

#endif  // HORUS_TYPES_STRING_VIEW_H_
