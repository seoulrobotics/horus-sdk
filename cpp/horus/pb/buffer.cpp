#include "horus/pb/buffer.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "horus/pointer/cast.h"
#include "horus/strings/string_view.h"

namespace horus {

// static
PbBuffer PbBuffer::Copy(StringView string) noexcept(false) {
  // NOLINTNEXTLINE(*-avoid-c-arrays)
  std::unique_ptr<char[]> buffer{std::make_unique<char[]>(string.size())};
  // `std::memcpy()` is not allowed...
  static_cast<void>(std::copy(string.begin(), string.end(), buffer.get()));
  // Only release `buffer` if the constructor of `std::shared_ptr` succeeds, since it might throw if
  // we're out of memory. We can't use `std::shared_ptr(std::unique_ptr&&)` here since the
  // conversion from array to pointer is not allowed in C++17 and up.
  // NOLINTNEXTLINE(*-avoid-c-arrays)
  std::shared_ptr<char> shared_buffer{buffer.get(), std::default_delete<char[]>()};
  static_cast<void>(buffer.release());
  return PbBuffer{std::move(shared_buffer), string.size()};
}

PbBuffer::PbBuffer(std::vector<std::uint8_t>&& buffer) noexcept(false)
    : buffer_{nullptr}, size_{buffer.size()} {
  const std::shared_ptr<const std::vector<std::uint8_t>> shared_buffer{
      std::make_shared<std::vector<std::uint8_t>>(std::move(buffer))};

  buffer_ = {shared_buffer, SafePointerCast<char>(shared_buffer->data())};
}

PbBuffer::PbBuffer(std::string&& buffer) noexcept(false) : buffer_{nullptr}, size_{buffer.size()} {
  const std::shared_ptr<const std::string> shared_buffer{
      std::make_shared<std::string>(std::move(buffer))};

  buffer_ = {shared_buffer, shared_buffer->data()};
}

}  // namespace horus
