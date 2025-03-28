#include "horus/sdk/health.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <utility>
#include <vector>

#include "horus/logs/format.h"  // IWYU pragma: keep
#include "horus/pb/config/metadata_pb.h"
#include "horus/pb/cow.h"
#include "horus/pb/cow_repeated.h"
#include "horus/pb/license_server/messages_pb.h"
#include "horus/pb/preprocessing/messages_pb.h"
#include "horus/pb/project_manager/service_pb.h"
#include "horus/sdk/errors.h"
#include "horus/strings/str_cat.h"
#include "horus/strings/string_view.h"
#include "horus/types/span.h"

namespace horus {
namespace sdk {

LicenseStatus::LicenseInfo::LicenseInfo(const pb::LicenseInfo& pb_license_info)
    : expiration_timestamp_{std::chrono::system_clock::from_time_t(
                                pb_license_info.expiration_date().seconds()) +
                            std::chrono::nanoseconds{pb_license_info.expiration_date().nanos()}},
      number_of_lidars_{pb_license_info.lidar_count()} {
  allowed_features_.reserve(pb_license_info.allowed_features().size());
  for (const Cow<pb::LicenseInfo::AllowedFeature> allowed_feature :
       pb_license_info.allowed_features()) {
    allowed_features_.push_back(allowed_feature.Ref().feature());
  }
}

LicenseStatus::LicenseStatus(const pb::LicenseStatus& pb_license_status) : license_info_{} {
  switch (pb_license_status.license_level().level_case()) {
    case pb::LicenseLevel::LevelOneof::kLevel1: {
      license_level_ = 1;
      reason_ = StrCat(pb_license_status.license_level().level_1().license_server_unreachable());
      break;
    }
    case pb::LicenseLevel::LevelOneof::kLevel2: {
      license_level_ = 2;
      reason_ = StrCat(pb_license_status.license_level().level_2().license_not_found());
      break;
    }
    case pb::LicenseLevel::LevelOneof::kLevel3: {
      license_level_ = 3;
      switch (pb_license_status.license_level().level_3().reason_case()) {
        case pb::LicenseLevel_Level3::ReasonOneof::kLicenseExpired:
          reason_ = StrCat(pb_license_status.license_level().level_3().license_expired());
          break;
        case pb::LicenseLevel_Level3::ReasonOneof::kLicenseServerDisconnected:
          reason_ =
              StrCat(pb_license_status.license_level().level_3().license_server_disconnected());
          break;
        case pb::LicenseLevel_Level3::ReasonOneof::kNotSet:
        default:
          throw UnexpectedDataReceivedFromRpcError{
              StrCat("Unexpected level 3 reason sent by the RPC: ",
                     static_cast<std::uint32_t>(
                         pb_license_status.license_level().level_3().reason_case()))};
      }
      reason_ = StrCat(pb_license_status.license_level().level_2().license_not_found());
      break;
    }
    case pb::LicenseLevel::LevelOneof::kLevel4: {
      license_level_ = 4;
      reason_ = StrCat(pb_license_status.license_level().level_4().license_active());
      break;
    }
    case pb::LicenseLevel::LevelOneof::kNotSet:
    default: {
      throw sdk::UnexpectedDataReceivedFromRpcError{
          StrCat("Unexpected level sent by the RPC: ",
                 static_cast<std::uint32_t>(pb_license_status.license_level().level_case()))};
    }
  }

  if (pb_license_status.has_license_info()) {
    license_info_ = LicenseInfo{pb_license_status.license_info()};
  }

  const auto if_has_privilege_then_add_to_set =
      [this, privileges{pb_license_status.license_level().privilege()}](
          const Privilege privilege) -> void {
    if ((privileges & static_cast<std::uint32_t>(privilege)) ==
        static_cast<std::uint32_t>(privilege)) {
      privileges_.push_back(privilege);
    }
  };
  if_has_privilege_then_add_to_set(Privilege::kLowest);
  if_has_privilege_then_add_to_set(Privilege::kSubscribe);
  if_has_privilege_then_add_to_set(Privilege::kLicenseActions);
  if_has_privilege_then_add_to_set(Privilege::kRead);
  if_has_privilege_then_add_to_set(Privilege::kWrite);
}

const LicenseStatus::LicenseInfo& LicenseStatus::GetLicenseInfo() const {
  if (!HasLicenseInfo()) {
    throw std::logic_error("current license status does not carry a license info");
  }
  return license_info_;
}

SensorInfo::SensorInfo(const pb::SensorInfo& pb_sensor_info, bool is_unreachable)
    : lidar_id_{pb_sensor_info.lidar_id().Str()}, measured_frequency_{static_cast<double>(NAN)} {
  if (is_unreachable) {
    return;
  }

  std::vector<SensorStatus> statuses;
  const auto if_has_status_flag_then_add_to_set =
      [&statuses, status{pb_sensor_info.status()}](SensorStatus flag) -> void {
    if ((status & static_cast<std::uint32_t>(flag)) == static_cast<std::uint32_t>(flag)) {
      statuses.push_back(flag);
    }
  };
  if_has_status_flag_then_add_to_set(SensorStatus::kNoData);
  if_has_status_flag_then_add_to_set(SensorStatus::kReceivingData);
  if_has_status_flag_then_add_to_set(SensorStatus::kLowFrequency);
  if_has_status_flag_then_add_to_set(SensorStatus::kHighFrequency);
  if_has_status_flag_then_add_to_set(SensorStatus::kTilted);
  if_has_status_flag_then_add_to_set(SensorStatus::kObstructed);
  status_ = std::move(statuses);

  measured_frequency_ = pb_sensor_info.measured_frequency();
}

Span<const SensorInfo::SensorStatus> SensorInfo::Status() const noexcept {
  return {status_.begin().base(), status_.end().base()};
}

bool SensorInfo::HasStatus(const SensorStatus status) const {
  return std::find(status_.begin(), status_.end(), status) != status_.end();
}

StringView SensorHealth::NodeUnreachableErrorMessage() const {
  if (node_unreachable_error_message_.empty()) {
    throw std::logic_error("sensor status can be reached; no unreachable error message found");
  }
  return node_unreachable_error_message_;
}

HealthStatus::HealthStatus(const pb::GetHealthStatusResponse& pb_response)
    : license_status_{pb_response.license_status()} {
  // License level does not grant the Read privilege.
  if (!HasReadPrivilege()) {
    return;
  }

  // License grant the Read privilege: we have sensor and service statuses.
  sensor_statuses_.reserve(pb_response.sensor_statuses().size());
  for (const Cow<pb::GetHealthStatusResponse::SensorHealth>& sensor_health_status :
       pb_response.sensor_statuses()) {
    sensor_statuses_.emplace_back(sensor_health_status.Ref());
  }

  service_statuses_.reserve(pb_response.service_statuses().size());
  for (const Cow<pb::GetHealthStatusResponse::NodeHealth>& service_status :
       pb_response.service_statuses()) {
    service_statuses_.emplace_back(service_status.Ref());
  }
}

Span<const SensorHealth> HealthStatus::SensorStatuses() const {
  return {sensor_statuses_.begin().base(), sensor_statuses_.end().base()};
}

Span<const NodeHealth> HealthStatus::ServiceStatuses() const {
  return {service_statuses_.begin().base(), service_statuses_.end().base()};
}

}  // namespace sdk
}  // namespace horus