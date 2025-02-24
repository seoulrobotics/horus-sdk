/// @file
///
/// The `CowRepeated` class.

#ifndef HORUS_PB_COW_REPEATED_H_
#define HORUS_PB_COW_REPEATED_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <protozero/exception.hpp>
#include <protozero/iterators.hpp>
#include <protozero/pbf_reader.hpp>
#include <protozero/pbf_writer.hpp>
#include <protozero/varint.hpp>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include "horus/internal/attributes.h"
#include "horus/internal/pointer_arithmetic.h"
#include "horus/internal/pointer_cast.h"
#include "horus/internal/type_traits.h"
#include "horus/pb/buffer.h"
#include "horus/pb/cow.h"
#include "horus/pb/repeated_iterator.h"
#include "horus/pb/serialize.h"
#include "horus/pb/types.h"
#include "horus/types/in_place.h"
#include "horus/types/one_of.h"
#include "horus/types/string_view.h"

namespace horus {
namespace horus_internal {

/// A `PbView` and a `PbTag`.
struct PbViewAndTag {
  /// Constructs the struct.
  PbViewAndTag(PbView&& view_p, PbTag tag_p) noexcept : view{std::move(view_p)}, tag{tag_p} {}

  /// The view of the data.
  PbView view;
  /// The tag of the field of interest.
  PbTag tag{};
};

/// A wrapper around a `std::vector<std::uint8_t>` which acts like a `std::vector<bool>` (with no
/// bitset-like semantics).
class VectorBool final {
  static_assert(sizeof(bool) == sizeof(std::uint8_t), "");
  static_assert(alignof(bool) == alignof(std::uint8_t), "");

 public:
  /// Constructs an empty vector.
  VectorBool() noexcept = default;

  /// Constructs a vector with the given values.
  VectorBool(std::initializer_list<bool> values) noexcept(false) {
    inner_.reserve(values.size());
    for (const bool value : values) {
      inner_.push_back(ToValue(value));
    }
  }

  /// Constructs a vector from a range of values.
  VectorBool(const bool* begin, const bool* end) noexcept(false) {
    inner_.reserve(static_cast<std::size_t>(end - begin));
    while (begin != end) {
      inner_.push_back(ToValue(*begin));
      begin = PointerAdd(begin, 1);
    }
  }

  /// Returns a pointer to the first bool.
  const bool* data() const noexcept { return UnsafePointerCast<const bool>(inner_.data()); }
  /// Returns the size of the vector.
  std::size_t size() const noexcept { return inner_.size(); }
  /// Returns whether the vector is empty.
  bool empty() const noexcept { return inner_.empty(); }

 private:
  /// Converts bool -> std::uint8_t.
  constexpr static std::uint8_t ToValue(bool value) noexcept { return value ? 1 : 0; }
  /// Converts std::uint8_t -> bool.
  constexpr static bool ToBool(std::uint8_t value) noexcept { return value != 0; }

  /// The inner vector.
  std::vector<std::uint8_t> inner_;
};

}  // namespace horus_internal

/// A list of repeated items of type `T`.
///
/// If the list references a `PbView`, attempting to mutate it will replace its contents with a
/// `PbBuffer` it owns.
///
/// Note that copying a `CowRepeated` internally storing an owned vector (because it was mutated)
/// will make a copy of the vector.
template <class T, bool kPacked = std::is_arithmetic<T>::value || std::is_enum<T>::value>
class CowRepeated;

// MARK: CowRepeated<T, kPacked = true>

template <class T>
class CowRepeated<T, /*kPacked=*/true> final {
  /// Type of the `std::vector` stored in `Data`. This is `std::vector<T>`, except for `bool` where
  /// this is `std::vector<std::uint8_t>`.
  using VectorT =
      std::conditional_t<!std::is_same<T, bool>::value, std::vector<T>, horus_internal::VectorBool>;
  /// Type of the inner `data_`.
  using Data = OneOf<PbView, VectorT>;

 public:
  /// Type of the iterator returned by `begin()` and `end()`.
  class const_iterator final {
    /// Type of the inner `data_`.
    using Data = OneOf<protozero::const_varint_iterator<T>, const T*>;

   public:
    /// Iterator difference type.
    using difference_type = std::ptrdiff_t;
    /// Iterator value type.
    using value_type = T;

    /// Constructs a `begin()` or `end()` iterator over the items in `container`.
    const_iterator(const CowRepeated& container, bool is_end) noexcept
        : data_{DataFrom(container, is_end)} {}

    /// Returns the current value.
    T operator*() const noexcept;

    /// Advances to the next value (`++it`).
    const_iterator& operator++() noexcept;

    /// Returns a copy of the current iterator and advances to the next value (`it++`).
    const_iterator operator++(std::int32_t /* unused */) noexcept {  // NOLINT(cert-dcl21-cpp)
      const_iterator result{*this};
      ++*this;
      return result;
    }

    /// Returns whether `lhs` and `rhs` are equal.
    friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) noexcept {
      if (lhs.data_.Tag() != rhs.data_.Tag()) {
        return false;
      }
      return lhs.data_.InvokeWithConst([&rhs](auto iterator) noexcept -> bool {
        return iterator == *rhs.data_.template TryAs<decltype(iterator)>();
      });
    }

    /// Returns whether `lhs` and `rhs` are not equal.
    friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) noexcept {
      return !(lhs == rhs);
    }

    /// Returns the number of elements between `lhs` and `rhs`.
    ///
    /// @throws std::logic_error If `lhs` and `rhs` come from different iterators.
    friend difference_type operator-(const const_iterator& lhs,
                                     const const_iterator& rhs) noexcept(false) {
      if (lhs.data_.Tag() != rhs.data_.Tag()) {
        throw std::logic_error{
            "called lhs-rhs with iterators from different CowRepeated collections"};
      }
      return lhs.data_.InvokeWithConst([&rhs](auto iterator) noexcept -> bool {
        return iterator - *rhs.data_.template TryAs<decltype(iterator)>();
      });
    }

   private:
    /// Returns the value of `Data` given these constructor arguments.
    static Data DataFrom(const CowRepeated& container, bool is_end) noexcept;

    /// The actual data.
    Data data_;
  };

  /// Type of the items yielded by `const_iterator`.
  using value_type = const T;

  /// Constructs an empty container.
  CowRepeated() noexcept : data_{InPlaceType<VectorT>} {}

  /// Creates a span of values taken from `values`.
  ///
  /// @throws std::bad_alloc If the resulting vector cannot be allocated.
  CowRepeated(std::initializer_list<T> values) noexcept(false)
      : data_{InPlaceType<VectorT>, values.begin(), values.end()} {}

  /// Creates a container which wraps a shared `view`.
  explicit CowRepeated(PbView&& view) noexcept : data_{InPlaceType<PbView>, std::move(view)} {}

  /// Returns the number of items in the container.
  std::size_t size() const noexcept;

  /// Returns whether the container is empty.
  bool empty() const noexcept;

  /// Returns a `const_iterator` pointing to the first item of the container.
  const_iterator begin() const noexcept { return const_iterator{*this, /*is_end=*/false}; }
  /// Returns a `const_iterator` pointing past the last item of the container.
  const_iterator end() const noexcept { return const_iterator{*this, /*is_end=*/true}; }

  /// Returns a mutable reference to the vector of elements.
  ///
  /// @throws std::bad_alloc If the contents of the repeated list had to be copied and the
  /// allocation failed.
  VectorT& Vector() noexcept(false) HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND;

  /// Emplaces a new value at the end of the collection and returns a reference to it.
  template <class... Args>
  T& Add(Args&&... args) & noexcept(std::is_nothrow_constructible<T, Args...>::value) {
    VectorT& vector{Vector()};
    vector.emplace_back(std::forward<Args>(args)...);
    return vector.back();
  }

  /// Emplaces a new value and returns `*this`.
  template <class... Args>
  CowRepeated&& Add(Args&&... args) && noexcept(std::is_nothrow_constructible<T, Args...>::value) {
    static_cast<void>(Add(std::forward<Args>(args)...));
    return std::move(*this);
  }

 private:
  /// The actual data.
  Data data_;
};

template <class T>
T CowRepeated<T, /*kPacked=*/true>::const_iterator::operator*() const noexcept {
  return data_.InvokeWithConst([](const auto& iterator) noexcept -> T {
    try {
      return *iterator;
    } catch (const protozero::varint_too_long_exception&) {
      // We check the buffer before creating the `CowRepeated`, so it should be valid.
      assert(false);
    } catch (const protozero::end_of_buffer_exception&) {
      // We check the buffer before creating the `CowRepeated`, so it should be valid.
      assert(false);
    }
    return {};  // `kPacked=true` so `T` is a default-constructible trivial type.
  });
}

template <class T>
auto CowRepeated<T, /*kPacked=*/true>::const_iterator::operator++() noexcept -> const_iterator& {
  data_.InvokeWith([](auto& iterator) noexcept {
    try {
      ++iterator;
    } catch (const protozero::varint_too_long_exception&) {
      // We check the buffer before creating the `CowRepeated`, so it should be valid.
      assert(false);
    } catch (const protozero::end_of_buffer_exception&) {
      // We check the buffer before creating the `CowRepeated`, so it should be valid.
      assert(false);
    }
  });
  return *this;
}

// static
template <class T>
auto CowRepeated<T, /*kPacked=*/true>::const_iterator::DataFrom(const CowRepeated& container,
                                                                bool is_end) noexcept -> Data {
  switch (container.data_.Tag()) {
    case CowRepeated::Data::template kTagFor<VectorT>: {
      const VectorT& data{*container.data_.template TryAs<VectorT>()};
      const T* const ptr{is_end ? horus_internal::PointerAdd(data.data(), data.size())
                                : data.data()};
      return Data{InPlaceType<const T*>, ptr};
    }
    case CowRepeated::Data::template kTagFor<PbView>:
    default: {
      const PbView& data{*container.data_.template TryAs<PbView>()};
      const protozero::const_varint_iterator<T> iterator{
          is_end ? data.Str().end() : data.Str().begin(), data.Str().end()};
      return Data{InPlaceType<protozero::const_varint_iterator<T>>, iterator};
    }
  }
}

template <class T>
std::size_t CowRepeated<T, /*kPacked=*/true>::size() const noexcept {
  switch (data_.Tag()) {
    case Data::template kTagFor<VectorT>: {
      return data_.template TryAs<VectorT>()->size();
    }
    case Data::template kTagFor<PbView>:
    default: {
      const StringView data{data_.template TryAs<PbView>()->Str()};
      const protozero::iterator_range<protozero::const_varint_iterator<T>> range{
          protozero::const_varint_iterator<T>{data.data(), data.end()},
          protozero::const_varint_iterator<T>{data.end(), data.end()},
      };
      return range.size();
    }
  }
}

template <class T>
bool CowRepeated<T, /*kPacked=*/true>::empty() const noexcept {
  switch (data_.Tag()) {
    case Data::template kTagFor<VectorT>: {
      return data_.template TryAs<VectorT>()->empty();
    }
    case Data::template kTagFor<PbView>:
    default: {
      return data_.template TryAs<PbView>()->Str().empty();
    }
  }
}

template <class T>
auto CowRepeated<T, /*kPacked=*/true>::Vector() noexcept(false) -> VectorT& {
  static_assert(std::is_nothrow_constructible<T>::value, "");

  if (data_.template Is<VectorT>()) {
    return *data_.template TryAs<VectorT>();
  }
  VectorT result;
  result.reserve(size());
  for (const T value : *this) {
    result.push_back(value);
  }
  return data_.template Emplace<VectorT>(std::move(result));
}

namespace horus_internal {

/// `add_packed_*` and `get_packed_*` functions for `T`.
template <class T, class Enable = void>
class PackedFns;

template <>
class PackedFns<bool> {
 public:
  constexpr static auto kAddPacked =
      &protozero::pbf_writer::add_packed_bool<CowRepeated<bool, true>::const_iterator>;
  constexpr static auto kGetPacked = &protozero::pbf_reader::get_packed_bool;
};
template <>
class PackedFns<std::uint32_t> {
 public:
  constexpr static auto kAddPacked =
      &protozero::pbf_writer::add_packed_uint32<CowRepeated<std::uint32_t, true>::const_iterator>;
  constexpr static auto kGetPacked = &protozero::pbf_reader::get_packed_uint32;
};
template <>
class PackedFns<std::uint64_t> {
 public:
  constexpr static auto kAddPacked =
      &protozero::pbf_writer::add_packed_uint64<CowRepeated<std::uint64_t, true>::const_iterator>;
  constexpr static auto kGetPacked = &protozero::pbf_reader::get_packed_uint64;
};
template <>
class PackedFns<std::int32_t> {
 public:
  constexpr static auto kAddPacked =
      &protozero::pbf_writer::add_packed_int32<CowRepeated<std::int32_t, true>::const_iterator>;
  constexpr static auto kGetPacked = &protozero::pbf_reader::get_packed_int32;
};
template <>
class PackedFns<std::int64_t> {
 public:
  constexpr static auto kAddPacked =
      &protozero::pbf_writer::add_packed_int64<CowRepeated<std::int64_t, true>::const_iterator>;
  constexpr static auto kGetPacked = &protozero::pbf_reader::get_packed_int64;
};
template <class T>
class PackedFns<T, VoidT<PbEnumTraits<T>>> {
 public:
  constexpr static auto kAddPacked =
      &protozero::pbf_writer::add_packed_enum<typename CowRepeated<T, true>::const_iterator>;
  constexpr static auto kGetPacked = &protozero::pbf_reader::get_packed_enum;
};

}  // namespace horus_internal

template <class T>
class PbTraits<CowRepeated<T, /*kPacked=*/true>> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, const CowRepeated<T, true>& value) {
    static constexpr auto kAdd = horus_internal::PackedFns<T>::kAddPacked;
    (writer.Writer().*kAdd)(tag, value.begin(), value.end());
  }

  /// Deserializes a `CowRepeated` from `reader`.
  static CowRepeated<T> Deserialize(PbReader& reader) {
    static constexpr auto kGet = horus_internal::PackedFns<T>::kGetPacked;
    // protozero doesn't give us direct access to the begin/end pointers so we must recover them
    // from the exposed APIs...
    const char* span_start{reader.Reader().data().data()};
    // Skip length.
    protozero::decode_varint(
        &span_start,
        horus_internal::PointerAdd(reader.Reader().data().data(), reader.Reader().data().size()));
    // Skip items.
    static_cast<void>((reader.Reader().*kGet)());
    const char* const span_end{reader.Reader().data().data()};
    const char* const buffer_start{reader.Buffer().Str().data()};
    std::size_t const buffer_offset{static_cast<std::size_t>(span_start - buffer_start)};
    std::size_t const buffer_len{static_cast<std::size_t>(span_end - span_start)};

    return CowRepeated<T, true>{PbBuffer{reader.Buffer()}.View(buffer_offset, buffer_len)};
  }
};

// MARK: CowRepeated<T, kPacked = false>

template <class T>
class CowRepeated<T, /*kPacked=*/false> final {
  /// Type of the inner `data_`.
  using Data = OneOf<horus_internal::PbViewAndTag, std::vector<T>>;

 public:
  /// Type of the iterator returned by `begin()` and `end()`.
  class const_iterator final {
    /// Type of the inner `data_`.
    using Data = OneOf<PbRepeatedIterator<T>, const T*>;

   public:
    /// Iterator difference type.
    using difference_type = std::ptrdiff_t;
    /// Iterator value type.
    using value_type = Cow<T>;

    /// Constructs a `begin()` or `end()` iterator over the items in `container`.
    const_iterator(const CowRepeated& container, bool is_end) noexcept
        : data_{DataFrom(container, is_end)} {}

    /// Returns the current value.
    Cow<T> operator*() const HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND;

    /// Advances to the next value (`++it`).
    const_iterator& operator++() noexcept;

    /// Returns a copy of the current iterator and advances to the next value (`it++`).
    const_iterator operator++(std::int32_t /* unused */) noexcept {  // NOLINT(cert-dcl21-cpp)
      const_iterator result{*this};
      ++*this;
      return result;
    }

    /// Returns whether `lhs` and `rhs` are equal.
    friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) noexcept {
      if (lhs.data_.Tag() != rhs.data_.Tag()) {
        return false;
      }
      return lhs.data_.InvokeWithConst([&rhs](auto iterator) noexcept -> bool {
        return iterator == *rhs.data_.template TryAs<decltype(iterator)>();
      });
    }

    /// Returns whether `lhs` and `rhs` are not equal.
    friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) noexcept {
      return !(lhs == rhs);
    }

    /// Returns the number of elements between `lhs` and `rhs`.
    friend difference_type operator-(const const_iterator& lhs,
                                     const const_iterator& rhs) noexcept(false) {
      if (lhs.data_.Tag() != rhs.data_.Tag()) {
        throw std::logic_error{
            "called 'lhs - rhs' with iterators from different CowRepeated collections"};
      }
      return lhs.data_.InvokeWithConst([&rhs](auto iterator) noexcept -> bool {
        return iterator - *rhs.data_.template TryAs<decltype(iterator)>();
      });
    }

   private:
    /// Returns the value of `Data` given these constructor arguments.
    static Data DataFrom(const CowRepeated& container, bool is_end) noexcept;

    /// The actual data.
    Data data_;
  };
  /// Type of the items yielded by `const_iterator`.
  using value_type = Cow<T>;

  /// Constructs an empty container.
  CowRepeated() noexcept : data_{InPlaceType<std::vector<T>>} {}

  /// Creates a container which wraps a shared `view`, matching all fields within it with the given
  /// `tag`.
  CowRepeated(PbTag tag, PbView&& view) noexcept  // NOLINT(*-exception-escape): false positive
      : data_{InPlaceType<horus_internal::PbViewAndTag>, std::move(view), tag} {
    static_assert(
        std::is_nothrow_constructible<horus_internal::PbViewAndTag, PbView&&, PbTag>::value, "");
    assert(size() >= 0);  // Compute size, which will fail if the data is invalid.
  }

  // Note that we don't provide `CowRepeated(std::initializer_list)` because it would imply copying
  // all input values, and we know that this specialization only gets complex, non-cheap-to-copy
  // types.

  /// Returns the number of items in the container.
  std::size_t size() const;

  /// Returns whether the container is empty.
  bool empty() const;

  /// Returns a `const_iterator` pointing to the first item of the container.
  const_iterator begin() const noexcept { return const_iterator{*this, /*is_end=*/false}; }
  /// Returns a `const_iterator` pointing past the last item of the container.
  const_iterator end() const noexcept { return const_iterator{*this, /*is_end=*/true}; }

  /// Returns a mutable reference to the vector of elements.
  ///
  /// @throws std::bad_alloc If the contents of the repeated list had to be copied and the
  /// allocation failed.
  std::vector<T>& Vector() noexcept(false) HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND;

  /// (Internal use only) Deserializes the repeated list from a `reader`.
  void InternalDeserialize(PbReader& reader) & noexcept(false);

  /// Emplaces a new value and returns a reference to it.
  ///
  /// @throws std::bad_alloc If the vector cannot be allocated/extended.
  /// @throws std::exception Any exception thrown by constructing `T` with `args...`.
  template <class... Args>
  T& Add(Args&&... args) & noexcept(false) {
    std::vector<T>& vector{Vector()};
    vector.emplace_back(std::forward<Args>(args)...);
    return vector.back();
  }

  /// Emplaces a new value and returns `*this`.
  ///
  /// @throws std::bad_alloc If the vector cannot be allocated/extended.
  /// @throws std::exception Any exception thrown by constructing `T` with `args...`.
  template <class... Args>
  CowRepeated&& Add(Args&&... args) && noexcept(false) {
    static_cast<void>(Add(std::forward<Args>(args)...));
    return std::move(*this);
  }

 private:
  /// The actual data.
  Data data_;
};

template <class T>
Cow<T> CowRepeated<T, /*kPacked=*/false>::const_iterator::operator*() const {
  static_assert(std::is_nothrow_move_constructible<T>::value, "");
  return data_.InvokeWithConst([](const auto& iterator) -> Cow<T> { return *iterator; });
}

template <class T>
auto CowRepeated<T, /*kPacked=*/false>::const_iterator::operator++() noexcept -> const_iterator& {
  data_.InvokeWith([](auto& iterator) noexcept { ++iterator; });
  return *this;
}

// static
template <class T>
auto CowRepeated<T, /*kPacked=*/false>::const_iterator::DataFrom(const CowRepeated& container,
                                                                 bool is_end) noexcept -> Data {
  switch (container.data_.Tag()) {
    case CowRepeated::Data::template kTagFor<std::vector<T>>: {
      const std::vector<T>& data{*container.data_.template TryAs<std::vector<T>>()};
      const T* const ptr{is_end ? horus_internal::PointerAdd(data.data(), data.size())
                                : data.data()};
      return Data{InPlaceType<const T*>, ptr};
    }
    case CowRepeated::Data::template kTagFor<horus_internal::PbViewAndTag>:
    default: {
      if (is_end) {
        return Data{InPlaceType<PbRepeatedIterator<T>>};
      }
      const horus_internal::PbViewAndTag& data{
          *container.data_.template TryAs<horus_internal::PbViewAndTag>()};
      return Data{InPlaceType<PbRepeatedIterator<T>>, data.tag, PbView{data.view}};
    }
  }
}

template <class T>
std::size_t CowRepeated<T, /*kPacked=*/false>::size() const {
  switch (data_.Tag()) {
    case Data::template kTagFor<std::vector<T>>: {
      return data_.template TryAs<std::vector<T>>()->size();
    }
    case Data::template kTagFor<horus_internal::PbViewAndTag>:
    default: {
      const horus_internal::PbViewAndTag& data{
          *data_.template TryAs<horus_internal::PbViewAndTag>()};
      protozero::pbf_reader reader{data.view.Str().data(), data.view.Str().size()};
      std::size_t length{0};
      while (reader.next(data.tag)) {
        reader.skip();
        ++length;
      }
      return length;
    }
  }
}

template <class T>
bool CowRepeated<T, /*kPacked=*/false>::empty() const {
  switch (data_.Tag()) {
    case Data::template kTagFor<std::vector<T>>: {
      return data_.template TryAs<std::vector<T>>()->empty();
    }
    case Data::template kTagFor<horus_internal::PbViewAndTag>:
    default: {
      const horus_internal::PbViewAndTag& data{
          *data_.template TryAs<horus_internal::PbViewAndTag>()};
      protozero::pbf_reader reader{data.view.Str().data(), data.view.Str().size()};
      return !reader.next(data.tag);
    }
  }
}

template <class T>
std::vector<T>& CowRepeated<T, /*kPacked=*/false>::Vector() noexcept(false) {
  static_assert(std::is_nothrow_constructible<T>::value, "");

  if (data_.template Is<std::vector<T>>()) {
    return *data_.template TryAs<std::vector<T>>();
  }
  std::vector<T> result;
  result.reserve(size());
  const horus_internal::PbViewAndTag& data{*data_.template TryAs<horus_internal::PbViewAndTag>()};
  const PbRepeatedIterator<T> end;
  for (PbRepeatedIterator<T> it{data.tag, PbView{data.view}}; it != end; ++it) {
    result.push_back(*it);
  }
  return data_.template Emplace<std::vector<T>>(std::move(result));
}

template <class T>
void CowRepeated<T, /*kPacked=*/false>::InternalDeserialize(PbReader& reader) & noexcept(false) {
  // Get a reference to the underlying data, resetting the current state first if necessary.
  PbTag const reader_tag{reader.Reader().tag()};
  PbView reader_view{reader.ViewIncludingTag(reader_tag, reader.Reader().get_view())};
  switch (data_.Tag()) {
    case Data::template kTagFor<std::vector<T>>: {
      // We owned a vector, but now we're deserializing from a stream so we can reset our state.
      static_cast<void>(
          data_.template Emplace<horus_internal::PbViewAndTag>(std::move(reader_view), reader_tag));
      break;
    }
    case Data::template kTagFor<horus_internal::PbViewAndTag>:
    default: {
      horus_internal::PbViewAndTag& data{*data_.template TryAs<horus_internal::PbViewAndTag>()};
      if ((data.tag != reader_tag) ||
          (data.view.Buffer().Str().data() != reader_view.Buffer().Str().data()) ||
          (data.view.Offset() >= reader_view.Offset())) {
        // The current view and the given view originate from different buffers, so we can reset our
        // state.
        data.view = std::move(reader_view);
        data.tag = reader_tag;
      } else {
        // The current view and the given view originate from the same buffer, so we're encountering
        // a new field in the same parent message.
        std::size_t const new_size{reader_view.Offset() - data.view.Offset() + reader_view.Size()};
        data.view = data.view.Buffer().View(data.view.Offset(), new_size);
      }
      break;
    }
  }
  assert(size() >= 0);  // Make sure that whatever we deserialized is valid.
}

template <class T>
class PbTraits<CowRepeated<T, /*kPacked=*/false>> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, const CowRepeated<T, false>& value) {
    for (const Cow<T>& item : value) {
      PbTraits<T>::Serialize(writer, tag, item.Ref());
    }
  }

  /// Deserializes a `CowRepeated` from `reader`.
  static void Deserialize(PbReader& reader, CowRepeated<T, false>& value) {
    value.InternalDeserialize(reader);
  }
};

/// ADL-resolved definition of `std::distance()` for `const_iterator`.
template <class T, bool kPacked>
typename CowRepeated<T, kPacked>::const_iterator::difference_type distance(
    const typename CowRepeated<T, kPacked>::const_iterator& lhs,
    const typename CowRepeated<T, kPacked>::const_iterator& rhs) noexcept(false) {
  return lhs - rhs;
}

}  // namespace horus

#endif  // HORUS_PB_COW_REPEATED_H_
