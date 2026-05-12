#include "horus/sdk/profiling.h"

#include <chrono>

#include "horus/pb/config/metadata_pb.h"
#include "horus/pb/cow.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/cow_repeated.h"
#include "horus/pb/profiling_pb.h"
#include "horus/types/in_place.h"
#include "horus/types/one_of.h"

// IWYU pragma: no_include <new>

namespace horus {
namespace sdk {
namespace {

/// Parses a `pb::Duration` into a `std::chrono::nanoseconds`.
constexpr std::chrono::nanoseconds ParseProtoDuration(const pb::Duration& duration_pb) noexcept {
  return std::chrono::seconds{duration_pb.seconds()} +
         std::chrono::nanoseconds{duration_pb.nanos()};
}

/// Parses a `pb::Timestamp` into a `std::chrono::system_clock::time_point`.
constexpr std::chrono::system_clock::time_point ParseProtoTimestamp(
    const pb::Timestamp& timestamp_pb) noexcept {
  using std::chrono::system_clock;

  return system_clock::time_point{std::chrono::duration_cast<system_clock::duration>(
      std::chrono::seconds{timestamp_pb.seconds()} +
      std::chrono::nanoseconds{timestamp_pb.nanos()})};
}

}  // namespace

ProfilingSet::ProfilingSet(const pb::ProfilingSet& profiling_set_pb) noexcept
    : profiled_service_{profiling_set_pb.profiled_service()},
      node_id_{profiling_set_pb.node_id().Str()} {
  processing_times_.reserve(profiling_set_pb.processing_times().size());

  for (const auto& proc_time : profiling_set_pb.processing_times()) {
    processing_times_.emplace(proc_time.Ref().key().Str(),
                              ParseProtoDuration(proc_time.Ref().value().duration()));
  }
}

ServiceProfiling::ServiceProfiling(const pb::ServiceProfiling& service_profiling_pb) noexcept
    : details_profiling_set_{service_profiling_pb.details_profiling_set()},
      total_service_latency_{ParseProtoDuration(service_profiling_pb.total_service_latency())},
      idle_time_before_processing_{
          ParseProtoDuration(service_profiling_pb.idle_time_before_processing())},
      intra_component_idle_time_{
          ParseProtoDuration(service_profiling_pb.intra_component_idle_time())},
      node_id_{service_profiling_pb.node_id().Str()} {}

FrameProfiling::FrameProfiling(const pb::FrameProfiling& frame_profiling_pb) noexcept
    : overall_frame_latency_{ParseProtoDuration(frame_profiling_pb.overall_frame_latency())},
      frame_bundling_latency_{ParseProtoDuration(frame_profiling_pb.frame_bundling_latency())},
      preprocessing_overhead_{ParseProtoDuration(frame_profiling_pb.preprocessing_overhead())} {}

BundledFrameProfilingSet::BundledFrameProfilingSet(
    const pb::BundledFrameProfilingSet& bundled_profile_pb) noexcept
    : detection_service_profiling_{bundled_profile_pb.detection_service_profiling()},
      frame_timestamp_{ParseProtoTimestamp(bundled_profile_pb.frame_timestamp())},
      frame_profiling_{bundled_profile_pb.frame_profiling()} {}

PreprocessingFrameProfiling::PreprocessingFrameProfiling(
    const pb::PreprocessingFrameProfiling& preprocessing_profile_pb) noexcept
    : frame_timestamp_{ParseProtoTimestamp(preprocessing_profile_pb.frame_timestamp())},
      lidar_id_{preprocessing_profile_pb.lidar_id().Str()},
      service_profiling_{preprocessing_profile_pb.service_profiling()} {}

DetectionMergerFrameProfiling::DetectionMergerFrameProfiling(
    const pb::DetectionMergerFrameProfiling& merger_profile_pb) noexcept
    : detection_merger_overhead_{ParseProtoDuration(merger_profile_pb.detection_merger_overhead())},
      total_overall_frame_latency_{
          ParseProtoDuration(merger_profile_pb.total_overall_frame_latency())} {}

ProfilingInfo::ProfilingInfo(const pb::ProfilingInfo& profiling_info_pb)
    : profile_type_{profiling_info_pb.profiling_set_case()}, profiling_set_{InPlaceType<void>} {
  switch (profile_type_) {
    case ProfilingSetOneof::kGeneralProfilingSet:
      profiling_set_.Emplace<ProfilingSet>(profiling_info_pb.general_profiling_set());
      break;
    case ProfilingSetOneof::kBundledFrameProfilingSet:
      profiling_set_.Emplace<BundledFrameProfilingSet>(
          profiling_info_pb.bundled_frame_profiling_set());
      break;
    case ProfilingSetOneof::kPreprocessingFrameProfiling:
      profiling_set_.Emplace<PreprocessingFrameProfiling>(
          profiling_info_pb.preprocessing_frame_profiling());
      break;
    case ProfilingSetOneof::kDetectionMergerFrameProfiling:
      profiling_set_.Emplace<DetectionMergerFrameProfiling>(
          profiling_info_pb.detection_merger_frame_profiling());
      break;
    case ProfilingSetOneof::kNotSet:
    default:
      throw std::runtime_error("invalid profiling information received");
  }
}

OneOf<ProfilingSet, void> ProfilingInfo::GeneralProfile() const {
  ProfilingSet const* const profiling_set{profiling_set_.TryAs<ProfilingSet>()};
  if (profiling_set != nullptr) {
    return *profiling_set;
  }
  return OneOf<ProfilingSet, void>{InPlaceType<void>};
}

OneOf<BundledFrameProfilingSet, void> ProfilingInfo::BundledFrameProfile() const {
  BundledFrameProfilingSet const* const profiling_set{
      profiling_set_.TryAs<BundledFrameProfilingSet>()};
  if (profiling_set != nullptr) {
    return *profiling_set;
  }
  return OneOf<BundledFrameProfilingSet, void>{InPlaceType<void>};
}

OneOf<PreprocessingFrameProfiling, void> ProfilingInfo::PreprocessingFrameProfile() const {
  PreprocessingFrameProfiling const* const profiling_set{
      profiling_set_.TryAs<PreprocessingFrameProfiling>()};
  if (profiling_set != nullptr) {
    return *profiling_set;
  }
  return OneOf<PreprocessingFrameProfiling, void>{InPlaceType<void>};
}

OneOf<DetectionMergerFrameProfiling, void> ProfilingInfo::DetectionMergerFrameProfile() const {
  DetectionMergerFrameProfiling const* const profiling_set{
      profiling_set_.TryAs<DetectionMergerFrameProfiling>()};
  if (profiling_set != nullptr) {
    return *profiling_set;
  }
  return OneOf<DetectionMergerFrameProfiling, void>{InPlaceType<void>};
}

}  // namespace sdk
}  // namespace horus
