#include "horus/pb/project_manager/service_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

LicenseLevel_Level1::LicenseLevel_Level1(const LicenseLevel_Level1& other) noexcept(false)
    : license_server_unreachable_{other.license_server_unreachable_}
    , reason_{other.reason_}
    , set_fields_{other.set_fields_} {}

void LicenseLevel_Level1::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<logs::LicenseServerConnectionError>(writer, /*tag=*/ 1, license_server_unreachable_);
  }
}

void LicenseLevel_Level1::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        clear_reason();
        reason_ = ReasonOneof::kLicenseServerUnreachable;
        DeserializeField<logs::LicenseServerConnectionError>(reader, license_server_unreachable_);
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

LicenseLevel_Level2::LicenseLevel_Level2(const LicenseLevel_Level2& other) noexcept(false)
    : license_not_found_{other.license_not_found_}
    , reason_{other.reason_}
    , set_fields_{other.set_fields_} {}

void LicenseLevel_Level2::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<logs::LicenseNotFoundError>(writer, /*tag=*/ 1, license_not_found_);
  }
}

void LicenseLevel_Level2::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        clear_reason();
        reason_ = ReasonOneof::kLicenseNotFound;
        DeserializeField<logs::LicenseNotFoundError>(reader, license_not_found_);
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

LicenseLevel_Level3::LicenseLevel_Level3(const LicenseLevel_Level3& other) noexcept(false)
    : license_expired_{other.license_expired_}
    , license_server_disconnected_{other.license_server_disconnected_}
    , reason_{other.reason_}
    , set_fields_{other.set_fields_} {}

void LicenseLevel_Level3::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<logs::LicenseExpiredError>(writer, /*tag=*/ 1, license_expired_);
  }
  if (set_fields_[1]) {
    SerializeField<logs::LicenseServerConnectionError>(writer, /*tag=*/ 2, license_server_disconnected_);
  }
}

void LicenseLevel_Level3::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        clear_reason();
        reason_ = ReasonOneof::kLicenseExpired;
        DeserializeField<logs::LicenseExpiredError>(reader, license_expired_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        clear_reason();
        reason_ = ReasonOneof::kLicenseServerDisconnected;
        DeserializeField<logs::LicenseServerConnectionError>(reader, license_server_disconnected_);
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

LicenseLevel_Level4::LicenseLevel_Level4(const LicenseLevel_Level4& other) noexcept(false)
    : license_active_{other.license_active_}
    , reason_{other.reason_}
    , set_fields_{other.set_fields_} {}

void LicenseLevel_Level4::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<logs::LicenseActiveInfo>(writer, /*tag=*/ 1, license_active_);
  }
}

void LicenseLevel_Level4::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        clear_reason();
        reason_ = ReasonOneof::kLicenseActive;
        DeserializeField<logs::LicenseActiveInfo>(reader, license_active_);
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

LicenseLevel::LicenseLevel(const LicenseLevel& other) noexcept(false)
    : level_1_{other.level_1_}
    , level_2_{other.level_2_}
    , level_3_{other.level_3_}
    , level_4_{other.level_4_}
    , privilege_{other.privilege_}
    , level_{other.level_}
    , set_fields_{other.set_fields_} {}

void LicenseLevel::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<LicenseLevel_Level1>(writer, /*tag=*/ 10, level_1_);
  }
  if (set_fields_[1]) {
    SerializeField<LicenseLevel_Level2>(writer, /*tag=*/ 20, level_2_);
  }
  if (set_fields_[2]) {
    SerializeField<LicenseLevel_Level3>(writer, /*tag=*/ 30, level_3_);
  }
  if (set_fields_[3]) {
    SerializeField<LicenseLevel_Level4>(writer, /*tag=*/ 40, level_4_);
  }
  if (set_fields_[4]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 60, privilege_);
  }
}

void LicenseLevel::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 10: {
        clear_level();
        level_ = LevelOneof::kLevel1;
        DeserializeField<LicenseLevel_Level1>(reader, level_1_);
        set_fields_[0] = true;
        break;
      }
      case 20: {
        clear_level();
        level_ = LevelOneof::kLevel2;
        DeserializeField<LicenseLevel_Level2>(reader, level_2_);
        set_fields_[1] = true;
        break;
      }
      case 30: {
        clear_level();
        level_ = LevelOneof::kLevel3;
        DeserializeField<LicenseLevel_Level3>(reader, level_3_);
        set_fields_[2] = true;
        break;
      }
      case 40: {
        clear_level();
        level_ = LevelOneof::kLevel4;
        DeserializeField<LicenseLevel_Level4>(reader, level_4_);
        set_fields_[3] = true;
        break;
      }
      case 60: {
        DeserializeField<std::uint32_t>(reader, privilege_);
        set_fields_[4] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

LicenseStatus::LicenseStatus(const LicenseStatus& other) noexcept(false)
    : license_level_{other.license_level_}
    , license_info_{other.license_info_}
    , set_fields_{other.set_fields_} {}

void LicenseStatus::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<LicenseLevel>(writer, /*tag=*/ 1, license_level_);
  }
  if (set_fields_[1]) {
    SerializeField<LicenseInfo>(writer, /*tag=*/ 2, license_info_);
  }
}

void LicenseStatus::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<LicenseLevel>(reader, license_level_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<LicenseInfo>(reader, license_info_);
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



GetHealthStatusResponse_NodeHealth::GetHealthStatusResponse_NodeHealth(const GetHealthStatusResponse_NodeHealth& other) noexcept(false)
    : service_{other.service_}
    , node_id_{other.node_id_}
    , node_status_{other.node_status_}
    , set_fields_{other.set_fields_} {}

void GetHealthStatusResponse_NodeHealth::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<GetHealthStatusResponse_NodeHealth_Service>(writer, /*tag=*/ 1, service_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, node_id_);
  }
  if (set_fields_[2]) {
    SerializeField<GetHealthStatusResponse_NodeHealth_Status>(writer, /*tag=*/ 3, node_status_);
  }
}

void GetHealthStatusResponse_NodeHealth::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<GetHealthStatusResponse_NodeHealth_Service>(reader, service_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, node_id_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<GetHealthStatusResponse_NodeHealth_Status>(reader, node_status_);
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

GetHealthStatusResponse_SensorHealth::GetHealthStatusResponse_SensorHealth(const GetHealthStatusResponse_SensorHealth& other) noexcept(false)
    : preprocessing_node_id_{other.preprocessing_node_id_}
    , info_{other.info_}
    , timeout_{other.timeout_}
    , node_unreachable_error_{other.node_unreachable_error_}
    , set_fields_{other.set_fields_} {}

void GetHealthStatusResponse_SensorHealth::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, preprocessing_node_id_);
  }
  if (set_fields_[1]) {
    SerializeField<SensorInfo>(writer, /*tag=*/ 2, info_);
  }
  if (set_fields_[2]) {
    SerializeField<logs::ServiceConnectionTimedOut>(writer, /*tag=*/ 3, timeout_);
  }
}

void GetHealthStatusResponse_SensorHealth::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, preprocessing_node_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<SensorInfo>(reader, info_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        clear_node_unreachable_error();
        node_unreachable_error_ = NodeUnreachableErrorOneof::kTimeout;
        DeserializeField<logs::ServiceConnectionTimedOut>(reader, timeout_);
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



GetHealthStatusResponse::GetHealthStatusResponse(const GetHealthStatusResponse& other) noexcept(false)
    : license_status_{other.license_status_}
    , sensor_statuses_{other.sensor_statuses_}
    , service_statuses_{other.service_statuses_}
    , node_resources_{other.node_resources_}
    , set_fields_{other.set_fields_} {}

void GetHealthStatusResponse::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<LicenseStatus>(writer, /*tag=*/ 1, license_status_);
  }
  if (set_fields_[1]) {
    SerializeField<CowRepeated<GetHealthStatusResponse_SensorHealth>>(writer, /*tag=*/ 2, sensor_statuses_);
  }
  if (set_fields_[2]) {
    SerializeField<CowRepeated<GetHealthStatusResponse_NodeHealth>>(writer, /*tag=*/ 3, service_statuses_);
  }
  if (set_fields_[3]) {
    SerializeField<CowRepeated<GetHealthStatusResponse_NodeResources>>(writer, /*tag=*/ 4, node_resources_);
  }
}

void GetHealthStatusResponse::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<LicenseStatus>(reader, license_status_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowRepeated<GetHealthStatusResponse_SensorHealth>>(reader, sensor_statuses_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowRepeated<GetHealthStatusResponse_NodeHealth>>(reader, service_statuses_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<CowRepeated<GetHealthStatusResponse_NodeResources>>(reader, node_resources_);
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

}  // namespace pb
}  // namespace sdk
}  // namespace horus
