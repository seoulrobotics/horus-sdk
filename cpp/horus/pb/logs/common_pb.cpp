#include "horus/pb/logs/common_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {
namespace logs {

ValidationError::ValidationError(const ValidationError& other) noexcept(false)
    : path_{other.path_}
    , message_{other.message_}
    , set_fields_{other.set_fields_} {}

void ValidationError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, message_);
  }
}

void ValidationError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, message_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

}  // namespace logs
}  // namespace pb
}  // namespace sdk
}  // namespace horus
