#include "horus/pb/resources_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

GpuProcessInfo::GpuProcessInfo(const GpuProcessInfo& other) noexcept(false)
    : pid_{other.pid_}
    , gpu_memory_usage_bytes_{other.gpu_memory_usage_bytes_}
    , set_fields_{other.set_fields_} {}

void GpuProcessInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 1, pid_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 2, gpu_memory_usage_bytes_);
  }
}

void GpuProcessInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint32_t>(reader, pid_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint64_t>(reader, gpu_memory_usage_bytes_);
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

GpuInfo::GpuInfo(const GpuInfo& other) noexcept(false)
    : gpu_id_{other.gpu_id_}
    , gpu_name_{other.gpu_name_}
    , temperature_celsius_{other.temperature_celsius_}
    , utilization_percentage_{other.utilization_percentage_}
    , memory_used_bytes_{other.memory_used_bytes_}
    , memory_total_bytes_{other.memory_total_bytes_}
    , running_processes_{other.running_processes_}
    , set_fields_{other.set_fields_} {}

void GpuInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 1, gpu_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, gpu_name_);
  }
  if (set_fields_[2]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 3, temperature_celsius_);
  }
  if (set_fields_[3]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 4, utilization_percentage_);
  }
  if (set_fields_[4]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 5, memory_used_bytes_);
  }
  if (set_fields_[5]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 6, memory_total_bytes_);
  }
  if (set_fields_[6]) {
    SerializeField<CowRepeated<GpuProcessInfo>>(writer, /*tag=*/ 7, running_processes_);
  }
}

void GpuInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint32_t>(reader, gpu_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, gpu_name_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<std::uint32_t>(reader, temperature_celsius_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<std::uint32_t>(reader, utilization_percentage_);
        set_fields_[3] = true;
        break;
      }
      case 5: {
        DeserializeField<std::uint64_t>(reader, memory_used_bytes_);
        set_fields_[4] = true;
        break;
      }
      case 6: {
        DeserializeField<std::uint64_t>(reader, memory_total_bytes_);
        set_fields_[5] = true;
        break;
      }
      case 7: {
        DeserializeField<CowRepeated<GpuProcessInfo>>(reader, running_processes_);
        set_fields_[6] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

CpuAndMemoryUsage::CpuAndMemoryUsage(const CpuAndMemoryUsage& other) noexcept(false)
    : cpu_usage_percentage_{other.cpu_usage_percentage_}
    , memory_usage_bytes_{other.memory_usage_bytes_}
    , set_fields_{other.set_fields_} {}

void CpuAndMemoryUsage::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 1, cpu_usage_percentage_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 2, memory_usage_bytes_);
  }
}

void CpuAndMemoryUsage::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint32_t>(reader, cpu_usage_percentage_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint64_t>(reader, memory_usage_bytes_);
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

ResourceUsage::ResourceUsage(const ResourceUsage& other) noexcept(false)
    : process_{other.process_}
    , host_{other.host_}
    , gpus_{other.gpus_}
    , cpu_cores_{other.cpu_cores_}
    , set_fields_{other.set_fields_} {}

void ResourceUsage::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CpuAndMemoryUsage>(writer, /*tag=*/ 1, process_);
  }
  if (set_fields_[1]) {
    SerializeField<CpuAndMemoryUsage>(writer, /*tag=*/ 2, host_);
  }
  if (set_fields_[2]) {
    SerializeField<CowRepeated<GpuInfo>>(writer, /*tag=*/ 3, gpus_);
  }
  if (set_fields_[3]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 4, cpu_cores_);
  }
}

void ResourceUsage::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CpuAndMemoryUsage>(reader, process_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CpuAndMemoryUsage>(reader, host_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowRepeated<GpuInfo>>(reader, gpus_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<std::uint32_t>(reader, cpu_cores_);
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

}  // namespace pb
}  // namespace sdk
}  // namespace horus
