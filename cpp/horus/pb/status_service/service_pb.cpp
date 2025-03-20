#include "horus/pb/status_service/service_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {



GetVersionResponse::GetVersionResponse(const GetVersionResponse& other) noexcept(false)
    : version_{other.version_}
    , set_fields_{other.set_fields_} {}

void GetVersionResponse::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<Version>(writer, /*tag=*/ 1, version_);
  }
}

void GetVersionResponse::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<Version>(reader, version_);
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
