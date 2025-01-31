/// @file
///
/// The `Span` class.

#ifndef HORUS_TYPES_SPAN_H_
#define HORUS_TYPES_SPAN_H_

#include <array>
#include <cassert>
#include <cstddef>
#include <type_traits>

#include "horus/internal/attributes.h"
#include "horus/internal/pointer_arithmetic.h"

namespace horus {

/// A minimal span.
///
/// @see https://en.cppreference.com/w/cpp/container/span
template <class T>
class Span final {
 public:
  /// Type of the iterator returned by `begin()` and `end()`.
  using iterator = const T*;
  /// Type of the referenced value.
  using value_type = T;

  /// Constructs an empty span.
  constexpr Span() noexcept = default;
  /// Constructs a span from `begin` (included) to `end` (excluded).
  constexpr Span(T* begin HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND,
                 T* end HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept
      : begin_{begin}, end_{end} {
    assert(begin <= end);
  }

  /// Constructs a span starting at `begin` and of size `size`.
  constexpr Span(T* begin HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND, std::size_t size) noexcept
      : begin_{begin}, end_{horus_internal::PointerAdd(begin, size)} {
    assert(begin != nullptr || size == 0);
  }

  /// Constructs a span from a `std::vector<T>`-like container.
  template <class Container,
            std::enable_if_t<std::is_same<typename Container::value_type, T>::value &&
                             !std::is_same<Container, Span>::value>* = nullptr>
  constexpr Span(Container& container  // NOLINT(*-explicit-*)
                     HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept
      : Span{container.data(), container.size()} {}

  /// Constructs a span from a `std::vector<T>`-like container.
  template <class Container,
            std::enable_if_t<std::is_same<const typename Container::value_type, T>::value &&
                             !std::is_same<Container, Span>::value>* = nullptr>
  constexpr Span(const Container& container  // NOLINT(*-explicit-*)
                     HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept
      : Span{container.data(), container.size()} {}

  /// Constructs a span from a `std::array<T, N>`.
  ///
  /// This is separate from the above constructor because it allows `constexpr` construction.
  template <std::size_t N>
  constexpr Span(std::array<T, N>& array  // NOLINT(*-explicit-*)
                     HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept
      : Span{&std::get<0>(array), array.size()} {}

  /// Constructs a span from a `std::array<T, N>`.
  ///
  /// This is separate from the above constructor because it allows `constexpr` construction.
  template <std::size_t N, std::enable_if_t<(N > 0) && std::is_const<T>::value>* = nullptr>
  constexpr Span(const std::array<std::remove_const_t<T>, N>& array  // NOLINT(*-explicit-*)
                     HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept
      : Span{&std::get<0>(array), array.size()} {}

  /// Returns a pointer to the first element of the span; may be null iff `empty()`.
  constexpr T* begin() const noexcept { return begin_; }
  /// Returns a pointer past the last element of the span; may be null iff `empty()`.
  constexpr T* end() const noexcept { return end_; }

  /// Returns a pointer to the beginning of the span; may be null iff `empty()`.
  constexpr T* data() const noexcept { return begin_; }

  /// Returns the size of the span.
  constexpr std::size_t size() const noexcept { return horus_internal::PointerDist(begin_, end_); }
  /// Returns whether the span is empty.
  constexpr bool empty() const noexcept { return begin_ == end_; }

  /// Returns a reference to the first value in the span.
  constexpr T& front() const noexcept { return operator[](0); }
  /// Returns a reference to the last value in the span.
  constexpr T& back() const noexcept {
    assert(!empty());
    return operator[](size() - 1);
  }

  /// Returns a span starting at `begin() + offset`.
  constexpr Span subspan(std::size_t offset) const noexcept {
    assert(offset <= size());
    return Span{horus_internal::PointerAdd(begin_, offset), end_};
  }
  /// Returns a span starting at `begin() + offset` and with size `count`.
  constexpr Span subspan(std::size_t offset, std::size_t count) const noexcept {
    assert(offset + count <= size());
    return Span{horus_internal::PointerAdd(begin_, offset),
                horus_internal::PointerAdd(begin_, offset + count)};
  }

  /// Returns a span which contains the first `count` items.
  constexpr Span first(std::size_t count) const noexcept {
    assert(count <= size());
    return Span{begin_, horus_internal::PointerAdd(begin_, count)};
  }
  /// Returns a span which contains the last `count` items.
  constexpr Span last(std::size_t count) const noexcept {
    assert(count <= size());
    return Span{horus_internal::PointerSub(end_, count), end_};
  }

  /// Returns a reference to the `index`th value in the span.
  constexpr T& operator[](std::size_t index) const noexcept {
    assert(index <= size());
    return *horus_internal::PointerAdd(begin_, index);
  }

 private:
  /// @see begin()
  T* begin_{nullptr};
  /// @see end()
  T* end_{nullptr};
};

}  // namespace horus

#endif  // HORUS_TYPES_SPAN_H_
