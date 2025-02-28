#include "horus/pb/license_server/messages_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

LicenseInfo_AllowedFeature::LicenseInfo_AllowedFeature(const LicenseInfo_AllowedFeature& other) noexcept(false)
    : feature_{other.feature_}
    , set_fields_{other.set_fields_} {}

void LicenseInfo_AllowedFeature::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<LicenseFeature>(writer, /*tag=*/ 1, feature_);
  }
}

void LicenseInfo_AllowedFeature::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<LicenseFeature>(reader, feature_);
        set_fields_[0] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

LicenseInfo::LicenseInfo(const LicenseInfo& other) noexcept(false)
    : expiration_date_{other.expiration_date_}
    , lidar_count_{other.lidar_count_}
    , allowed_features_{other.allowed_features_}
    , set_fields_{other.set_fields_} {}

void LicenseInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<Timestamp>(writer, /*tag=*/ 1, expiration_date_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 2, lidar_count_);
  }
  if (set_fields_[2]) {
    SerializeField<CowRepeated<LicenseInfo_AllowedFeature>>(writer, /*tag=*/ 3, allowed_features_);
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
      case 3: {
        DeserializeField<CowRepeated<LicenseInfo_AllowedFeature>>(reader, allowed_features_);
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
