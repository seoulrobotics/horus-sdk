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
