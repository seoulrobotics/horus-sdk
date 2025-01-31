/// @file
///
/// Listing of known Horus RPC services.

#ifndef HORUS_RPC_SERVICES_H_
#define HORUS_RPC_SERVICES_H_

#include <cstdint>

#include "horus/types/string_view.h"

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
  };

  /// Returns static information about the `DetectionService`.
  static constexpr ServiceInfo DetectionService() noexcept {
    return ServiceInfo{
        /*   full_name=*/"horus.pb.DetectionService",
        /*          id=*/7U,
        /*  default_ip=*/"127.0.0.1",
        /*default_port=*/40007U,
    };
  }

  /// Returns static information about the `NotificationService`.
  static constexpr ServiceInfo NotificationService() noexcept {
    return ServiceInfo{
        /*   full_name=*/"horus.pb.NotificationService",
        /*          id=*/2U,
        /*  default_ip=*/"127.0.0.1",
        /*default_port=*/40002U,
    };
  }

  /// Returns static information about the `PointAggregatorService`.
  static constexpr ServiceInfo PointAggregatorService() noexcept {
    return ServiceInfo{
        /*   full_name=*/"horus.pb.PointAggregatorService",
        /*          id=*/6U,
        /*  default_ip=*/"127.0.0.1",
        /*default_port=*/40006U,
    };
  }
};

}  // namespace horus

#endif  // HORUS_RPC_SERVICES_H_
