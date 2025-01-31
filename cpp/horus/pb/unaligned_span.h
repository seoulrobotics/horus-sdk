/// @file
///
/// The `UnalignedSpan` class.

#ifndef HORUS_PB_UNALIGNED_SPAN_H_
#define HORUS_PB_UNALIGNED_SPAN_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <type_traits>
#include <utility>

#include "horus/internal/attributes.h"
#include "horus/internal/pointer_arithmetic.h"
#include "horus/internal/pointer_cast.h"

namespace horus {

/// A read-only `Span` which allows referring to unaligned values of fixed size in a contiguous
/// chunk of memory.
template <class T>
class UnalignedSpan final {
 public:
  /// Type of the iterator returned by `begin()` and `end()`.
  class const_iterator final {
   public:
    /// Iterator difference type.
    using difference_type = std::ptrdiff_t;
    /// Iterator value type.
    using value_type = T;
    /// N/A, defined for `std::iterator_traits<>`.
    using pointer = T;
    /// N/A, defined for `std::iterator_traits<>`.
    using reference = T;
    /// Iterator category.
    using iterator_category = std::forward_iterator_tag;

    /// Constructs a `begin()` or `end()` iterator over the items in `span`.
    constexpr const_iterator(const UnalignedSpan& span, bool is_end) noexcept
        : ptr_{is_end ? horus_internal::PointerAdd(span.begin_, span.size_ * sizeof(T))
                      : span.begin_} {}

    /// Returns the current value.
    T operator*() const noexcept { return ReadValue(ptr_); }

    /// Advances to the next value (`++it`).
    constexpr const_iterator& operator++() noexcept {
      ptr_ = horus_internal::PointerAdd(ptr_, sizeof(T));
      return *this;
    }

    /// Returns a copy of the current iterator and advances to the next value (`it++`).
    constexpr const_iterator operator++(  // NOLINT(cert-dcl21-cpp)
        std::int32_t /* unused */) noexcept {
      const_iterator result{*this};
      ++*this;
      return result;
    }

    /// Returns whether `lhs` and `rhs` are equal.
    friend constexpr bool operator==(const_iterator lhs, const_iterator rhs) noexcept {
      return lhs.ptr_ == rhs.ptr_;
    }

    /// Returns whether `lhs` and `rhs` are not equal.
    friend constexpr bool operator!=(const_iterator lhs, const_iterator rhs) noexcept {
      return !(lhs == rhs);
    }

   private:
    /// Pointer to the current element.
    const char* ptr_;
  };

  /// Type of the referenced value.
  using value_type = T;

  /// Constructs an empty span.
  constexpr UnalignedSpan() noexcept : begin_{nullptr}, size_{0} {}

  /// Constructs a span pointing to the values starting at `begin` and ending at `end` (excluded).
  ///
  /// `begin` and `end` must either both be null, or both be non-null.
  UnalignedSpan(const T* begin HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND,
                const T* end HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept
      : begin_{horus_internal::UnsafePointerCast<const char>(begin)}, size_{end - begin} {
    assert((begin == nullptr) == (end == nullptr));
    assert(end >= begin);
  }

  /// Constructs a span pointing to the values starting at `begin` and ending at `end` (excluded).
  ///
  /// `begin` and `end` must either both be null, or both be non-null.
  constexpr UnalignedSpan(const char* begin HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND,
                          const char* end HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept
      : begin_{begin}, size_{(end - begin) / sizeof(T)} {
    assert((begin == nullptr) == (end == nullptr));
    assert(end >= begin && (end - begin) % sizeof(T) == 0);
  }

  /// Constructs a span pointing to the values starting at `begin` and ending at `end` (excluded).
  UnalignedSpan(const T* begin HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND, std::size_t size) noexcept
      : begin_{horus_internal::UnsafePointerCast<const char>(begin)}, size_{size} {
    assert(begin != nullptr || size == 0);
  }

  /// Constructs a span pointing to the values starting at `begin` and with size `size`.
  ///
  /// `begin` may be null iff `size` is `0`.
  constexpr UnalignedSpan(const char* begin HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND,
                          std::size_t size) noexcept
      : begin_{begin}, size_{size} {
    assert(begin != nullptr || size == 0);
  }

  /// Returns the number of items in the span.
  constexpr std::size_t size() const noexcept { return size_; }
  /// Returns whether `size() == 0`.
  constexpr bool empty() const noexcept { return size() == 0; }

  /// Returns a copy of the value at the given index.
  T operator[](std::size_t index) const noexcept {
    assert(index < size());
    return ReadValue(horus_internal::PointerAdd(begin_, index * sizeof(T)));
  }

  /// Returns a span starting at `begin() + offset`.
  constexpr UnalignedSpan subspan(std::size_t offset) const noexcept {
    assert(offset <= size());
    return UnalignedSpan{horus_internal::PointerAdd(begin_, offset * sizeof(T)), size() - offset};
  }
  /// Returns a span starting at `begin() + offset` and with size `count`.
  constexpr UnalignedSpan subspan(std::size_t offset, std::size_t count) const noexcept {
    assert(offset + count <= size());
    return UnalignedSpan{horus_internal::PointerAdd(begin_, offset * sizeof(T)),
                         horus_internal::PointerAdd(begin_, (offset + count) * sizeof(T))};
  }

  /// Returns a `const_iterator` pointing to the first item of the span.
  constexpr const_iterator begin() const noexcept {
    return const_iterator{*this, /*is_end=*/false};
  }
  /// Returns a `const_iterator` pointing past the last item of the span.
  constexpr const_iterator end() const noexcept { return const_iterator{*this, /*is_end=*/true}; }

 private:
  /// Whether the platform we're compiling form uses little-endian byte order.
  static constexpr bool kIsLittleEndian{BYTE_ORDER == LITTLE_ENDIAN};

  /// Reads and returns the value at `ptr`. `ptr` does not need to be aligned to `T`.
  static T ReadValue(const char* ptr) noexcept {
    T result{};
    static_cast<void>(std::memcpy(&result, ptr, sizeof(T)));
    if (!kIsLittleEndian && std::is_integral<T>::value && sizeof(T) > 1) {
      // Protobuf fixed values are encoded in little-endian byte order, so we byte-swap them in big
      // endian platforms: https://protobuf.dev/programming-guides/encoding/#cheat-sheet
      std::uint8_t* const result_bytes{horus_internal::UnsafePointerCast<std::uint8_t>(&result)};
      for (std::size_t i{0}; i < sizeof(T) / 2; ++i) {
        std::swap(*horus_internal::PointerAdd(result_bytes, i),
                  *horus_internal::PointerAdd(result_bytes, sizeof(T) - i - 1));
      }
    }
    return result;
  }

  /// A pointer to the first value. If null, `end_` must be null too.
  const char* begin_;
  /// The size of the span in values of type `T`.
  std::size_t size_;
};

}  // namespace horus

#endif  // HORUS_PB_UNALIGNED_SPAN_H_
