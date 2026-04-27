#include "horus/pb/config/schema/zone_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

Zone::Zone(const Zone& other) noexcept(false)
    : zone_id_{other.zone_id_}
    , status_{other.status_}
    , type_{other.type_}
    , name_{other.name_}
    , z_range_{other.z_range_}
    , vertices_{other.vertices_}
    , set_fields_{other.set_fields_} {}

void Zone::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, zone_id_);
  }
  if (set_fields_[1]) {
    SerializeField<RepeatedMessageStatus>(writer, /*tag=*/ 2, status_);
  }
  if (set_fields_[2]) {
    SerializeField<ZoneType>(writer, /*tag=*/ 3, type_);
  }
  if (set_fields_[3]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 4, name_);
  }
  if (set_fields_[4]) {
    SerializeField<Range>(writer, /*tag=*/ 5, z_range_);
  }
  if (set_fields_[5]) {
    SerializeField<Vector2dList>(writer, /*tag=*/ 6, vertices_);
  }
}

void Zone::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, zone_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<RepeatedMessageStatus>(reader, status_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<ZoneType>(reader, type_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<CowBytes>(reader, name_);
        set_fields_[3] = true;
        break;
      }
      case 5: {
        DeserializeField<Range>(reader, z_range_);
        set_fields_[4] = true;
        break;
      }
      case 6: {
        DeserializeField<Vector2dList>(reader, vertices_);
        set_fields_[5] = true;
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
