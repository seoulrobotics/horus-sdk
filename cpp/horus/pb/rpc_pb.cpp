#include "horus/pb/rpc_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {



RpcServiceOptions::RpcServiceOptions(const RpcServiceOptions& other) noexcept(false)
    : id_{other.id_}
    , description_{other.description_}
    , set_fields_{other.set_fields_} {}

void RpcServiceOptions::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 1, id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, description_);
  }
}

void RpcServiceOptions::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint32_t>(reader, id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, description_);
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

RpcMethodOptions_SubscribeMethod::RpcMethodOptions_SubscribeMethod(const RpcMethodOptions_SubscribeMethod& other) noexcept(false)
    : subscriber_name_{other.subscriber_name_}
    , set_fields_{other.set_fields_} {}

void RpcMethodOptions_SubscribeMethod::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, subscriber_name_);
  }
}

void RpcMethodOptions_SubscribeMethod::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, subscriber_name_);
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

RpcMethodOptions_UnsubscribeMethod::RpcMethodOptions_UnsubscribeMethod(const RpcMethodOptions_UnsubscribeMethod& other) noexcept(false)
    : subscriber_name_{other.subscriber_name_}
    , set_fields_{other.set_fields_} {}

void RpcMethodOptions_UnsubscribeMethod::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, subscriber_name_);
  }
}

void RpcMethodOptions_UnsubscribeMethod::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, subscriber_name_);
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

RpcMethodOptions::RpcMethodOptions(const RpcMethodOptions& other) noexcept(false)
    : id_{other.id_}
    , description_{other.description_}
    , subscribe_{other.subscribe_}
    , unsubscribe_{other.unsubscribe_}
    , type_{other.type_}
    , set_fields_{other.set_fields_} {}

void RpcMethodOptions::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 1, id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, description_);
  }
  if (set_fields_[2]) {
    SerializeField<RpcMethodOptions_SubscribeMethod>(writer, /*tag=*/ 3, subscribe_);
  }
  if (set_fields_[3]) {
    SerializeField<RpcMethodOptions_UnsubscribeMethod>(writer, /*tag=*/ 4, unsubscribe_);
  }
}

void RpcMethodOptions::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint32_t>(reader, id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, description_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        clear_type();
        type_ = TypeOneof::kSubscribe;
        DeserializeField<RpcMethodOptions_SubscribeMethod>(reader, subscribe_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        clear_type();
        type_ = TypeOneof::kUnsubscribe;
        DeserializeField<RpcMethodOptions_UnsubscribeMethod>(reader, unsubscribe_);
        set_fields_[3] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}



DefaultSubscribeResponse::DefaultSubscribeResponse(const DefaultSubscribeResponse& other) noexcept(false)
    : connection_error_{other.connection_error_}
    , error_{other.error_}
    , set_fields_{other.set_fields_} {}

void DefaultSubscribeResponse::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<logs::RpcConnectionError>(writer, /*tag=*/ 1, connection_error_);
  }
}

void DefaultSubscribeResponse::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        clear_error();
        error_ = ErrorOneof::kConnectionError;
        DeserializeField<logs::RpcConnectionError>(reader, connection_error_);
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



DefaultUnsubscribeResponse::DefaultUnsubscribeResponse(const DefaultUnsubscribeResponse& other) noexcept(false)
    : disconnection_error_{other.disconnection_error_}
    , error_{other.error_}
    , set_fields_{other.set_fields_} {}

void DefaultUnsubscribeResponse::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<logs::RpcDisconnectionError>(writer, /*tag=*/ 1, disconnection_error_);
  }
}

void DefaultUnsubscribeResponse::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        clear_error();
        error_ = ErrorOneof::kDisconnectionError;
        DeserializeField<logs::RpcDisconnectionError>(reader, disconnection_error_);
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
