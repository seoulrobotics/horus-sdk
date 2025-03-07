#include <chrono>
#include <cstddef>
#include <cstdio>
#include <string>
#include <utility>
#include <vector>

#include "examples/helpers.h"
#include "horus/future/cancel.h"
#include "horus/rpc/endpoint.h"
#include "horus/rpc/services.h"
#include "horus/sdk.h"
#include "horus/sdk/errors.h"
#include "horus/sdk/health.h"
#include "horus/strings//str_cat.h"
#include "horus/strings/chrono.h"
#include "horus/strings/logging.h"
#include "horus/strings/stdio.h"
#include "horus/strings/str_sink.h"
#include "horus/types/span.h"
#include "horus/types/string_view.h"

namespace {

/// Converts a `LicenseFeature` to its stringified representation.
horus::StringView LicenseFeatureToString(
    const horus::sdk::LicenseStatus::LicenseInfo::LicenseFeature feature) {
  switch (feature) {
    case horus::sdk::LicenseStatus::LicenseInfo::LicenseFeature::kCan: {
      return "CAN bus";
    }
    case horus::sdk::LicenseStatus::LicenseInfo::LicenseFeature::kVinAssociator: {
      return "VIN associator";
    }
    case horus::sdk::LicenseStatus::LicenseInfo::LicenseFeature::kMultiRosbag: {
      return "Multiple rosbags";
    }
    case horus::sdk::LicenseStatus::LicenseInfo::LicenseFeature::kDebuggingSupport: {
      return "Debugging support";
    }
    case horus::sdk::LicenseStatus::LicenseInfo::LicenseFeature::kAtlas: {
      return "Atlas";
    }
    case horus::sdk::LicenseStatus::LicenseInfo::LicenseFeature::kMacgyver: {
      return "SR Analytics";
    }
    case horus::sdk::LicenseStatus::LicenseInfo::LicenseFeature::kUnknownWireValue:
    default: {
      return "UNKNOWN";
    }
  }
}

/// Stringifies a `LicenseStatus`.
void PrintLicenseStatus(const horus::sdk::LicenseStatus& license_status) {
  horus::StrAppendToSink(horus::StdoutSink(), "- License level: ", license_status.LicenseLevel(),
                         "\n");
  horus::StrAppendToSink(horus::StdoutSink(), "- Reason: ", license_status.Reason(), "\n");

  horus::StrAppendToSink(horus::StdoutSink(), "- Privileges:\n");
  if (license_status.HasPrivilege(horus::sdk::LicenseStatus::Privilege::kLowest)) {
    horus::StrAppendToSink(horus::StdoutSink(), "  - Lowest privilege\n");
  }
  if (license_status.HasPrivilege(horus::sdk::LicenseStatus::Privilege::kSubscribe)) {
    horus::StrAppendToSink(horus::StdoutSink(), "  - Subscribe to services\n");
  }
  if (license_status.HasPrivilege(horus::sdk::LicenseStatus::Privilege::kLicenseActions)) {
    horus::StrAppendToSink(horus::StdoutSink(), "  - Perform license actions\n");
  }
  if (license_status.HasPrivilege(horus::sdk::LicenseStatus::Privilege::kRead)) {
    horus::StrAppendToSink(horus::StdoutSink(), "  - Read\n");
  }
  if (license_status.HasPrivilege(horus::sdk::LicenseStatus::Privilege::kWrite)) {
    horus::StrAppendToSink(horus::StdoutSink(), "  - Write\n");
  }

  if (!license_status.HasLicenseInfo()) {
    return;
  }

  const horus::sdk::LicenseStatus::LicenseInfo& license_info{license_status.GetLicenseInfo()};

  horus::StrAppendToSink(horus::StdoutSink(), "- License info:\n");
  horus::StrAppendToSink(horus::StdoutSink(), "  - Expiration date: ",
                         horus::Iso8601{license_info.ExpirationTimestamp()}, "\n");
  horus::StrAppendToSink(horus::StdoutSink(),
                         "  - Number of lidars: ", license_info.NumberOfLidars(), "\n");

  if (!license_info.AllowedFeatures().empty()) {
    horus::StrAppendToSink(horus::StdoutSink(), "  - Allowed features:\n");
    for (const horus::sdk::LicenseStatus::LicenseInfo::LicenseFeature feature :
         license_info.AllowedFeatures()) {
      horus::StrAppendToSink(horus::StdoutSink(), "    - ", LicenseFeatureToString(feature), "\n");
    }
  } else {
    horus::StrAppendToSink(horus::StdoutSink(), "  - Allowed features: NONE!\n");
  }
}

void PrintSensorHealth(const horus::sdk::SensorHealth& sensor_health_status) {
  const horus::sdk::SensorInfo& sensor_info{sensor_health_status.Info()};

  horus::StrAppendToSink(horus::StdoutSink(), sensor_info.LidarId(), ":");

  if (sensor_health_status.IsNodeUnreachable()) {
    const horus::StringView error{sensor_health_status.NodeUnreachableErrorMessage()};
    horus::StrAppendToSink(horus::StdoutSink(), " Unreachable! ", error, "\n");
    return;
  }

  horus::StrAppendToSink(horus::StdoutSink(), "\n- Frequency: ", sensor_info.MeasuredFrequencyHz(),
                         "Hz\n");

  horus::StrAppendToSink(horus::StdoutSink(), "- Status:\n");
  if (sensor_info.HasStatus(horus::sdk::SensorInfo::SensorStatus::kNoData)) {
    horus::StrAppendToSink(horus::StdoutSink(), "  - No data\n");
  }
  if (sensor_info.HasStatus(horus::sdk::SensorInfo::SensorStatus::kReceivingData)) {
    horus::StrAppendToSink(horus::StdoutSink(), "  - Receiving data\n");
  }
  if (sensor_info.HasStatus(horus::sdk::SensorInfo::SensorStatus::kLowFrequency)) {
    horus::StrAppendToSink(horus::StdoutSink(), "  - Low frequency\n");
  }
  if (sensor_info.HasStatus(horus::sdk::SensorInfo::SensorStatus::kHighFrequency)) {
    horus::StrAppendToSink(horus::StdoutSink(), "  - High frequency\n");
  }
  if (sensor_info.HasStatus(horus::sdk::SensorInfo::SensorStatus::kTilted)) {
    horus::StrAppendToSink(horus::StdoutSink(), "  - Titled\n");
  }
  if (sensor_info.HasStatus(horus::sdk::SensorInfo::SensorStatus::kObstructed)) {
    horus::StrAppendToSink(horus::StdoutSink(), "  - Obstructed\n");
  }
}

void PrintServiceStatuses(horus::Span<const horus::sdk::NodeHealth> service_health_statuses) {
  // This vector will be progressively emptied while we call
  // stringify_service_health_status().
  std::vector<horus::sdk::NodeHealth> service_health_statuses_copy{};
  for (const horus::sdk::NodeHealth& node : service_health_statuses) {
    service_health_statuses_copy.push_back(node);
  }

  const auto stringify_service_health_status = [&service_health_statuses_copy](
                                                   const std::string& service_name,
                                                   const horus::sdk::NodeHealth::Service service) {
    std::vector<horus::sdk::NodeHealth> service_nodes{};
    for (horus::sdk::NodeHealth& node : service_health_statuses_copy) {
      if (node.service != service) {
        continue;
      }
      service_nodes.emplace_back(std::move(node));
    }

    if (service_nodes.empty()) {
      horus::StrAppendToSink(horus::StdoutSink(), service_name, ": No node found!\n");
      return;
    }

    horus::StrAppendToSink(horus::StdoutSink(), service_name, ":\n");
    for (const auto& node_status : service_nodes) {
      std::string str_status{};
      switch (node_status.node_status) {
        case horus::sdk::NodeHealth::Status::kAlive: {
          str_status = "alive";
          break;
        }
        case horus::sdk::NodeHealth::Status::kUnreachable: {
          str_status = "Unreachable!";
          break;
        }
        case horus::sdk::NodeHealth::Status::kUnknownWireValue:
        case horus::sdk::NodeHealth::Status::kUnspecified:
        default: {
          str_status = "unknown";
        }
      }
      horus::StrAppendToSink(horus::StdoutSink(),
                             horus::StrCat("  - ", node_status.node_id, ": ", str_status, "\n"));
    }
    horus::StrAppendToSink(horus::StdoutSink(), "\n");
  };

  stringify_service_health_status("Detection service", horus::sdk::NodeHealth::Service::kDetection);
  stringify_service_health_status("Lidar runner service",
                                  horus::sdk::NodeHealth::Service::kLidarRunner);
  stringify_service_health_status("Notification service",
                                  horus::sdk::NodeHealth::Service::kNotification);
  stringify_service_health_status("Point aggregator service",
                                  horus::sdk::NodeHealth::Service::kPointAggregator);
  stringify_service_health_status("Preprocessing service",
                                  horus::sdk::NodeHealth::Service::kPreprocessing);
  stringify_service_health_status("Project manager service",
                                  horus::sdk::NodeHealth::Service::kProjectManager);
}
}  // namespace

int main(int argc, const char* argv[]) {
  const horus::Span<const char*> args{argv, static_cast<std::size_t>(argc)};

  horus::RpcServices::ServiceResolutionMap service_map;
  if (!horus::ParseArgs(service_map, args)) {
    return 1;
  }
  horus::Sdk sdk{service_map};

  try {
    horus::StrAppendToSink(horus::StdoutSink(), "Sending RPC request...\n");
    const horus::sdk::HealthStatus health_status{sdk.GetHealthStatus({}).Wait()};

    horus::StrAppendToSink(horus::StdoutSink(), "---- License status ----\n");
    PrintLicenseStatus(health_status.GetLicenseStatus());

    horus::StrAppendToSink(horus::StdoutSink(), "\n");
    if (!health_status.HasReadPrivilege()) {
      horus::StrAppendToSink(
          horus::StdoutSink(),
          "Sensor and service statuses not received because license level does not grant the "
          "Read "
          "privilege.");
      return 0;
    }

    horus::StrAppendToSink(horus::StdoutSink(), "---- Lidar statuses ----\n");
    for (const horus::sdk::SensorHealth& sensor_health_status : health_status.SensorStatuses()) {
      PrintSensorHealth(sensor_health_status);
      horus::StrAppendToSink(horus::StdoutSink(), "\n");
    }

    horus::StrAppendToSink(horus::StdoutSink(), "---- Service statuses ----\n");
    PrintServiceStatuses(health_status.ServiceStatuses());
  } catch (const horus::RpcEndpointDisconnectedError&) {
    horus::Log("Cannot connect to the ProjectManager.");
    return 1;
  } catch (const horus::CancellationError&) {
    horus::Log("Request timeout.");
    return 1;
  } catch (const horus::sdk::RpcApplicationError& error) {
    horus::Log("RPC returned an error: ", error.what());
    return 1;
  } catch (const horus::sdk::UnexpectedDataReceivedFromRpcError& error) {
    horus::Log("Something is wrong with the SDK: ", error.what());
    return 1;
  }

  return 0;
}
