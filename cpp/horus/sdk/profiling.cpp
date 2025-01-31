#include "horus/sdk/profiling.h"

#include <chrono>
#include <stdexcept>

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
    : profiled_service_{profiling_set_pb.profiled_service()} {
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
          ParseProtoDuration(service_profiling_pb.intra_component_idle_time())} {}

PreprocessingServicePointCloudProfiling::PreprocessingServicePointCloudProfiling(
    const pb::PreprocessingServicePointCloudProfiling&
        preprocessing_service_point_cloud_profiling) noexcept
    : service_profiling_{preprocessing_service_point_cloud_profiling.service_profiling()},
      sending_latency_{ParseProtoDuration(
          preprocessing_service_point_cloud_profiling.point_cloud_sending_latency())} {}

FrameProfiling::FrameProfiling(const pb::FrameProfiling& frame_profiling_pb) noexcept
    : overall_frame_latency_{ParseProtoDuration(frame_profiling_pb.overall_frame_latency())},
      frame_bundling_latency_{ParseProtoDuration(frame_profiling_pb.frame_bundling_latency())} {}

BundledFrameProfilingSet::BundledFrameProfilingSet(
    const pb::BundledFrameProfilingSet& bundled_profile_pb) noexcept
    : detection_service_profiling_{bundled_profile_pb.detection_service_profiling()},
      frame_timestamp_{ParseProtoTimestamp(bundled_profile_pb.frame_timestamp())},
      frame_profiling_{bundled_profile_pb.frame_profiling()} {
  preprocessing_service_point_cloud_profiling_.reserve(
      bundled_profile_pb.preprocessing_service_point_cloud_profiling().size());
  for (const auto& cloud_proc_time :
       bundled_profile_pb.preprocessing_service_point_cloud_profiling()) {
    preprocessing_service_point_cloud_profiling_.emplace(
        cloud_proc_time.Ref().key().Str(),
        sdk::PreprocessingServicePointCloudProfiling{cloud_proc_time.Ref().value()});
  }
}

ProfilingInfo::ProfilingInfo(const pb::ProfilingInfo& profiling_info_pb)
    : profile_type_{profiling_info_pb.profiling_set_case()}, profiling_set_{InPlaceIndex<2>} {
  switch (profile_type_) {
    case ProfilingSetOneof::kGeneralProfilingSet:
      profiling_set_.Emplace<0>(profiling_info_pb.general_profiling_set());
      break;
    case ProfilingSetOneof::kBundledFrameProfilingSet:
      profiling_set_.Emplace<1>(profiling_info_pb.bundled_frame_profiling_set());
      break;
    case ProfilingSetOneof::kNotSet:
    default:
      throw std::runtime_error("invalid profiling information received");
  }
}

OneOf<ProfilingSet, void> ProfilingInfo::GeneralProfile() const {
  if (profiling_set_.Is<0>()) {
    return OneOf<ProfilingSet, void>{InPlaceIndex<0>, profiling_set_.As<ProfilingSet>()};
  }
  return OneOf<ProfilingSet, void>{InPlaceIndex<1>};
}

OneOf<BundledFrameProfilingSet, void> ProfilingInfo::BundledFrameProfile() const {
  if (profiling_set_.Is<1>()) {
    return OneOf<BundledFrameProfilingSet, void>{InPlaceIndex<0>,
                                                 profiling_set_.As<BundledFrameProfilingSet>()};
  }
  return OneOf<BundledFrameProfilingSet, void>{InPlaceIndex<1>};
}

}  // namespace sdk
}  // namespace horus
