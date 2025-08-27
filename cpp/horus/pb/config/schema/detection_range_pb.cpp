#include "horus/pb/config/schema/detection_range_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

DetectionRange::DetectionRange(const DetectionRange& other) noexcept(false)
    : x_range_{other.x_range_}
    , y_range_{other.y_range_}
    , z_range_{other.z_range_}
    , set_fields_{other.set_fields_} {}

void DetectionRange::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<Range>(writer, /*tag=*/ 1, x_range_);
  }
  if (set_fields_[1]) {
    SerializeField<Range>(writer, /*tag=*/ 2, y_range_);
  }
  if (set_fields_[2]) {
    SerializeField<Range>(writer, /*tag=*/ 3, z_range_);
  }
}

void DetectionRange::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<Range>(reader, x_range_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<Range>(reader, y_range_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<Range>(reader, z_range_);
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
