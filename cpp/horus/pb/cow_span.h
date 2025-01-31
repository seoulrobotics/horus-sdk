/// @file
///
/// The `CowSpan` class.

#ifndef HORUS_PB_COW_SPAN_H_
#define HORUS_PB_COW_SPAN_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <protozero/data_view.hpp>
#include <protozero/iterators.hpp>
#include <protozero/pbf_reader.hpp>
#include <protozero/pbf_writer.hpp>
#include <type_traits>
#include <utility>
#include <vector>

#include "horus/internal/attributes.h"
#include "horus/internal/pointer_arithmetic.h"
#include "horus/internal/pointer_cast.h"
#include "horus/pb/buffer.h"
#include "horus/pb/serialize.h"
#include "horus/pb/unaligned_span.h"
#include "horus/types/one_of.h"
#include "horus/types/span.h"
#include "horus/types/string_view.h"

namespace horus {

/// A span which refers to some borrowed or owned data.
///
/// Note that copying a `CowSpan` internally storing an owned vector (because it was mutated)
/// will make a copy of the vector.
template <class T>
class CowSpan final {
  /// Type of the inner `data_`.
  using Data = OneOf<PbView, std::vector<T>>;

 public:
  /// Type of the iterator returned by `begin()` and `end()`.
  using const_iterator = typename UnalignedSpan<T>::const_iterator;
  /// Type of the items yielded by `const_iterator`.
  using value_type = const T;

  /// Returns a `CowSpan` container which refers to the data in `span`.
  static CowSpan<T> Borrowed(
      horus::Span<const T> span HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept {
    return CowSpan<T>{span};
  }

  /// Creates an empty span.
  CowSpan() noexcept : data_{InPlaceType<PbView>, PbBuffer{}} {}

  /// Creates a span of values taken from `values`.
  ///
  /// @throws std::bad_alloc If the resulting vector cannot be allocated.
  CowSpan(std::initializer_list<T> values) noexcept(false)
      : data_{InPlaceType<std::vector<T>>, values.begin(), values.end()} {}

  /// Creates a span which wraps a shared `view`.
  explicit CowSpan(PbView&& view) noexcept : data_{InPlaceType<PbView>, std::move(view)} {}

  /// Creates a span with its own data.
  explicit CowSpan(std::vector<T>&& data) noexcept
      : data_{InPlaceType<std::vector<T>>, std::move(data)} {}

  /// Returns a `Span` over the elements.
  UnalignedSpan<T> Span() const noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND;

  /// Returns a mutable reference to the vector of elements.
  ///
  /// @throws std::bad_alloc If the contents of the span had to be copied and the allocation failed.
  std::vector<T>& Vector() noexcept(false) HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND;

  /// Returns a `const_iterator` pointing to the first item of the container.
  const_iterator begin() const noexcept { return Span().begin(); }
  /// Returns a `const_iterator` pointing past the last item of the container.
  const_iterator end() const noexcept { return Span().end(); }

  /// Pushes a value at the end of the collection and returns a reference to it.
  T& Add(T value) & noexcept {
    std::vector<T>& vector{Vector()};
    vector.push_back(value);
    return vector.back();
  }

  /// Emplaces a new value and returns `*this`.
  CowSpan&& Add(T value) && noexcept {
    static_cast<void>(Add(value));
    return std::move(*this);
  }

 private:
  /// Constructs a `CowSpan` which refers to a borrowed span.
  explicit CowSpan(horus::Span<const T> span) noexcept
      : data_{InPlaceType<PbView>,
              PbBuffer::Borrowed(
                  StringView{horus_internal::UnsafePointerCast<const char>(span.begin()),
                             horus_internal::UnsafePointerCast<const char>(span.end())})} {}

  /// The actual data.
  Data data_;
};

template <class T>
UnalignedSpan<T> CowSpan<T>::Span() const noexcept {
  switch (data_.Tag()) {
    case Data::template kTagFor<std::vector<T>>: {
      const std::vector<T>& vector{*data_.template TryAs<std::vector<T>>()};
      return UnalignedSpan<T>{vector.data(), vector.size()};
    }
    case Data::template kTagFor<PbView>:
    default: {
      const PbView& view{*data_.template TryAs<PbView>()};
      assert(view.Str().size() % sizeof(T) == 0);
      return UnalignedSpan<T>{view.Str().data(), view.Str().size() / sizeof(T)};
    }
  }
}

template <class T>
std::vector<T>& CowSpan<T>::Vector() noexcept(false) {
  static_assert(std::is_nothrow_constructible<T>::value, "");

  switch (data_.Tag()) {
    case Data::template kTagFor<std::vector<T>>: {
      return *data_.template TryAs<std::vector<T>>();
    }
    case Data::template kTagFor<PbView>:
    default: {
      const PbView view{std::move(*data_.template TryAs<PbView>())};
      const UnalignedSpan<T> span{view.Str().data(), view.Str().size() / sizeof(T)};
      std::vector<T> result{span.begin(), span.end()};
      return data_.template Emplace<std::vector<T>>(std::move(result));
    }
  }
}

namespace horus_internal {

/// `add_packed_*` and `get_packed_*` functions for `T`.
template <class T>
class FixedPackedFns;

template <>
class FixedPackedFns<float> {
 public:
  constexpr static auto kAddPacked =
      &protozero::pbf_writer::add_packed_float<typename CowSpan<float>::const_iterator>;
  constexpr static auto kGetPacked = &protozero::pbf_reader::get_packed_float;
};
template <>
class FixedPackedFns<double> {
 public:
  constexpr static auto kAddPacked =
      &protozero::pbf_writer::add_packed_double<typename CowSpan<double>::const_iterator>;
  constexpr static auto kGetPacked = &protozero::pbf_reader::get_packed_double;
};
template <>
class FixedPackedFns<std::uint32_t> {
 public:
  constexpr static auto kAddPacked =
      &protozero::pbf_writer::add_packed_fixed32<typename CowSpan<std::uint32_t>::const_iterator>;
  constexpr static auto kGetPacked = &protozero::pbf_reader::get_packed_fixed32;
};
template <>
class FixedPackedFns<std::uint64_t> {
 public:
  constexpr static auto kAddPacked =
      &protozero::pbf_writer::add_packed_fixed64<typename CowSpan<std::uint64_t>::const_iterator>;
  constexpr static auto kGetPacked = &protozero::pbf_reader::get_packed_fixed64;
};
template <>
class FixedPackedFns<std::int32_t> {
 public:
  constexpr static auto kAddPacked =
      &protozero::pbf_writer::add_packed_sfixed32<typename CowSpan<std::int32_t>::const_iterator>;
  constexpr static auto kGetPacked = &protozero::pbf_reader::get_packed_sfixed32;
};
template <>
class FixedPackedFns<std::int64_t> {
 public:
  constexpr static auto kAddPacked =
      &protozero::pbf_writer::add_packed_sfixed64<typename CowSpan<std::int64_t>::const_iterator>;
  constexpr static auto kGetPacked = &protozero::pbf_reader::get_packed_sfixed64;
};

}  // namespace horus_internal

template <class T>
class PbTraits<CowSpan<T>, PbDeserFlags::kFixed> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, const CowSpan<T>& value) {
    static constexpr auto kAdd = horus_internal::FixedPackedFns<T>::kAddPacked;
    (writer.Writer().*kAdd)(tag, value.begin(), value.end());
  }

  /// Deserializes a `CowSpan<T>` from `reader`.
  static CowSpan<T> Deserialize(PbReader& reader) {
    static constexpr auto kGet = horus_internal::FixedPackedFns<T>::kGetPacked;
    // protozero doesn't give us direct access to the begin/end pointers so we must recover them
    // from the exposed APIs...
    std::size_t const size{(reader.Reader().*kGet)().size()};  // `kGet()` will skip the span.
    const char* const span_end{reader.Reader().data().data()};
    const char* const span_start{horus_internal::PointerSub(span_end, sizeof(T) * size)};
    const char* const buffer_start{reader.Buffer().Str().data()};
    std::size_t const buffer_offset{static_cast<std::size_t>(span_start - buffer_start)};

    return CowSpan<T>{PbBuffer{reader.Buffer()}.View(buffer_offset, size * sizeof(T))};
  }
};

}  // namespace horus

#endif  // HORUS_PB_COW_SPAN_H_
