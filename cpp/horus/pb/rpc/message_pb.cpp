#include "horus/pb/rpc/message_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

RpcMessage::RpcMessage(const RpcMessage& other) noexcept(false)
    : version_{other.version_}
    , service_id_{other.service_id_}
    , message_bytes_{other.message_bytes_}
    , request_id_{other.request_id_}
    , method_id_{other.method_id_}
    , error_{other.error_}
    , cancel_{other.cancel_}
    , set_fields_{other.set_fields_} {}

void RpcMessage::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<RpcMessage_Version>(writer, /*tag=*/ 1, version_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 2, service_id_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, message_bytes_);
  }
  if (set_fields_[3]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 4, request_id_);
  }
  if (set_fields_[4]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 5, method_id_);
  }
  if (set_fields_[5]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 6, error_);
  }
  if (set_fields_[6]) {
    SerializeField<bool>(writer, /*tag=*/ 7, cancel_);
  }
}

void RpcMessage::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<RpcMessage_Version>(reader, version_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint32_t>(reader, service_id_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, message_bytes_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<std::uint32_t>(reader, request_id_);
        set_fields_[3] = true;
        break;
      }
      case 5: {
        DeserializeField<std::uint32_t>(reader, method_id_);
        set_fields_[4] = true;
        break;
      }
      case 6: {
        DeserializeField<CowBytes>(reader, error_);
        set_fields_[5] = true;
        break;
      }
      case 7: {
        DeserializeField<bool>(reader, cancel_);
        set_fields_[6] = true;
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
