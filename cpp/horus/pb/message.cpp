#include "horus/pb/message.h"

#include <cstdint>
#include <utility>
#include <vector>

#include "horus/pb/serialize.h"

namespace horus {

std::vector<std::uint8_t> PbMessage::SerializeToBuffer() const noexcept(false) {
  PbWriter writer;
  SerializeTo(writer);
  return std::move(writer).ToVector();
}

void PbMessage::SerializeToBuffer(std::vector<std::uint8_t>& buffer) const noexcept(false) {
  PbWriter writer{std::move(buffer)};
  SerializeTo(writer);
  buffer = std::move(writer).ToVector();
}

}  // namespace horus
