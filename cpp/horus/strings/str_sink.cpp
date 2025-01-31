#include "horus/strings/str_sink.h"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace horus {
namespace horus_internal {

std::size_t StringifyDoubleTo(std::array<char, kDoubleMaxStrSize + 1>& buffer,
                              double value) noexcept {
  // https://en.cppreference.com/w/cpp/io/c/fprintf
  // NOLINTNEXTLINE(*-vararg)
  std::int32_t const written{std::snprintf(buffer.data(), buffer.size(), "%g", value)};
  assert(written > 0);
  std::size_t const written_size{static_cast<std::size_t>(written)};
  assert(written_size <= buffer.size());  // `written_size` does not include null terminator.
  return written_size;
}

std::size_t StringifyPointerTo(std::array<char, kPointerMaxStrSizeWithPrefix + 1>& buffer,
                               const void* value) noexcept {
  // https://en.cppreference.com/w/cpp/io/c/fprintf
  // NOLINTNEXTLINE(*-vararg)
  std::int32_t const written{std::snprintf(buffer.data(), buffer.size(), "%p", value)};
  assert(written > 0);
  std::size_t const written_size{static_cast<std::size_t>(written)};
  assert(written_size <= buffer.size());  // `written_size` does not include null terminator.
  return written_size;
}

}  // namespace horus_internal
}  // namespace horus
