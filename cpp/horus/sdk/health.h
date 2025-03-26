/// @file
///
/// Types for health status

#ifndef HORUS_SDK_HEALTH_H_
#define HORUS_SDK_HEALTH_H_

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <new>
#include <string>
#include <vector>

#include "horus/attributes.h"
#include "horus/logs/format.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/license_server/messages_pb.h"
#include "horus/pb/logs/logs_pb.h"
#include "horus/pb/preprocessing/messages_pb.h"
#include "horus/pb/project_manager/service_pb.h"
#include "horus/strings/str_cat.h"
#include "horus/strings/string_view.h"

namespace horus {
namespace sdk {

/// Status of horus license.
class LicenseStatus final {
 public:
  /// Privileges that a license can grant.
  using Privilege = pb::LicensePrivilege;

  /// Constructs form a `pb::LicenseStatus`.
  explicit LicenseStatus(const pb::LicenseStatus& pb_license_status);

  /// Gathers license details.
  /// Only available if a license was found by the license server.
  class LicenseInfo final {
   public:
    /// Protobuf enum representing a license feature.
    using LicenseFeature = pb::LicenseFeature;

    /// Default constructor, called if LicenseInfo is empty.
    LicenseInfo() : expiration_timestamp_{}, number_of_lidars_{} {};

    /// Constructos from a `pb::LicenseInfo`.
    explicit LicenseInfo(const pb::LicenseInfo& pb_license_info);

    /// Returns the expiration date and time.
    std::chrono::system_clock::time_point ExpirationTimestamp() const noexcept {
      return expiration_timestamp_;
    }

    /// Returns the number of lidars.
    constexpr std::uint32_t NumberOfLidars() const noexcept { return number_of_lidars_; }

    /// Returns a ref to the allowed features.
    Span<const pb::LicenseFeature> AllowedFeatures() const noexcept HORUS_LIFETIME_BOUND {
      return {allowed_features_.begin().base(), allowed_features_.end().base()};
    }

   private:
    /// Timestamp of license expiration.
    std::chrono::system_clock::time_point expiration_timestamp_;
    /// Number of simultaneous lidars allowed by the license.
    std::uint32_t number_of_lidars_;
    /// List of features allowed by the license.
    std::vector<pb::LicenseFeature> allowed_features_;
  };

  /// Returns the license level.
  std::uint16_t LicenseLevel() const { return license_level_; }

  /// Returns a ref to the license level reason.
  StringView Reason() const noexcept HORUS_LIFETIME_BOUND { return reason_; }

  /// Returns a ref to the privileges.
  Span<const Privilege> Privileges() const noexcept HORUS_LIFETIME_BOUND {
    return {privileges_.begin().base(), privileges_.end().base()};
  }

  /// Returns a ref to the license info.
  ///
  /// @throws `std::logic_error` if `HasLicenseInfo()` is false.
  const LicenseInfo& GetLicenseInfo() const HORUS_LIFETIME_BOUND;

  /// Returns whether a license info is carred by the license status.
  bool HasLicenseInfo() const noexcept { return license_level_ > 2; }

  /// Returns whether the license grants a specific privilege.
  bool HasPrivilege(const Privilege privilege) const noexcept {
    return std::find(privileges_.begin(), privileges_.end(), privilege) != privileges_.end();
  }

 private:
  /// Numeric value of license level. Ranges from 1 to 4.
  std::uint16_t license_level_;
  /// The reason why the license is at that level.
  std::string reason_;
  /// The list of privileges granted by the license.
  std::vector<Privilege> privileges_;
  /// If a license was found, i.e if level is >= 3, contains the info of the license.
  LicenseInfo license_info_;
};

/// Contains the current status of a lidar.
class SensorInfo final {
 public:
  /// Sensor status enum.
  using SensorStatus = pb::SensorStatus;

  /// Constructs from a `pb::SensorInfo`.
  explicit SensorInfo(const pb::SensorInfo& pb_sensor_info, bool is_unreachable);

  /// Returns a view to lidar id.
  StringView LidarId() const noexcept HORUS_LIFETIME_BOUND { return lidar_id_; }

  /// Returns a span containing the status.
  ///
  /// Returns an empty span if `IsUnreachable()` returns true.
  Span<const SensorStatus> Status() const noexcept HORUS_LIFETIME_BOUND;

  /// Returns the frequency.
  double MeasuredFrequencyHz() const noexcept { return measured_frequency_; };

  /// If the sensor is unreachable, status and measured_frequency are set to void.
  bool IsUnreachable() const { return status_.empty(); }

  /// Returns the presente of `status` in the status list.
  bool HasStatus(const SensorStatus status) const;

 private:
  /// Lidar unique identifier.
  std::string lidar_id_;
  /// Array of `SensorStatus`.
  std::vector<SensorStatus> status_;
  /// Measured frequency of the lidar, in Hz. Is set to NaN if `IsUnreachable()` is true.
  double measured_frequency_;
};

/// Contains the current status of a lidar.
class SensorHealth final {
 public:
  /// Constructs from a `pb::SensorInfo`.
  explicit SensorHealth(const pb::GetHealthStatusResponse::SensorHealth& pb_sensor_health_status)
      : preprocessing_node_id_{pb_sensor_health_status.preprocessing_node_id().Str()},
        info_{pb_sensor_health_status.info(),
              pb_sensor_health_status.node_unreachable_error_case() !=
                  pb::GetHealthStatusResponse::SensorHealth::NodeUnreachableErrorOneof::kNotSet} {
    std::string error_str{};
    switch (pb_sensor_health_status.node_unreachable_error_case()) {
      case pb::GetHealthStatusResponse::SensorHealth::NodeUnreachableErrorOneof::kTimeout: {
        StrAppendTo(error_str, pb_sensor_health_status.timeout());
        break;
      }
      case pb::GetHealthStatusResponse::SensorHealth::NodeUnreachableErrorOneof::kNotSet:
      default: {
        return;
      }
    }
    node_unreachable_error_message_ = std::move(error_str);
  }

  /// Returns a view to the preprocessing node id.
  StringView PreprocessingNodeId() const noexcept HORUS_LIFETIME_BOUND {
    return preprocessing_node_id_;
  }

  /// Returns a reference to sensor info.
  const SensorInfo& Info() const noexcept HORUS_LIFETIME_BOUND { return info_; };

  /// Returns a reference to the unreachable error message.
  ///
  /// @throws `std::logic_error` if `NodeIsUnreachable()` is false.
  StringView NodeUnreachableErrorMessage() const HORUS_LIFETIME_BOUND;

  /// Returns whether node is unreachable, i.e a error message is set.
  bool IsNodeUnreachable() const noexcept { return !node_unreachable_error_message_.empty(); }

 private:
  /// Responsible node id.
  std::string preprocessing_node_id_;
  /// Information of the sensor.
  SensorInfo info_;
  /// Message if node is unreachable. Empty if the node is actually reachable.
  std::string node_unreachable_error_message_;
};

/// Health status of a specific node.
struct NodeHealth {
  /// Service related to the node.
  using Service = pb::GetHealthStatusResponse::NodeHealth::Service;
  /// Status of the node.
  using Status = pb::GetHealthStatusResponse::NodeHealth::Status;

  /// Service related to the node.
  Service service;
  /// Unique identifier of the node.
  std::string node_id;
  /// Status of the node.
  Status node_status;

  /// Constructs from a `pb::NodeHealth`.
  explicit NodeHealth(const pb::GetHealthStatusResponse::NodeHealth& pb_node_status)
      : service{pb_node_status.service()},
        node_id{pb_node_status.node_id().Str()},
        node_status{pb_node_status.node_status()} {}
};

/// Request to call `ProjectManagerService.GetHealthStatus()`.
struct GetHealthStatusRequest {};

/// Health status of the system.
class HealthStatus final {
 public:
  /// Constructs from a `pb::GetHealthStatusResponse`.
  explicit HealthStatus(const pb::GetHealthStatusResponse& pb_response);

  /// Returns a reference to the license status.
  constexpr const LicenseStatus& GetLicenseStatus() const noexcept HORUS_LIFETIME_BOUND {
    return license_status_;
  }

  /// Returns a span to the sensor statuses.
  ///
  /// Returns empty span if `HasReadPrivilege()` returns false.
  Span<const SensorHealth> SensorStatuses() const HORUS_LIFETIME_BOUND;

  /// Returns a span to the service statuses.
  ///
  /// Returns empty span if `HasReadPrivilege()` returns false.
  Span<const NodeHealth> ServiceStatuses() const HORUS_LIFETIME_BOUND;

  /// Returns true if the current license level grants READ privilege.
  bool HasReadPrivilege() const noexcept {
    return license_status_.HasPrivilege(LicenseStatus::Privilege::kRead);
  }

 private:
  /// Status of the license.
  LicenseStatus license_status_;
  /// Statuses of the running lidars.
  std::vector<SensorHealth> sensor_statuses_;
  /// Statuses of the services.
  std::vector<NodeHealth> service_statuses_;
};

}  // namespace sdk
}  // namespace horus

#endif  // HORUS_SDK_HEALTH_H_
