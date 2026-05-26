/// @file
///
/// Types for subscribing to zone-entry/exit events.

#ifndef HORUS_SDK_ZONE_EVENTS_H_
#define HORUS_SDK_ZONE_EVENTS_H_

#include <functional>

#include "horus/pb/detection_service/detection_pb.h"

namespace horus {
namespace sdk {

/// A request to subscribe to zone-entry/exit events emitted by the detection
/// merger.
struct ZoneEventSubscriptionRequest {
  /// Function to call when a batch of zone events for a frame is received.
  std::function<void(pb::ZoneEventList&&)> on_zone_events;
};

}  // namespace sdk
}  // namespace horus

#endif  // HORUS_SDK_ZONE_EVENTS_H_
