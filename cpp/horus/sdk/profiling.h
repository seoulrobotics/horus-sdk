/// @file
///
/// Types for subscribing to profiling information.

#ifndef HORUS_SDK_PROFILING_H_
#define HORUS_SDK_PROFILING_H_

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "horus/attributes.h"
#include "horus/pb/profiling_pb.h"
#include "horus/types/one_of.h"

namespace horus {
namespace sdk {

/// A wrapper around a `pb::ProfilingSet` with native C++ types.
class ProfilingSet final {
 public:
  /// Constructs a `ProfilingSet` which refers to a `profiling_set_pb`.
  explicit ProfilingSet(const pb::ProfilingSet& profiling_set_pb) noexcept;

  /// Returns the profiled service.
  constexpr pb::ProfilingSet::ProfiledService ProfiledService() const noexcept {
    return profiled_service_;
  }

  /// Returns the processing times.
  const std::unordered_map<std::string, std::chrono::nanoseconds>& ProcessingTimes() const noexcept
      HORUS_LIFETIME_BOUND {
    return processing_times_;
  }

  /// Returns the node ID of the service that produced this profiling set, or an empty string
  /// if no node ID was set.
  const std::string& NodeId() const noexcept HORUS_LIFETIME_BOUND { return node_id_; }

 private:
  /// The profiled service enum.
  pb::ProfilingSet::ProfiledService profiled_service_;
  /// Map of algorithm names to processing times.
  std::unordered_map<std::string, std::chrono::nanoseconds> processing_times_;
  /// The node ID of the service that produced this profiling set.
  std::string node_id_;
};

/// A wrapper around a `pb::ServiceProfiling` with native C++ types.
class ServiceProfiling final {
 public:
  /// Constructs a `ServiceProfiling` which refers to a `service_profiling_pb`.
  explicit ServiceProfiling(const pb::ServiceProfiling& service_profiling_pb) noexcept;

  /// Returns the details profiling set.
  constexpr const ProfilingSet& DetailsProfilingSet() const noexcept HORUS_LIFETIME_BOUND {
    return details_profiling_set_;
  }

  /// Returns the total service latency.
  constexpr std::chrono::nanoseconds TotalServiceLatency() const noexcept {
    return total_service_latency_;
  }

  /// Returns the idle time before the service starts processing the data, i.e. from
  /// bundle time to the first processing time.
  constexpr std::chrono::nanoseconds IdleTimeBeforeProcessing() const noexcept {
    return idle_time_before_processing_;
  }

  /// Returns the intra idle time of the service (when the data is not being processed
  /// between two processing times).
  constexpr std::chrono::nanoseconds IntraComponentIdleTime() const noexcept {
    return intra_component_idle_time_;
  }

  /// Returns the node ID of the service that generated this profiling data.
  const std::string& NodeId() const noexcept HORUS_LIFETIME_BOUND { return node_id_; }

 private:
  /// The details profiling set.
  ProfilingSet details_profiling_set_;

  /// The total service latency.
  std::chrono::nanoseconds total_service_latency_;

  /// The idle time before the service starts processing the data.
  std::chrono::nanoseconds idle_time_before_processing_;

  /// The intra idle time of the service.
  std::chrono::nanoseconds intra_component_idle_time_;

  /// The node ID of the service.
  std::string node_id_;
};

/// A wrapper around a `pb::FrameProfiling` with native C++ types.
class FrameProfiling final {
 public:
  /// Constructs a `FrameProfiling` which refers to a `frame_profiling_pb`.
  explicit FrameProfiling(const pb::FrameProfiling& frame_profiling_pb) noexcept;

  /// Returns the overall frame latency.
  constexpr std::chrono::nanoseconds OverallFrameLatency() const noexcept {
    return overall_frame_latency_;
  }

  /// Returns the frame bundling latency.
  constexpr std::chrono::nanoseconds FrameBundlingLatency() const noexcept {
    return frame_bundling_latency_;
  }

  /// Returns the preprocessing overhead.
  constexpr std::chrono::nanoseconds PreprocessingOverhead() const noexcept {
    return preprocessing_overhead_;
  }

 private:
  /// The overall frame latency.
  std::chrono::nanoseconds overall_frame_latency_;

  /// The frame bundling latency.
  std::chrono::nanoseconds frame_bundling_latency_;

  /// The preprocessing overhead.
  std::chrono::nanoseconds preprocessing_overhead_;
};

/// A wrapper around a `pb::PreprocessingFrameProfiling` with native C++ types.
///
/// Sent directly from the preprocessing service to the notification service, once per LiDAR frame.
class PreprocessingFrameProfiling final {
 public:
  /// Constructs a `PreprocessingFrameProfiling` which refers to a `preprocessing_profile_pb`.
  explicit PreprocessingFrameProfiling(
      const pb::PreprocessingFrameProfiling& preprocessing_profile_pb) noexcept;

  /// Returns the frame timestamp (average point cloud arrival timestamp).
  constexpr std::chrono::system_clock::time_point FrameTimestamp() const noexcept {
    return frame_timestamp_;
  }

  /// Returns the LiDAR ID this profiling data belongs to.
  const std::string& LidarId() const noexcept HORUS_LIFETIME_BOUND { return lidar_id_; }

  /// Returns the preprocessing service profiling for this frame.
  const ServiceProfiling& ServiceProfile() const noexcept HORUS_LIFETIME_BOUND {
    return service_profiling_;
  }

 private:
  /// The frame timestamp.
  std::chrono::system_clock::time_point frame_timestamp_;

  /// The LiDAR ID.
  std::string lidar_id_;

  /// The preprocessing service profiling for this frame.
  ServiceProfiling service_profiling_;
};

/// A wrapper around a `pb::DetectionMergerFrameProfiling` with native C++ types.
class DetectionMergerFrameProfiling final {
 public:
  /// Constructs a `DetectionMergerFrameProfiling` which refers to a `merger_profile_pb`.
  explicit DetectionMergerFrameProfiling(
      const pb::DetectionMergerFrameProfiling& merger_profile_pb) noexcept;

  /// Returns the detection merger overhead: time from the latest received detection service event
  /// (by its publishing time) to the publishing of the merged event by the detection merger.
  constexpr std::chrono::nanoseconds DetectionMergerOverhead() const noexcept {
    return detection_merger_overhead_;
  }

  /// Returns the total overall frame latency: the maximum overall frame latency across all
  /// detection service events received in this window, plus the detection merger overhead.
  constexpr std::chrono::nanoseconds TotalOverallFrameLatency() const noexcept {
    return total_overall_frame_latency_;
  }

 private:
  /// The detection merger overhead.
  std::chrono::nanoseconds detection_merger_overhead_;

  /// The total overall frame latency.
  std::chrono::nanoseconds total_overall_frame_latency_;
};

/// A wrapper around a `pb::BundledFrameProfilingSet` with native C++ types.
class BundledFrameProfilingSet final {
 public:
  /// Constructs a `BundledFrameProfilingSet` which refers to a `bundled_profile_pb`.
  explicit BundledFrameProfilingSet(
      const pb::BundledFrameProfilingSet& bundled_profile_pb) noexcept;

  /// Returns the detection profiling set.
  const ServiceProfiling& DetectionProfile() const HORUS_LIFETIME_BOUND {
    return detection_service_profiling_;
  }

  /// Returns the timestamp of the bundled frame.
  constexpr std::chrono::system_clock::time_point FrameTimestamp() const noexcept {
    return frame_timestamp_;
  }

 private:
  /// The detection profiling set.
  ServiceProfiling detection_service_profiling_;

  /// The timestamp of the bundled frame.
  std::chrono::system_clock::time_point frame_timestamp_;

  /// The frame profiling set.
  FrameProfiling frame_profiling_;
};

/// A wrapper around a `pb::ProfilingInfo` with native C++ types.
class ProfilingInfo final {
 public:
  /// Alias for the oneof field in the protobuf.
  using ProfilingSetOneof = pb::ProfilingInfo::ProfilingSetOneof;

  /// Constructs a `ProfilingInfo` which refers to a `prof_info_pb`.
  explicit ProfilingInfo(const pb::ProfilingInfo& profiling_info_pb);

  /// Returns the profile type.
  constexpr ProfilingSetOneof ProfileType() const noexcept { return profile_type_; }

  /// Returns either the general profiling set or void if the profiling type is not
  /// kGeneralProfilingSet.
  OneOf<ProfilingSet, void> GeneralProfile() const;

  /// Returns either the bundled frame profiling set or void if the profiling type is not
  /// kBundledFrameProfilingSet.
  OneOf<BundledFrameProfilingSet, void> BundledFrameProfile() const;

  /// Returns either the preprocessing frame profiling or void if the profiling type is not
  /// kPreprocessingFrameProfiling.
  OneOf<PreprocessingFrameProfiling, void> PreprocessingFrameProfile() const;

  /// Returns either the detection merger frame profiling or void if the profiling type is not
  /// kDetectionMergerFrameProfiling.
  OneOf<DetectionMergerFrameProfiling, void> DetectionMergerFrameProfile() const;

 private:
  /// Alias for the variant of the profiling set.
  ///
  /// @note `void` is a placeholder for the default case.
  using ProfilingSetVariant =
      OneOf<ProfilingSet, BundledFrameProfilingSet, PreprocessingFrameProfiling,
            DetectionMergerFrameProfiling, void>;

  /// The profile type.
  ProfilingSetOneof profile_type_;

  /// One of the profiling sets.
  ProfilingSetVariant profiling_set_;
};

/// A request to subscribe to profiling information.
struct ProfilingSubscriptionRequest {
  /// Function to call when profiling information is received.
  std::function<void(pb::ProfilingInfo&&)> on_profiling_info;
};

}  // namespace sdk
}  // namespace horus

#endif  // HORUS_SDK_PROFILING_H_
