/// @file
///
/// Types for subscribing to objects.

#ifndef HORUS_SDK_OBJECTS_H_
#define HORUS_SDK_OBJECTS_H_

#include <functional>

#include "horus/pb/detection_service/detection_pb.h"

namespace horus {
namespace sdk {

/// A request to subscribe to objects.
struct ObjectSubscriptionRequest {
  /// Function to call when an object is detected.
  std::function<void(pb::DetectionEvent&&)> on_detection_results;
};

}  // namespace sdk
}  // namespace horus

#endif  // HORUS_SDK_OBJECTS_H_
