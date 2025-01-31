/// @file
///
/// The `PbRepeatedIterator<T>` type.

#ifndef HORUS_PB_REPEATED_ITERATOR_H_
#define HORUS_PB_REPEATED_ITERATOR_H_

#include <cassert>
#include <cstddef>
#include <protozero/exception.hpp>
#include <protozero/pbf_reader.hpp>

#include "horus/pb/buffer.h"
#include "horus/pb/serialize.h"
#include "horus/pb/types.h"
#include "horus/types/string_view.h"

namespace horus {

/// An iterator over **non-packed** (and therefore dynamically-sized) fields of a message.
template <class T>
class PbRepeatedIterator final {
 public:
  /// Iterator value type.
  using value_type = T;

  /// Returns a `PbRepeatedIterator` which points to nothing, also used as the `end()` iterator.
  PbRepeatedIterator() noexcept : tag_{0}, data_{PbView{PbBuffer::Borrowed("")}} {}

  /// Constructs an iterator referring to the fields with the given `tag` starting at `data`.
  PbRepeatedIterator(PbTag tag, PbView&& data) noexcept : tag_{tag}, data_{std::move(data)} {}

  /// Returns the current value.
  T operator*() const;

  /// Advances to the next value (`++it`).
  PbRepeatedIterator& operator++() noexcept;

  /// Returns a copy of the current iterator and advances to the next value (`it++`).
  PbRepeatedIterator operator++(std::int32_t /* unused */) noexcept {  // NOLINT(cert-dcl21-cpp)
    PbRepeatedIterator result{*this};
    ++*this;
    return result;
  }

  /// Returns whether `lhs` and `rhs` are equal.
  friend bool operator==(const PbRepeatedIterator& lhs, const PbRepeatedIterator& rhs) noexcept {
    return (lhs.tag_ == rhs.tag_) && (lhs.data_.Str().data() == rhs.data_.Str().data()) &&
           (lhs.data_.Str().size() == rhs.data_.Str().size());
  }

  /// Returns whether `lhs` and `rhs` are not equal.
  friend bool operator!=(const PbRepeatedIterator& lhs, const PbRepeatedIterator& rhs) noexcept {
    return !(lhs == rhs);
  }

 private:
  /// Invokes `method` on `reader`, asserting that it does not throw because the buffer was checked
  /// before the iterator was created.
  static bool AssertNoException(protozero::pbf_reader& reader,
                                bool (protozero::pbf_reader::*method)()) noexcept;
  /// Invokes `method` on `reader`, asserting that it does not throw because the buffer was checked
  /// before the iterator was created.
  static void AssertNoException(protozero::pbf_reader& reader,
                                void (protozero::pbf_reader::*method)()) noexcept;

  /// The tag of the fields we are interested in.
  PbTag tag_;
  /// A view to the data which (possibly) contains the fields we are interested in.
  PbView data_;
};

// MARK: method definitions

template <class T>
// static
bool PbRepeatedIterator<T>::AssertNoException(protozero::pbf_reader& reader,
                                              bool (protozero::pbf_reader::*method)()) noexcept {
  try {
    return (reader.*method)();
  } catch (const protozero::end_of_buffer_exception&) {
    // We check the buffer before creating the `PbRepeatedIterator`, so it should be valid.
    assert(false);
  } catch (const protozero::invalid_tag_exception&) {
    // We check the buffer before creating the `PbRepeatedIterator`, so it should be valid.
    assert(false);
  } catch (const protozero::unknown_pbf_wire_type_exception&) {
    // We check the buffer before creating the `PbRepeatedIterator`, so it should be valid.
    assert(false);
  }
  return false;
}

template <class T>
// static
void PbRepeatedIterator<T>::AssertNoException(protozero::pbf_reader& reader,
                                              void (protozero::pbf_reader::*method)()) noexcept {
  try {
    (reader.*method)();
  } catch (const protozero::end_of_buffer_exception&) {
    // We check the buffer before creating the `PbRepeatedIterator`, so it should be valid.
    assert(false);
  } catch (const protozero::invalid_tag_exception&) {
    // We check the buffer before creating the `PbRepeatedIterator`, so it should be valid.
    assert(false);
  } catch (const protozero::unknown_pbf_wire_type_exception&) {
    // We check the buffer before creating the `PbRepeatedIterator`, so it should be valid.
    assert(false);
  }
}

template <class T>
T PbRepeatedIterator<T>::operator*() const {
  assert(!data_.Str().empty());
  PbReader reader{PbView{data_}};
  const bool read_field{AssertNoException(reader.Reader(), &protozero::pbf_reader::next)};
  assert(read_field);
  static_cast<void>(read_field);
  assert(reader.Reader().tag() == tag_);
  PbReader inner_reader{reader.View(reader.Reader().get_view())};
  return T{inner_reader};
}

template <class T>
PbRepeatedIterator<T>& PbRepeatedIterator<T>::operator++() noexcept {
  protozero::pbf_reader reader{data_.Str().data(), data_.Str().size()};
  std::size_t const len_before{reader.length()};

  // Skip current value.
  const bool read_field{AssertNoException(reader, &protozero::pbf_reader::next)};
  assert(read_field);
  static_cast<void>(read_field);
  AssertNoException(reader, &protozero::pbf_reader::skip);

  // Move to next value.
  for (;;) {
    // Make sure to compute length _before_ reading the tag with `next()`.
    std::size_t const len_after{reader.length()};
    if (!AssertNoException(reader, &protozero::pbf_reader::next)) {
      // Reached end.
      *this = {};
      return *this;
    }
    if (reader.tag() == tag_) {
      // Reached new field with the correct tag.
      data_ = data_.View(len_before - len_after);
      return *this;
    }
    AssertNoException(reader, &protozero::pbf_reader::skip);
  }
}

}  // namespace horus

#endif  // HORUS_PB_REPEATED_ITERATOR_H_
