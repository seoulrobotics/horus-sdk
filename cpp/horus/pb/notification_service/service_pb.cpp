#include "horus/pb/notification_service/service_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

LogMessageRequest::LogMessageRequest(const LogMessageRequest& other) noexcept(false)
    : log_message_{other.log_message_}
    , set_fields_{other.set_fields_} {}

void LogMessageRequest::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<LogMessage>(writer, /*tag=*/ 1, log_message_);
  }
}

void LogMessageRequest::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<LogMessage>(reader, log_message_);
        set_fields_[0] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

LogMessageEvent::LogMessageEvent(const LogMessageEvent& other) noexcept(false)
    : log_message_{other.log_message_}
    , set_fields_{other.set_fields_} {}

void LogMessageEvent::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<LogMessage>(writer, /*tag=*/ 1, log_message_);
  }
}

void LogMessageEvent::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<LogMessage>(reader, log_message_);
        set_fields_[0] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

ProfilingInfoEvent::ProfilingInfoEvent(const ProfilingInfoEvent& other) noexcept(false)
    : profiling_info_{other.profiling_info_}
    , set_fields_{other.set_fields_} {}

void ProfilingInfoEvent::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<ProfilingInfo>(writer, /*tag=*/ 1, profiling_info_);
  }
}

void ProfilingInfoEvent::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<ProfilingInfo>(reader, profiling_info_);
        set_fields_[0] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

SensorInfo::SensorInfo(const SensorInfo& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , status_{other.status_}
    , measured_frequency_{other.measured_frequency_}
    , set_fields_{other.set_fields_} {}

void SensorInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 2, status_);
  }
  if (set_fields_[2]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 3, measured_frequency_);
  }
}

void SensorInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint32_t>(reader, status_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, measured_frequency_);
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

SensorInfoEvent::SensorInfoEvent(const SensorInfoEvent& other) noexcept(false)
    : sensor_info_{other.sensor_info_}
    , set_fields_{other.set_fields_} {}

void SensorInfoEvent::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowRepeated<SensorInfo>>(writer, /*tag=*/ 1, sensor_info_);
  }
}

void SensorInfoEvent::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowRepeated<SensorInfo>>(reader, sensor_info_);
        set_fields_[0] = true;
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
