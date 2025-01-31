#include "horus/pb/message.h"

#include <string>
#include <utility>

#include "horus/pb/serialize.h"

namespace horus {

std::string PbMessage::SerializeToString() const noexcept(false) {
  PbWriter writer;
  SerializeTo(writer);
  return std::move(writer).ToString();
}

void PbMessage::SerializeToString(std::string& string) const noexcept(false) {
  PbWriter writer{std::move(string)};
  SerializeTo(writer);
  string = std::move(writer).ToString();
}

}  // namespace horus
