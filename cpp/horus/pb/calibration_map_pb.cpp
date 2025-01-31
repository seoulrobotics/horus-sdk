#include "horus/pb/calibration_map_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

CalibrationMap::CalibrationMap(const CalibrationMap& other) noexcept(false)
    : map_points_{other.map_points_}
    , set_fields_{other.set_fields_} {}

void CalibrationMap::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowSpan<float>, PbDeserFlags::kFixed>(writer, /*tag=*/ 2, map_points_);
  }
}

void CalibrationMap::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 2: {
        DeserializeField<CowSpan<float>, PbDeserFlags::kFixed>(reader, map_points_);
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

}  // namespace pb
}  // namespace sdk
}  // namespace horus
