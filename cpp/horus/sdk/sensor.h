/// @file
///
/// Types for subscribing to sensor information such as status.

#ifndef HORUS_SDK_SENSOR_H_
#define HORUS_SDK_SENSOR_H_

#include "horus/pb/notification_service/service_pb.h"
#include "horus/pb/preprocessing/messages_pb.h"

namespace horus {
namespace sdk {

/// A request to subscribe to sensor information events.
struct SensorInfoSubscriptionRequest {
  /// Function to call when sensor information is received.
  std::function<void(pb::SensorInfoEvent&&)> on_sensor_info_event;
};

/// A request to subscribe to global sensor occupancy grid information events.
struct OccupancyGridSubscriptionRequest {
  /// Function to call when occupancy grid information is received.
  std::function<void(pb::OccupancyGridEvent&&)> on_occupancy_grid;
};

}  // namespace sdk
}  // namespace horus

#endif  // HORUS_SDK_SENSOR_H_