#include "horus/pb/license_server/messages_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

LicenseInfo::LicenseInfo(const LicenseInfo& other) noexcept(false)
    : expiration_date_{other.expiration_date_}
    , lidar_count_{other.lidar_count_}
    , set_fields_{other.set_fields_} {}

void LicenseInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<Timestamp>(writer, /*tag=*/ 1, expiration_date_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 2, lidar_count_);
  }
}

void LicenseInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<Timestamp>(reader, expiration_date_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint32_t>(reader, lidar_count_);
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

}  // namespace pb
}  // namespace sdk
}  // namespace horus
