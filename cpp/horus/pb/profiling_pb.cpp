#include "horus/pb/profiling_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

ProfilingSet_ProfiledDuration_PerformanceHint::ProfilingSet_ProfiledDuration_PerformanceHint(const ProfilingSet_ProfiledDuration_PerformanceHint& other) noexcept(false)
    : config_parameter_action_{other.config_parameter_action_}
    , config_parameter_name_{other.config_parameter_name_}
    , set_fields_{other.set_fields_} {}

void ProfilingSet_ProfiledDuration_PerformanceHint::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<ProfilingSet_ProfiledDuration_PerformanceHint_ConfigParameterAction>(writer, /*tag=*/ 1, config_parameter_action_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, config_parameter_name_);
  }
}

void ProfilingSet_ProfiledDuration_PerformanceHint::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<ProfilingSet_ProfiledDuration_PerformanceHint_ConfigParameterAction>(reader, config_parameter_action_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, config_parameter_name_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

ProfilingSet_ProfiledDuration::ProfilingSet_ProfiledDuration(const ProfilingSet_ProfiledDuration& other) noexcept(false)
    : duration_{other.duration_}
    , performance_hints_{other.performance_hints_}
    , set_fields_{other.set_fields_} {}

void ProfilingSet_ProfiledDuration::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<Duration>(writer, /*tag=*/ 1, duration_);
  }
  if (set_fields_[1]) {
    SerializeField<CowRepeated<ProfilingSet_ProfiledDuration_PerformanceHint>>(writer, /*tag=*/ 2, performance_hints_);
  }
}

void ProfilingSet_ProfiledDuration::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<Duration>(reader, duration_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowRepeated<ProfilingSet_ProfiledDuration_PerformanceHint>>(reader, performance_hints_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

ProfilingSet_ProfiledDurationMapEntry::ProfilingSet_ProfiledDurationMapEntry(const ProfilingSet_ProfiledDurationMapEntry& other) noexcept(false)
    : key_{other.key_}
    , value_{other.value_}
    , set_fields_{other.set_fields_} {}

void ProfilingSet_ProfiledDurationMapEntry::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, key_);
  }
  if (set_fields_[1]) {
    SerializeField<ProfilingSet_ProfiledDuration>(writer, /*tag=*/ 2, value_);
  }
}

void ProfilingSet_ProfiledDurationMapEntry::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, key_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<ProfilingSet_ProfiledDuration>(reader, value_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

ProfilingSet_ResourceUsage::ProfilingSet_ResourceUsage(const ProfilingSet_ResourceUsage& other) noexcept(false)
    : cpu_usage_percentage_{other.cpu_usage_percentage_}
    , memory_usage_{other.memory_usage_}
    , set_fields_{other.set_fields_} {}

void ProfilingSet_ResourceUsage::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 1, cpu_usage_percentage_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 2, memory_usage_);
  }
}

void ProfilingSet_ResourceUsage::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint32_t>(reader, cpu_usage_percentage_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint64_t>(reader, memory_usage_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

ProfilingSet::ProfilingSet(const ProfilingSet& other) noexcept(false)
    : profiled_service_{other.profiled_service_}
    , processing_times_{other.processing_times_}
    , resource_usage_{other.resource_usage_}
    , set_fields_{other.set_fields_} {}

void ProfilingSet::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<ProfilingSet_ProfiledService>(writer, /*tag=*/ 1, profiled_service_);
  }
  if (set_fields_[1]) {
    SerializeField<CowRepeated<ProfilingSet_ProfiledDurationMapEntry>>(writer, /*tag=*/ 2, processing_times_);
  }
  if (set_fields_[2]) {
    SerializeField<ProfilingSet_ResourceUsage>(writer, /*tag=*/ 3, resource_usage_);
  }
}

void ProfilingSet::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<ProfilingSet_ProfiledService>(reader, profiled_service_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowRepeated<ProfilingSet_ProfiledDurationMapEntry>>(reader, processing_times_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<ProfilingSet_ResourceUsage>(reader, resource_usage_);
        set_fields_[2] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

ServiceProfiling::ServiceProfiling(const ServiceProfiling& other) noexcept(false)
    : details_profiling_set_{other.details_profiling_set_}
    , total_service_latency_{other.total_service_latency_}
    , idle_time_before_processing_{other.idle_time_before_processing_}
    , intra_component_idle_time_{other.intra_component_idle_time_}
    , set_fields_{other.set_fields_} {}

void ServiceProfiling::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<ProfilingSet>(writer, /*tag=*/ 1, details_profiling_set_);
  }
  if (set_fields_[1]) {
    SerializeField<Duration>(writer, /*tag=*/ 2, total_service_latency_);
  }
  if (set_fields_[2]) {
    SerializeField<Duration>(writer, /*tag=*/ 4, idle_time_before_processing_);
  }
  if (set_fields_[3]) {
    SerializeField<Duration>(writer, /*tag=*/ 5, intra_component_idle_time_);
  }
}

void ServiceProfiling::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<ProfilingSet>(reader, details_profiling_set_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<Duration>(reader, total_service_latency_);
        set_fields_[1] = true;
        break;
      }
      case 4: {
        DeserializeField<Duration>(reader, idle_time_before_processing_);
        set_fields_[2] = true;
        break;
      }
      case 5: {
        DeserializeField<Duration>(reader, intra_component_idle_time_);
        set_fields_[3] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

PreprocessingServicePointCloudProfiling::PreprocessingServicePointCloudProfiling(const PreprocessingServicePointCloudProfiling& other) noexcept(false)
    : service_profiling_{other.service_profiling_}
    , point_cloud_sending_latency_{other.point_cloud_sending_latency_}
    , set_fields_{other.set_fields_} {}

void PreprocessingServicePointCloudProfiling::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<ServiceProfiling>(writer, /*tag=*/ 1, service_profiling_);
  }
  if (set_fields_[1]) {
    SerializeField<Duration>(writer, /*tag=*/ 2, point_cloud_sending_latency_);
  }
}

void PreprocessingServicePointCloudProfiling::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<ServiceProfiling>(reader, service_profiling_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<Duration>(reader, point_cloud_sending_latency_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

FrameProfiling::FrameProfiling(const FrameProfiling& other) noexcept(false)
    : overall_frame_latency_{other.overall_frame_latency_}
    , frame_bundling_latency_{other.frame_bundling_latency_}
    , set_fields_{other.set_fields_} {}

void FrameProfiling::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<Duration>(writer, /*tag=*/ 1, overall_frame_latency_);
  }
  if (set_fields_[1]) {
    SerializeField<Duration>(writer, /*tag=*/ 2, frame_bundling_latency_);
  }
}

void FrameProfiling::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<Duration>(reader, overall_frame_latency_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<Duration>(reader, frame_bundling_latency_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

BundledFrameProfilingSet_PreprocessingServicePointCloudProfilingMapEntry::BundledFrameProfilingSet_PreprocessingServicePointCloudProfilingMapEntry(const BundledFrameProfilingSet_PreprocessingServicePointCloudProfilingMapEntry& other) noexcept(false)
    : key_{other.key_}
    , value_{other.value_}
    , set_fields_{other.set_fields_} {}

void BundledFrameProfilingSet_PreprocessingServicePointCloudProfilingMapEntry::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, key_);
  }
  if (set_fields_[1]) {
    SerializeField<PreprocessingServicePointCloudProfiling>(writer, /*tag=*/ 2, value_);
  }
}

void BundledFrameProfilingSet_PreprocessingServicePointCloudProfilingMapEntry::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, key_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<PreprocessingServicePointCloudProfiling>(reader, value_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

BundledFrameProfilingSet::BundledFrameProfilingSet(const BundledFrameProfilingSet& other) noexcept(false)
    : frame_timestamp_{other.frame_timestamp_}
    , frame_profiling_{other.frame_profiling_}
    , detection_service_profiling_{other.detection_service_profiling_}
    , preprocessing_service_point_cloud_profiling_{other.preprocessing_service_point_cloud_profiling_}
    , set_fields_{other.set_fields_} {}

void BundledFrameProfilingSet::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<Timestamp>(writer, /*tag=*/ 3, frame_timestamp_);
  }
  if (set_fields_[1]) {
    SerializeField<FrameProfiling>(writer, /*tag=*/ 4, frame_profiling_);
  }
  if (set_fields_[2]) {
    SerializeField<ServiceProfiling>(writer, /*tag=*/ 5, detection_service_profiling_);
  }
  if (set_fields_[3]) {
    SerializeField<CowRepeated<BundledFrameProfilingSet_PreprocessingServicePointCloudProfilingMapEntry>>(writer, /*tag=*/ 7, preprocessing_service_point_cloud_profiling_);
  }
}

void BundledFrameProfilingSet::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 3: {
        DeserializeField<Timestamp>(reader, frame_timestamp_);
        set_fields_[0] = true;
        break;
      }
      case 4: {
        DeserializeField<FrameProfiling>(reader, frame_profiling_);
        set_fields_[1] = true;
        break;
      }
      case 5: {
        DeserializeField<ServiceProfiling>(reader, detection_service_profiling_);
        set_fields_[2] = true;
        break;
      }
      case 7: {
        DeserializeField<CowRepeated<BundledFrameProfilingSet_PreprocessingServicePointCloudProfilingMapEntry>>(reader, preprocessing_service_point_cloud_profiling_);
        set_fields_[3] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

ProfilingInfo::ProfilingInfo(const ProfilingInfo& other) noexcept(false)
    : general_profiling_set_{other.general_profiling_set_}
    , bundled_frame_profiling_set_{other.bundled_frame_profiling_set_}
    , profiling_set_{other.profiling_set_}
    , set_fields_{other.set_fields_} {}

void ProfilingInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<ProfilingSet>(writer, /*tag=*/ 1, general_profiling_set_);
  }
  if (set_fields_[1]) {
    SerializeField<BundledFrameProfilingSet>(writer, /*tag=*/ 2, bundled_frame_profiling_set_);
  }
}

void ProfilingInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        clear_profiling_set();
        profiling_set_ = ProfilingSetOneof::kGeneralProfilingSet;
        DeserializeField<ProfilingSet>(reader, general_profiling_set_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        clear_profiling_set();
        profiling_set_ = ProfilingSetOneof::kBundledFrameProfilingSet;
        DeserializeField<BundledFrameProfilingSet>(reader, bundled_frame_profiling_set_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus
