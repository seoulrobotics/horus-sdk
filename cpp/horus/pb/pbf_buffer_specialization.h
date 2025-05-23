/// @file
///
/// Specialization of `protozero::buffer_customization` for `std::vector<std::uint8_t>`.
///
/// The naming of this file comes from `protozero::basic_pbf_writer`.

#ifndef HORUS_PB_PBF_BUFFER_SPECIALIZATION_H_
#define HORUS_PB_PBF_BUFFER_SPECIALIZATION_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <protozero/buffer_tmpl.hpp>
#include <vector>

#include "horus/pointer/arithmetic.h"
#include "horus/pointer/cast.h"

namespace protozero {

// NOLINTBEGIN(*-identifier-naming): must respect naming of protozero

/// See `protozero::buffer_customization<std::vector<char>>`.
template <>
struct buffer_customization<std::vector<std::uint8_t>> {
  static std::size_t size(std::vector<std::uint8_t> const* buffer) noexcept {
    return buffer->size();
  }

  static void append(std::vector<std::uint8_t>* buffer, char const* data, std::size_t count) {
    // NOLINTNEXTLINE(*-pointer-arithmetic)
    static_cast<void>(buffer->insert(buffer->end(), data, horus::PointerAdd(data, count)));
  }

  static void append_zeros(std::vector<std::uint8_t>* buffer, std::size_t count) {
    static_cast<void>(buffer->insert(buffer->end(), count, '\0'));
  }

  static void resize(std::vector<std::uint8_t>* buffer, std::size_t size) {
    assert(size < buffer->size());
    buffer->resize(size);
  }

  static void reserve_additional(std::vector<std::uint8_t>* buffer, std::size_t size) {
    buffer->reserve(buffer->size() + size);
  }

  // NOLINTNEXTLINE(*-identifier-length)
  static void erase_range(std::vector<std::uint8_t>* buffer, std::size_t from, std::size_t to) {
    assert(from <= buffer->size());
    assert(to <= buffer->size());
    assert(from <= to);
    static_cast<void>(buffer->erase(
        std::next(buffer->begin(),
                  static_cast<std::vector<std::uint8_t>::iterator::difference_type>(from)),
        std::next(buffer->begin(),
                  static_cast<std::vector<std::uint8_t>::iterator::difference_type>(to))));
  }

  static char* at_pos(std::vector<std::uint8_t>* buffer, std::size_t pos) {
    assert(pos <= buffer->size());
    // NOLINTNEXTLINE(*-pointer-arithmetic)
    return horus::SafePointerCast<char>(horus::PointerAdd(buffer->data(), pos));
  }

  // NOLINTNEXTLINE(*-identifier-length)
  static void push_back(std::vector<std::uint8_t>* buffer, char ch) {
    buffer->push_back(static_cast<std::uint8_t>(ch));
  }
};

// NOLINTEND(*-identifier-naming)

}  // namespace protozero

#endif  // HORUS_PB_PBF_BUFFER_SPECIALIZATION_H_
