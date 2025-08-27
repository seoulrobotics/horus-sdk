#include "horus/pb/logs/metadata_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

LogMetadata_SourceLocation::LogMetadata_SourceLocation(const LogMetadata_SourceLocation& other) noexcept(false)
    : file_{other.file_}
    , line_{other.line_}
    , set_fields_{other.set_fields_} {}

void LogMetadata_SourceLocation::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, file_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 2, line_);
  }
}

void LogMetadata_SourceLocation::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, file_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint32_t>(reader, line_);
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

LogMetadata_Duration::LogMetadata_Duration(const LogMetadata_Duration& other) noexcept(false)
    : ms_{other.ms_}
    , set_fields_{other.set_fields_} {}

void LogMetadata_Duration::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, ms_);
  }
}

void LogMetadata_Duration::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, ms_);
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

LogMetadata_Timestamp::LogMetadata_Timestamp(const LogMetadata_Timestamp& other) noexcept(false)
    : ms_since_epoch_{other.ms_since_epoch_}
    , set_fields_{other.set_fields_} {}

void LogMetadata_Timestamp::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, ms_since_epoch_);
  }
}

void LogMetadata_Timestamp::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, ms_since_epoch_);
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

LogMetadata::LogMetadata(const LogMetadata& other) noexcept(false)
    : ms_since_epoch_{other.ms_since_epoch_}
    , severity_{other.severity_}
    , node_id_{other.node_id_}
    , encoded_stack_frames_{other.encoded_stack_frames_}
    , stack_frames_modules_{other.stack_frames_modules_}
    , set_fields_{other.set_fields_} {}

void LogMetadata::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, ms_since_epoch_);
  }
  if (set_fields_[1]) {
    SerializeField<LogMetadata_Severity>(writer, /*tag=*/ 2, severity_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, node_id_);
  }
  if (set_fields_[3]) {
    SerializeField<CowSpan<std::uint64_t>, PbDeserFlags::kFixed>(writer, /*tag=*/ 4, encoded_stack_frames_);
  }
  if (set_fields_[4]) {
    SerializeField<CowRepeated<CowBytes>>(writer, /*tag=*/ 5, stack_frames_modules_);
  }
}

void LogMetadata::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, ms_since_epoch_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<LogMetadata_Severity>(reader, severity_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, node_id_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<CowSpan<std::uint64_t>, PbDeserFlags::kFixed>(reader, encoded_stack_frames_);
        set_fields_[3] = true;
        break;
      }
      case 5: {
        DeserializeField<CowRepeated<CowBytes>>(reader, stack_frames_modules_);
        set_fields_[4] = true;
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
