#include "horus/pb/status_service/messages_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

Version::Version(const Version& other) noexcept(false)
    : major_{other.major_}
    , patch_{other.patch_}
    , pre_{other.pre_}
    , set_fields_{other.set_fields_} {}

void Version::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 1, major_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 3, patch_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 4, pre_);
  }
}

void Version::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint32_t>(reader, major_);
        set_fields_[0] = true;
        break;
      }
      case 3: {
        DeserializeField<std::uint32_t>(reader, patch_);
        set_fields_[1] = true;
        break;
      }
      case 4: {
        DeserializeField<CowBytes>(reader, pre_);
        set_fields_[2] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus
