/// @file
///
/// Listing of known Horus RPC services.

#ifndef HORUS_RPC_SERVICES_H_
#define HORUS_RPC_SERVICES_H_

#include <cstdint>

#include "horus/attributes.h"
#include "horus/strings/string_view.h"

namespace horus {

/// Statically known information about RPC services.
class RpcServices final {
 public:
  /// Information about an RPC service.
  struct ServiceInfo {
    /// The full name of the service.
    StringView full_name;
    /// The unique identifier of the service.
    std::uint16_t id;
    /// The default IP of the service.
    StringView default_ip;
    /// The default port of the service.
    std::uint16_t default_port;

    /// Data to build the URL from.
    struct Host {
      /// The IP of the service.
      StringView host;
      /// The port of the service.
      std::uint16_t port;

      /// Constructs a host pointing to a `<given_host>:<given_port>` pair. `given_host` will
      /// *not* be copied.
      constexpr Host(StringView given_host HORUS_LIFETIME_BOUND,
                      std::uint16_t given_port) noexcept
          : host{given_host}, port{given_port} {}

      /// Constructs an entry pointing to `127.0.0.1:<local_port>`.
      constexpr explicit Host(std::uint16_t local_port) noexcept
          : Host{"127.0.0.1", local_port} {}

      /// Constructs an entry pointing to the default configuration of a service.
      constexpr explicit Host(const RpcServices::ServiceInfo& service_info) noexcept
          : Host{service_info.default_ip, service_info.default_port} {}
    };

    /// Wraps ip and port in an Host struct.
    constexpr Host DefaultHost() const noexcept {
      return Host{/*given_host=*/default_ip, /*given_port=*/default_port};
    }
  };

  /// Returns default static information about the `DetectionMergerService`.
  static constexpr ServiceInfo DetectionMergerService() noexcept {
    return ServiceInfo{
        /*   full_name=*/"horus.pb.DetectionMergerService",
        /*          id=*/11U,
        /*  default_ip=*/"127.0.0.1",
        /*default_port=*/40011U,
    };
  }

  /// Returns default static information about the `DetectionService`.
  static constexpr ServiceInfo DetectionService() noexcept {
    return ServiceInfo{
        /*   full_name=*/"horus.pb.DetectionService",
        /*          id=*/7U,
        /*  default_ip=*/"127.0.0.1",
        /*default_port=*/40007U,
    };
  }

  /// Returns default static information about the `NotificationService`.
  static constexpr ServiceInfo NotificationService() noexcept {
    return ServiceInfo{
        /*   full_name=*/"horus.pb.NotificationService",
        /*          id=*/2U,
        /*  default_ip=*/"127.0.0.1",
        /*default_port=*/40002U,
    };
  }

  /// Returns default static information about the `PointAggregatorService`.
  static constexpr ServiceInfo PointAggregatorService() noexcept {
    return ServiceInfo{
        /*   full_name=*/"horus.pb.PointAggregatorService",
        /*          id=*/6U,
        /*  default_ip=*/"127.0.0.1",
        /*default_port=*/40006U,
    };
  }

  /// Returns default static information about the `ProjectManagerService`.
  static constexpr ServiceInfo ProjectManagerService() noexcept {
    return ServiceInfo{
        /*   full_name=*/"horus.pb.ProjectManagerService",
        /*          id=*/3U,
        /*  default_ip=*/"127.0.0.1",
        /*default_port=*/40003U,
    };
  }

  /// Returns default static information about the `StatusService`.
  static constexpr ServiceInfo StatusService() noexcept {
    return ServiceInfo{
        /*   full_name=*/"horus.pb.StatusService",
        /*          id=*/15U,
        /*  default_ip=*/"127.0.0.1",
        /*default_port=*/40015U,
    };
  }

  /// A map from service to URL where it can be resolved.
  struct ServiceResolutionMap {
    /// Entry of the map, representing a Host.
    using Entry = ServiceInfo::Host;

    /// How to resolve the detection_merger service.
    Entry detection_merger{RpcServices::DetectionMergerService().DefaultHost()};

    /// How to resolve the detection service.
    Entry detection{RpcServices::DetectionService().DefaultHost()};

    /// How to resolve the notification service.
    Entry notification{RpcServices::NotificationService().DefaultHost()};

    /// How to resolve the point_aggregator service.
    Entry point_aggregator{RpcServices::PointAggregatorService().DefaultHost()};

    /// How to resolve the project_manager service.
    Entry project_manager{RpcServices::ProjectManagerService().DefaultHost()};

    /// How to resolve the status service.
    Entry status{RpcServices::StatusService().DefaultHost()};
  };
};

}  // namespace horus

#endif  // HORUS_RPC_SERVICES_H_
