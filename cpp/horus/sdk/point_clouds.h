/// @file
///
/// Types for subscribing to point clouds.

#ifndef HORUS_SDK_POINT_CLOUDS_H_
#define HORUS_SDK_POINT_CLOUDS_H_

#include <functional>

#include "horus/pb/point/point_message_pb.h"

namespace horus {
namespace sdk {

/// A request to subscribe to point clouds.
struct PointCloudSubscriptionRequest {
  /// Function to call when a point cloud is received.
  std::function<void(pb::PointFrame&&)> on_point_cloud;
};

}  // namespace sdk
}  // namespace horus

#endif  // HORUS_SDK_POINT_CLOUDS_H_
