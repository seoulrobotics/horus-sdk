#include "horus/pb/logs/logs_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {
namespace logs {

Generic::Generic(const Generic& other) noexcept(false)
    : message_{other.message_}
    , location_{other.location_}
    , set_fields_{other.set_fields_} {}

void Generic::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, message_);
  }
  if (set_fields_[1]) {
    SerializeField<horus::pb::LogMetadata_SourceLocation>(writer, /*tag=*/ 2, location_);
  }
}

void Generic::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, message_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<horus::pb::LogMetadata_SourceLocation>(reader, location_);
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



RpcConnectionError::RpcConnectionError(const RpcConnectionError& other) noexcept(false)
    : target_service_{other.target_service_}
    , target_uri_{other.target_uri_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void RpcConnectionError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, target_service_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, target_uri_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, details_);
  }
}

void RpcConnectionError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, target_service_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, target_uri_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, details_);
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

InvalidProjectName::InvalidProjectName(const InvalidProjectName& other) noexcept(false)
    : name_{other.name_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void InvalidProjectName::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, name_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void InvalidProjectName::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, name_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

ProjectNotFound::ProjectNotFound(const ProjectNotFound& other) noexcept(false)
    : name_{other.name_}
    , set_fields_{other.set_fields_} {}

void ProjectNotFound::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, name_);
  }
}

void ProjectNotFound::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, name_);
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

ProjectAlreadyExists::ProjectAlreadyExists(const ProjectAlreadyExists& other) noexcept(false)
    : name_{other.name_}
    , set_fields_{other.set_fields_} {}

void ProjectAlreadyExists::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, name_);
  }
}

void ProjectAlreadyExists::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, name_);
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

InvalidConfiguration::InvalidConfiguration(const InvalidConfiguration& other) noexcept(false)
    : validation_errors_{other.validation_errors_}
    , set_fields_{other.set_fields_} {}

void InvalidConfiguration::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowRepeated<ValidationError>>(writer, /*tag=*/ 1, validation_errors_);
  }
}

void InvalidConfiguration::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowRepeated<ValidationError>>(reader, validation_errors_);
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

EntityNotFound::EntityNotFound(const EntityNotFound& other) noexcept(false)
    : id_{other.id_}
    , entity_type_{other.entity_type_}
    , set_fields_{other.set_fields_} {}

void EntityNotFound::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, entity_type_);
  }
}

void EntityNotFound::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, entity_type_);
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



RpcDisconnectionError::RpcDisconnectionError(const RpcDisconnectionError& other) noexcept(false)
    : target_service_{other.target_service_}
    , target_uri_{other.target_uri_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void RpcDisconnectionError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, target_service_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, target_uri_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, details_);
  }
}

void RpcDisconnectionError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, target_service_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, target_uri_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, details_);
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

DroppedLogs::DroppedLogs(const DroppedLogs& other) noexcept(false)
    : n_{other.n_}
    , set_fields_{other.set_fields_} {}

void DroppedLogs::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, n_);
  }
}

void DroppedLogs::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, n_);
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

OpenedProject::OpenedProject(const OpenedProject& other) noexcept(false)
    : project_name_{other.project_name_}
    , set_fields_{other.set_fields_} {}

void OpenedProject::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, project_name_);
  }
}

void OpenedProject::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, project_name_);
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

CreatedProject::CreatedProject(const CreatedProject& other) noexcept(false)
    : project_name_{other.project_name_}
    , source_name_{other.source_name_}
    , set_fields_{other.set_fields_} {}

void CreatedProject::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, project_name_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, source_name_);
  }
}

void CreatedProject::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, project_name_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, source_name_);
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



InvalidRequest::InvalidRequest(const InvalidRequest& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void InvalidRequest::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void InvalidRequest::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

SanityCheckError::SanityCheckError(const SanityCheckError& other) noexcept(false)
    : sanity_check_name_{other.sanity_check_name_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void SanityCheckError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, sanity_check_name_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void SanityCheckError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, sanity_check_name_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

BagFailedToOpen::BagFailedToOpen(const BagFailedToOpen& other) noexcept(false)
    : horus_bag_path_{other.horus_bag_path_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void BagFailedToOpen::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, horus_bag_path_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void BagFailedToOpen::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, horus_bag_path_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

BagFailedToClose::BagFailedToClose(const BagFailedToClose& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void BagFailedToClose::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void BagFailedToClose::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

BagConversionFailed::BagConversionFailed(const BagConversionFailed& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void BagConversionFailed::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void BagConversionFailed::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

BagFailedToWrite::BagFailedToWrite(const BagFailedToWrite& other) noexcept(false)
    : name_{other.name_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void BagFailedToWrite::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, name_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void BagFailedToWrite::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, name_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

CalibrationError::CalibrationError(const CalibrationError& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void CalibrationError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void CalibrationError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

ProjectManagerFailedToStartRecording::ProjectManagerFailedToStartRecording(const ProjectManagerFailedToStartRecording& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void ProjectManagerFailedToStartRecording::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void ProjectManagerFailedToStartRecording::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

ProjectManagerFailedToStopRecording::ProjectManagerFailedToStopRecording(const ProjectManagerFailedToStopRecording& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void ProjectManagerFailedToStopRecording::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void ProjectManagerFailedToStopRecording::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

ServiceConnectionTimedOut::ServiceConnectionTimedOut(const ServiceConnectionTimedOut& other) noexcept(false)
    : node_type_{other.node_type_}
    , node_id_{other.node_id_}
    , set_fields_{other.set_fields_} {}

void ServiceConnectionTimedOut::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, node_type_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, node_id_);
  }
}

void ServiceConnectionTimedOut::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, node_type_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, node_id_);
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

BagRecorderAlreadyRunning::BagRecorderAlreadyRunning(const BagRecorderAlreadyRunning& other) noexcept(false)
    : path_{other.path_}
    , set_fields_{other.set_fields_} {}

void BagRecorderAlreadyRunning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
}

void BagRecorderAlreadyRunning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
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

LicenseServerConnectionError::LicenseServerConnectionError(const LicenseServerConnectionError& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void LicenseServerConnectionError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void LicenseServerConnectionError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

LicenseError::LicenseError(const LicenseError& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void LicenseError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void LicenseError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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



LicenseExpiredError::LicenseExpiredError(const LicenseExpiredError& other) noexcept(false)
    : expiration_time_{other.expiration_time_}
    , set_fields_{other.set_fields_} {}

void LicenseExpiredError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<horus::pb::LogMetadata_Timestamp>(writer, /*tag=*/ 1, expiration_time_);
  }
}

void LicenseExpiredError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<horus::pb::LogMetadata_Timestamp>(reader, expiration_time_);
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

LicenseExceededError::LicenseExceededError(const LicenseExceededError& other) noexcept(false)
    : lidar_count_{other.lidar_count_}
    , max_lidar_count_{other.max_lidar_count_}
    , set_fields_{other.set_fields_} {}

void LicenseExceededError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 1, lidar_count_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 2, max_lidar_count_);
  }
}

void LicenseExceededError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint32_t>(reader, lidar_count_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint32_t>(reader, max_lidar_count_);
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

LicenseHostMachineError::LicenseHostMachineError(const LicenseHostMachineError& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void LicenseHostMachineError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void LicenseHostMachineError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

LicensePrivilegeError::LicensePrivilegeError(const LicensePrivilegeError& other) noexcept(false)
    : missing_privilege_{other.missing_privilege_}
    , level_error_{other.level_error_}
    , set_fields_{other.set_fields_} {}

void LicensePrivilegeError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, missing_privilege_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, level_error_);
  }
}

void LicensePrivilegeError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, missing_privilege_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, level_error_);
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



MultipleLicensesWarning::MultipleLicensesWarning(const MultipleLicensesWarning& other) noexcept(false)
    : non_expired_licenses_count_{other.non_expired_licenses_count_}
    , active_license_filename_{other.active_license_filename_}
    , set_fields_{other.set_fields_} {}

void MultipleLicensesWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 1, non_expired_licenses_count_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, active_license_filename_);
  }
}

void MultipleLicensesWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint32_t>(reader, non_expired_licenses_count_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, active_license_filename_);
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

LicenseCurrentLicenseStatusInfo::LicenseCurrentLicenseStatusInfo(const LicenseCurrentLicenseStatusInfo& other) noexcept(false)
    : expiration_time_{other.expiration_time_}
    , max_lidar_count_{other.max_lidar_count_}
    , set_fields_{other.set_fields_} {}

void LicenseCurrentLicenseStatusInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<horus::pb::LogMetadata_Timestamp>(writer, /*tag=*/ 1, expiration_time_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 2, max_lidar_count_);
  }
}

void LicenseCurrentLicenseStatusInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<horus::pb::LogMetadata_Timestamp>(reader, expiration_time_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint32_t>(reader, max_lidar_count_);
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

BagRecordingStoppedInfo::BagRecordingStoppedInfo(const BagRecordingStoppedInfo& other) noexcept(false)
    : path_{other.path_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void BagRecordingStoppedInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void BagRecordingStoppedInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

BagRecordingFailedToStart::BagRecordingFailedToStart(const BagRecordingFailedToStart& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void BagRecordingFailedToStart::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void BagRecordingFailedToStart::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

BagRecordingStartedInfo::BagRecordingStartedInfo(const BagRecordingStartedInfo& other) noexcept(false)
    : path_{other.path_}
    , set_fields_{other.set_fields_} {}

void BagRecordingStartedInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
}

void BagRecordingStartedInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
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



InputSourceChangeRequestedInfo::InputSourceChangeRequestedInfo(const InputSourceChangeRequestedInfo& other) noexcept(false)
    : source_{other.source_}
    , set_fields_{other.set_fields_} {}

void InputSourceChangeRequestedInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, source_);
  }
}

void InputSourceChangeRequestedInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, source_);
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

InputSourceSwitchedInfo::InputSourceSwitchedInfo(const InputSourceSwitchedInfo& other) noexcept(false)
    : source_{other.source_}
    , set_fields_{other.set_fields_} {}

void InputSourceSwitchedInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, source_);
  }
}

void InputSourceSwitchedInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, source_);
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

RpcTimeoutWarning::RpcTimeoutWarning(const RpcTimeoutWarning& other) noexcept(false)
    : request_name_{other.request_name_}
    , endpoint_{other.endpoint_}
    , elapsed_{other.elapsed_}
    , set_fields_{other.set_fields_} {}

void RpcTimeoutWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, request_name_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, endpoint_);
  }
  if (set_fields_[2]) {
    SerializeField<horus::pb::LogMetadata_Duration>(writer, /*tag=*/ 3, elapsed_);
  }
}

void RpcTimeoutWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, request_name_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, endpoint_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<horus::pb::LogMetadata_Duration>(reader, elapsed_);
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

CannotWriteLogFile::CannotWriteLogFile(const CannotWriteLogFile& other) noexcept(false)
    : path_{other.path_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void CannotWriteLogFile::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void CannotWriteLogFile::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

PointCloudParsingFailureWarning::PointCloudParsingFailureWarning(const PointCloudParsingFailureWarning& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void PointCloudParsingFailureWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void PointCloudParsingFailureWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

LidarIsDead::LidarIsDead(const LidarIsDead& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void LidarIsDead::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void LidarIsDead::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

LidarIsNotDeadAnymore::LidarIsNotDeadAnymore(const LidarIsNotDeadAnymore& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void LidarIsNotDeadAnymore::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void LidarIsNotDeadAnymore::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

LidarIsObstructed::LidarIsObstructed(const LidarIsObstructed& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void LidarIsObstructed::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void LidarIsObstructed::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

LidarIsNotObstructedAnymore::LidarIsNotObstructedAnymore(const LidarIsNotObstructedAnymore& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void LidarIsNotObstructedAnymore::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void LidarIsNotObstructedAnymore::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

LidarIsTilted::LidarIsTilted(const LidarIsTilted& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void LidarIsTilted::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void LidarIsTilted::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

LidarIsNotTiltedAnymore::LidarIsNotTiltedAnymore(const LidarIsNotTiltedAnymore& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void LidarIsNotTiltedAnymore::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void LidarIsNotTiltedAnymore::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

LidarHasBeenAutomaticallyRecalibrated::LidarHasBeenAutomaticallyRecalibrated(const LidarHasBeenAutomaticallyRecalibrated& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , translation_{other.translation_}
    , rotation_rpy_{other.rotation_rpy_}
    , set_fields_{other.set_fields_} {}

void LidarHasBeenAutomaticallyRecalibrated::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, translation_);
  }
  if (set_fields_[3]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 4, rotation_rpy_);
  }
}

void LidarHasBeenAutomaticallyRecalibrated::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, translation_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<CowBytes>(reader, rotation_rpy_);
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

LidarAutoCorrectionFailed::LidarAutoCorrectionFailed(const LidarAutoCorrectionFailed& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , failure_count_{other.failure_count_}
    , set_fields_{other.set_fields_} {}

void LidarAutoCorrectionFailed::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
  if (set_fields_[2]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 3, failure_count_);
  }
}

void LidarAutoCorrectionFailed::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<std::uint32_t>(reader, failure_count_);
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

LidarIcpFailed::LidarIcpFailed(const LidarIcpFailed& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , failure_count_{other.failure_count_}
    , set_fields_{other.set_fields_} {}

void LidarIcpFailed::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
  if (set_fields_[2]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 3, failure_count_);
  }
}

void LidarIcpFailed::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<std::uint32_t>(reader, failure_count_);
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

ReceivedFirstDataForLidar::ReceivedFirstDataForLidar(const ReceivedFirstDataForLidar& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void ReceivedFirstDataForLidar::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void ReceivedFirstDataForLidar::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

TerminationFailureError::TerminationFailureError(const TerminationFailureError& other) noexcept(false)
    : component_{other.component_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void TerminationFailureError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, component_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void TerminationFailureError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, component_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

FrameProcessingError::FrameProcessingError(const FrameProcessingError& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void FrameProcessingError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void FrameProcessingError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

ThreadPoolUnavailableError::ThreadPoolUnavailableError(const ThreadPoolUnavailableError& other) noexcept(false)
    : consequence_{other.consequence_}
    , set_fields_{other.set_fields_} {}

void ThreadPoolUnavailableError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, consequence_);
  }
}

void ThreadPoolUnavailableError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, consequence_);
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

InvalidArgument::InvalidArgument(const InvalidArgument& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void InvalidArgument::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void InvalidArgument::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

ComponentInitializationFailureFatal::ComponentInitializationFailureFatal(const ComponentInitializationFailureFatal& other) noexcept(false)
    : component_{other.component_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void ComponentInitializationFailureFatal::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, component_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void ComponentInitializationFailureFatal::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, component_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

UnhandledEnumCaseError::UnhandledEnumCaseError(const UnhandledEnumCaseError& other) noexcept(false)
    : case_name_{other.case_name_}
    , location_{other.location_}
    , set_fields_{other.set_fields_} {}

void UnhandledEnumCaseError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, case_name_);
  }
  if (set_fields_[1]) {
    SerializeField<horus::pb::LogMetadata_SourceLocation>(writer, /*tag=*/ 2, location_);
  }
}

void UnhandledEnumCaseError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, case_name_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<horus::pb::LogMetadata_SourceLocation>(reader, location_);
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



DiscardingDataError::DiscardingDataError(const DiscardingDataError& other) noexcept(false)
    : component_{other.component_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void DiscardingDataError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, component_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void DiscardingDataError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, component_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

DiscardingDataWarning::DiscardingDataWarning(const DiscardingDataWarning& other) noexcept(false)
    : component_{other.component_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void DiscardingDataWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, component_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void DiscardingDataWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, component_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

NothingToProcess::NothingToProcess(const NothingToProcess& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void NothingToProcess::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void NothingToProcess::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

InvalidComponentConfiguration::InvalidComponentConfiguration(const InvalidComponentConfiguration& other) noexcept(false)
    : component_{other.component_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void InvalidComponentConfiguration::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, component_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void InvalidComponentConfiguration::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, component_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

ChannelReceiverNotFoundWarning::ChannelReceiverNotFoundWarning(const ChannelReceiverNotFoundWarning& other) noexcept(false)
    : key_{other.key_}
    , set_fields_{other.set_fields_} {}

void ChannelReceiverNotFoundWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, key_);
  }
}

void ChannelReceiverNotFoundWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, key_);
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

ModelLoadFailure::ModelLoadFailure(const ModelLoadFailure& other) noexcept(false)
    : model_{other.model_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void ModelLoadFailure::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, model_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void ModelLoadFailure::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, model_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

ModelExecutionFailureError::ModelExecutionFailureError(const ModelExecutionFailureError& other) noexcept(false)
    : model_{other.model_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void ModelExecutionFailureError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, model_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void ModelExecutionFailureError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, model_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

ServiceReadyInfo::ServiceReadyInfo(const ServiceReadyInfo& other) noexcept(false)
    : service_{other.service_}
    , set_fields_{other.set_fields_} {}

void ServiceReadyInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, service_);
  }
}

void ServiceReadyInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, service_);
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

ModelPreparingInfo::ModelPreparingInfo(const ModelPreparingInfo& other) noexcept(false)
    : model_{other.model_}
    , set_fields_{other.set_fields_} {}

void ModelPreparingInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, model_);
  }
}

void ModelPreparingInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, model_);
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

ModelInitializedInfo::ModelInitializedInfo(const ModelInitializedInfo& other) noexcept(false)
    : name_{other.name_}
    , set_fields_{other.set_fields_} {}

void ModelInitializedInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, name_);
  }
}

void ModelInitializedInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, name_);
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

ModelInitializationFailureWarning::ModelInitializationFailureWarning(const ModelInitializationFailureWarning& other) noexcept(false)
    : model_{other.model_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void ModelInitializationFailureWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, model_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void ModelInitializationFailureWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, model_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

RosSpinnerStoppedWarning::RosSpinnerStoppedWarning(const RosSpinnerStoppedWarning& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void RosSpinnerStoppedWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void RosSpinnerStoppedWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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



ConfigNodeNotFoundError::ConfigNodeNotFoundError(const ConfigNodeNotFoundError& other) noexcept(false)
    : node_{other.node_}
    , set_fields_{other.set_fields_} {}

void ConfigNodeNotFoundError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, node_);
  }
}

void ConfigNodeNotFoundError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, node_);
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

BagTimestampOutOfOrderError::BagTimestampOutOfOrderError(const BagTimestampOutOfOrderError& other) noexcept(false)
    : prior_timestamp_{other.prior_timestamp_}
    , current_timestamp_{other.current_timestamp_}
    , set_fields_{other.set_fields_} {}

void BagTimestampOutOfOrderError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<horus::pb::LogMetadata_Timestamp>(writer, /*tag=*/ 1, prior_timestamp_);
  }
  if (set_fields_[1]) {
    SerializeField<horus::pb::LogMetadata_Timestamp>(writer, /*tag=*/ 2, current_timestamp_);
  }
}

void BagTimestampOutOfOrderError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<horus::pb::LogMetadata_Timestamp>(reader, prior_timestamp_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<horus::pb::LogMetadata_Timestamp>(reader, current_timestamp_);
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

BagReplayUnexpectedTimestampError::BagReplayUnexpectedTimestampError(const BagReplayUnexpectedTimestampError& other) noexcept(false)
    : expected_timestamp_{other.expected_timestamp_}
    , received_timestamp_{other.received_timestamp_}
    , set_fields_{other.set_fields_} {}

void BagReplayUnexpectedTimestampError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<horus::pb::LogMetadata_Timestamp>(writer, /*tag=*/ 1, expected_timestamp_);
  }
  if (set_fields_[1]) {
    SerializeField<horus::pb::LogMetadata_Timestamp>(writer, /*tag=*/ 2, received_timestamp_);
  }
}

void BagReplayUnexpectedTimestampError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<horus::pb::LogMetadata_Timestamp>(reader, expected_timestamp_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<horus::pb::LogMetadata_Timestamp>(reader, received_timestamp_);
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

WebsocketClosedInfo::WebsocketClosedInfo(const WebsocketClosedInfo& other) noexcept(false)
    : uri_{other.uri_}
    , reason_{other.reason_}
    , set_fields_{other.set_fields_} {}

void WebsocketClosedInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, uri_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, reason_);
  }
}

void WebsocketClosedInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, uri_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, reason_);
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

WebsocketOpenedInfo::WebsocketOpenedInfo(const WebsocketOpenedInfo& other) noexcept(false)
    : uri_{other.uri_}
    , endpoint_is_server_client_{other.endpoint_is_server_client_}
    , set_fields_{other.set_fields_} {}

void WebsocketOpenedInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, uri_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, endpoint_is_server_client_);
  }
}

void WebsocketOpenedInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, uri_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, endpoint_is_server_client_);
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

SubscriberDisconnectedInfo::SubscriberDisconnectedInfo(const SubscriberDisconnectedInfo& other) noexcept(false)
    : name_{other.name_}
    , uri_{other.uri_}
    , set_fields_{other.set_fields_} {}

void SubscriberDisconnectedInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, name_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, uri_);
  }
}

void SubscriberDisconnectedInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, name_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, uri_);
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

ThreadPoolSlowingDownWarning::ThreadPoolSlowingDownWarning(const ThreadPoolSlowingDownWarning& other) noexcept(false)
    : number_{other.number_}
    , set_fields_{other.set_fields_} {}

void ThreadPoolSlowingDownWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, number_);
  }
}

void ThreadPoolSlowingDownWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, number_);
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

ThreadPoolNotRespondingWarning::ThreadPoolNotRespondingWarning(const ThreadPoolNotRespondingWarning& other) noexcept(false)
    : not_responding_for_{other.not_responding_for_}
    , set_fields_{other.set_fields_} {}

void ThreadPoolNotRespondingWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<horus::pb::LogMetadata_Duration>(writer, /*tag=*/ 1, not_responding_for_);
  }
}

void ThreadPoolNotRespondingWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<horus::pb::LogMetadata_Duration>(reader, not_responding_for_);
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





FailedToRemoveStalePointsWarning::FailedToRemoveStalePointsWarning(const FailedToRemoveStalePointsWarning& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void FailedToRemoveStalePointsWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void FailedToRemoveStalePointsWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

IrregularBroadcastingPeriodWarning::IrregularBroadcastingPeriodWarning(const IrregularBroadcastingPeriodWarning& other) noexcept(false)
    : num_irregular_{other.num_irregular_}
    , duration_{other.duration_}
    , mean_deviation_{other.mean_deviation_}
    , set_fields_{other.set_fields_} {}

void IrregularBroadcastingPeriodWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, num_irregular_);
  }
  if (set_fields_[1]) {
    SerializeField<horus::pb::LogMetadata_Duration>(writer, /*tag=*/ 2, duration_);
  }
  if (set_fields_[2]) {
    SerializeField<horus::pb::LogMetadata_Duration>(writer, /*tag=*/ 3, mean_deviation_);
  }
}

void IrregularBroadcastingPeriodWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, num_irregular_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<horus::pb::LogMetadata_Duration>(reader, duration_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<horus::pb::LogMetadata_Duration>(reader, mean_deviation_);
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

ClustererPointsOutOfRangeWarning::ClustererPointsOutOfRangeWarning(const ClustererPointsOutOfRangeWarning& other) noexcept(false)
    : num_points_{other.num_points_}
    , sample_points_{other.sample_points_}
    , set_fields_{other.set_fields_} {}

void ClustererPointsOutOfRangeWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, num_points_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, sample_points_);
  }
}

void ClustererPointsOutOfRangeWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, num_points_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, sample_points_);
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

InternalError::InternalError(const InternalError& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void InternalError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void InternalError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

InternalFatal::InternalFatal(const InternalFatal& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void InternalFatal::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void InternalFatal::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

ServiceStartingInfo::ServiceStartingInfo(const ServiceStartingInfo& other) noexcept(false)
    : service_{other.service_}
    , set_fields_{other.set_fields_} {}

void ServiceStartingInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, service_);
  }
}

void ServiceStartingInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, service_);
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

ConfigNodeNotFoundFatal::ConfigNodeNotFoundFatal(const ConfigNodeNotFoundFatal& other) noexcept(false)
    : node_{other.node_}
    , set_fields_{other.set_fields_} {}

void ConfigNodeNotFoundFatal::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, node_);
  }
}

void ConfigNodeNotFoundFatal::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, node_);
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

ServiceSetupError::ServiceSetupError(const ServiceSetupError& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void ServiceSetupError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void ServiceSetupError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

FilesystemError::FilesystemError(const FilesystemError& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void FilesystemError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void FilesystemError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

InvalidPresetWarning::InvalidPresetWarning(const InvalidPresetWarning& other) noexcept(false)
    : preset_{other.preset_}
    , fallback_name_{other.fallback_name_}
    , reason_{other.reason_}
    , set_fields_{other.set_fields_} {}

void InvalidPresetWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, preset_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, fallback_name_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, reason_);
  }
}

void InvalidPresetWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, preset_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, fallback_name_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, reason_);
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

WebsocketFailedClearPendingError::WebsocketFailedClearPendingError(const WebsocketFailedClearPendingError& other) noexcept(false)
    : exception_{other.exception_}
    , set_fields_{other.set_fields_} {}

void WebsocketFailedClearPendingError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, exception_);
  }
}

void WebsocketFailedClearPendingError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, exception_);
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

WebsocketFailedToStopError::WebsocketFailedToStopError(const WebsocketFailedToStopError& other) noexcept(false)
    : exception_{other.exception_}
    , set_fields_{other.set_fields_} {}

void WebsocketFailedToStopError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, exception_);
  }
}

void WebsocketFailedToStopError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, exception_);
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

WebsocketFailedToCreateError::WebsocketFailedToCreateError(const WebsocketFailedToCreateError& other) noexcept(false)
    : host_{other.host_}
    , port_{other.port_}
    , exception_{other.exception_}
    , set_fields_{other.set_fields_} {}

void WebsocketFailedToCreateError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, host_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 2, port_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, exception_);
  }
}

void WebsocketFailedToCreateError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, host_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint32_t>(reader, port_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, exception_);
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

UnexpectedRpcError::UnexpectedRpcError(const UnexpectedRpcError& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void UnexpectedRpcError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void UnexpectedRpcError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

LicensePollFailed::LicensePollFailed(const LicensePollFailed& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void LicensePollFailed::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void LicensePollFailed::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

LicenseExpiredWarning::LicenseExpiredWarning(const LicenseExpiredWarning& other) noexcept(false)
    : expiration_time_{other.expiration_time_}
    , set_fields_{other.set_fields_} {}

void LicenseExpiredWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<horus::pb::LogMetadata_Timestamp>(writer, /*tag=*/ 1, expiration_time_);
  }
}

void LicenseExpiredWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<horus::pb::LogMetadata_Timestamp>(reader, expiration_time_);
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

LicenseUsageExceededWarning::LicenseUsageExceededWarning(const LicenseUsageExceededWarning& other) noexcept(false)
    : lidar_count_{other.lidar_count_}
    , max_lidar_count_{other.max_lidar_count_}
    , set_fields_{other.set_fields_} {}

void LicenseUsageExceededWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, lidar_count_);
  }
  if (set_fields_[1]) {
    SerializeField<std::int64_t>(writer, /*tag=*/ 2, max_lidar_count_);
  }
}

void LicenseUsageExceededWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, lidar_count_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::int64_t>(reader, max_lidar_count_);
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

StaticThreadPoolSlowTaskWarning::StaticThreadPoolSlowTaskWarning(const StaticThreadPoolSlowTaskWarning& other) noexcept(false)
    : time_elapsed_{other.time_elapsed_}
    , enqueue_location_{other.enqueue_location_}
    , set_fields_{other.set_fields_} {}

void StaticThreadPoolSlowTaskWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<horus::pb::LogMetadata_Duration>(writer, /*tag=*/ 1, time_elapsed_);
  }
  if (set_fields_[1]) {
    SerializeField<horus::pb::LogMetadata_SourceLocation>(writer, /*tag=*/ 2, enqueue_location_);
  }
}

void StaticThreadPoolSlowTaskWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<horus::pb::LogMetadata_Duration>(reader, time_elapsed_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<horus::pb::LogMetadata_SourceLocation>(reader, enqueue_location_);
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

RpcUnsupportedServiceWarning::RpcUnsupportedServiceWarning(const RpcUnsupportedServiceWarning& other) noexcept(false)
    : service_id_{other.service_id_}
    , set_fields_{other.set_fields_} {}

void RpcUnsupportedServiceWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 1, service_id_);
  }
}

void RpcUnsupportedServiceWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint32_t>(reader, service_id_);
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

WebsocketHandlerProblem::WebsocketHandlerProblem(const WebsocketHandlerProblem& other) noexcept(false)
    : what_{other.what_}
    , set_fields_{other.set_fields_} {}

void WebsocketHandlerProblem::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, what_);
  }
}

void WebsocketHandlerProblem::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, what_);
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

WebsocketDeserializeError::WebsocketDeserializeError(const WebsocketDeserializeError& other) noexcept(false)
    : what_{other.what_}
    , set_fields_{other.set_fields_} {}

void WebsocketDeserializeError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, what_);
  }
}

void WebsocketDeserializeError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, what_);
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

WebsocketExpiredRpcEndpointError::WebsocketExpiredRpcEndpointError(const WebsocketExpiredRpcEndpointError& other) noexcept(false)
    : uri_{other.uri_}
    , set_fields_{other.set_fields_} {}

void WebsocketExpiredRpcEndpointError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, uri_);
  }
}

void WebsocketExpiredRpcEndpointError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, uri_);
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

WebsocketQueueOverloadedWarning::WebsocketQueueOverloadedWarning(const WebsocketQueueOverloadedWarning& other) noexcept(false)
    : current_{other.current_}
    , max_{other.max_}
    , set_fields_{other.set_fields_} {}

void WebsocketQueueOverloadedWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, current_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 2, max_);
  }
}

void WebsocketQueueOverloadedWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, current_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint64_t>(reader, max_);
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

RpcFailedToNotifyWarning::RpcFailedToNotifyWarning(const RpcFailedToNotifyWarning& other) noexcept(false)
    : service_{other.service_}
    , uri_{other.uri_}
    , notification_name_{other.notification_name_}
    , what_{other.what_}
    , set_fields_{other.set_fields_} {}

void RpcFailedToNotifyWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, service_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, uri_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, notification_name_);
  }
  if (set_fields_[3]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 4, what_);
  }
}

void RpcFailedToNotifyWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, service_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, uri_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, notification_name_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<CowBytes>(reader, what_);
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

ConfigSubscriptionFailedWarning::ConfigSubscriptionFailedWarning(const ConfigSubscriptionFailedWarning& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void ConfigSubscriptionFailedWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void ConfigSubscriptionFailedWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

ThreadPoolClampedWorkersWarning::ThreadPoolClampedWorkersWarning(const ThreadPoolClampedWorkersWarning& other) noexcept(false)
    : clamped_{other.clamped_}
    , requested_{other.requested_}
    , set_fields_{other.set_fields_} {}

void ThreadPoolClampedWorkersWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, clamped_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 2, requested_);
  }
}

void ThreadPoolClampedWorkersWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, clamped_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint64_t>(reader, requested_);
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





ClampingDataWarning::ClampingDataWarning(const ClampingDataWarning& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void ClampingDataWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void ClampingDataWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

LidarIncompatibleValues::LidarIncompatibleValues(const LidarIncompatibleValues& other) noexcept(false)
    : lidar_ip1_{other.lidar_ip1_}
    , lidar_ip2_{other.lidar_ip2_}
    , value1_{other.value1_}
    , value2_{other.value2_}
    , value_names_{other.value_names_}
    , resolution_{other.resolution_}
    , set_fields_{other.set_fields_} {}

void LidarIncompatibleValues::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_ip1_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_ip2_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, value1_);
  }
  if (set_fields_[3]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 4, value2_);
  }
  if (set_fields_[4]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 5, value_names_);
  }
  if (set_fields_[5]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 6, resolution_);
  }
}

void LidarIncompatibleValues::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_ip1_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_ip2_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, value1_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<CowBytes>(reader, value2_);
        set_fields_[3] = true;
        break;
      }
      case 5: {
        DeserializeField<CowBytes>(reader, value_names_);
        set_fields_[4] = true;
        break;
      }
      case 6: {
        DeserializeField<CowBytes>(reader, resolution_);
        set_fields_[5] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

CannotDetermineContainerIdError::CannotDetermineContainerIdError(const CannotDetermineContainerIdError& other) noexcept(false)
    : container_id_file_path_{other.container_id_file_path_}
    , set_fields_{other.set_fields_} {}

void CannotDetermineContainerIdError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, container_id_file_path_);
  }
}

void CannotDetermineContainerIdError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, container_id_file_path_);
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

StartedLidarDriver::StartedLidarDriver(const StartedLidarDriver& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void StartedLidarDriver::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void StartedLidarDriver::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

CannotStartLidarDriver::CannotStartLidarDriver(const CannotStartLidarDriver& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , details_{other.details_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void CannotStartLidarDriver::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, lidar_name_);
  }
}

void CannotStartLidarDriver::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

StoppedLidarDriver::StoppedLidarDriver(const StoppedLidarDriver& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void StoppedLidarDriver::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void StoppedLidarDriver::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

CannotStopLidarDriver::CannotStopLidarDriver(const CannotStopLidarDriver& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , details_{other.details_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void CannotStopLidarDriver::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, lidar_name_);
  }
}

void CannotStopLidarDriver::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

RestartedLidarDriver::RestartedLidarDriver(const RestartedLidarDriver& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void RestartedLidarDriver::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void RestartedLidarDriver::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

CannotRestartLidarDriver::CannotRestartLidarDriver(const CannotRestartLidarDriver& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , details_{other.details_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void CannotRestartLidarDriver::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, lidar_name_);
  }
}

void CannotRestartLidarDriver::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

RemovedUnusedLidarDriver::RemovedUnusedLidarDriver(const RemovedUnusedLidarDriver& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void RemovedUnusedLidarDriver::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void RemovedUnusedLidarDriver::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

CannotRemoveUnusedLidarDriver::CannotRemoveUnusedLidarDriver(const CannotRemoveUnusedLidarDriver& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , details_{other.details_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void CannotRemoveUnusedLidarDriver::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, lidar_name_);
  }
}

void CannotRemoveUnusedLidarDriver::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

LidarDriverGcFailure::LidarDriverGcFailure(const LidarDriverGcFailure& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void LidarDriverGcFailure::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void LidarDriverGcFailure::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

IdSpaceExhausted::IdSpaceExhausted(const IdSpaceExhausted& other) noexcept(false)
    : prefix_{other.prefix_}
    , set_fields_{other.set_fields_} {}

void IdSpaceExhausted::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, prefix_);
  }
}

void IdSpaceExhausted::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, prefix_);
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

PreprocessingToPointAggregatorPointsSkipped::PreprocessingToPointAggregatorPointsSkipped(const PreprocessingToPointAggregatorPointsSkipped& other) noexcept(false)
    : num_skipped_points_{other.num_skipped_points_}
    , check_interval_{other.check_interval_}
    , set_fields_{other.set_fields_} {}

void PreprocessingToPointAggregatorPointsSkipped::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, num_skipped_points_);
  }
  if (set_fields_[1]) {
    SerializeField<horus::pb::LogMetadata_Duration>(writer, /*tag=*/ 2, check_interval_);
  }
}

void PreprocessingToPointAggregatorPointsSkipped::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, num_skipped_points_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<horus::pb::LogMetadata_Duration>(reader, check_interval_);
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

MinMsgIntervalLessThanThreshold::MinMsgIntervalLessThanThreshold(const MinMsgIntervalLessThanThreshold& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , threshold_{other.threshold_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void MinMsgIntervalLessThanThreshold::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<horus::pb::LogMetadata_Duration>(writer, /*tag=*/ 2, threshold_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, lidar_name_);
  }
}

void MinMsgIntervalLessThanThreshold::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<horus::pb::LogMetadata_Duration>(reader, threshold_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

FailedToCleanupRosWarning::FailedToCleanupRosWarning(const FailedToCleanupRosWarning& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void FailedToCleanupRosWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void FailedToCleanupRosWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

RpcDisconnectedWarning::RpcDisconnectedWarning(const RpcDisconnectedWarning& other) noexcept(false)
    : request_name_{other.request_name_}
    , endpoint_{other.endpoint_}
    , set_fields_{other.set_fields_} {}

void RpcDisconnectedWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, request_name_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, endpoint_);
  }
}

void RpcDisconnectedWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, request_name_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, endpoint_);
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

RpcUnhandledError::RpcUnhandledError(const RpcUnhandledError& other) noexcept(false)
    : request_name_{other.request_name_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void RpcUnhandledError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, request_name_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void RpcUnhandledError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, request_name_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

TimeDiffOutOfRangeWarning::TimeDiffOutOfRangeWarning(const TimeDiffOutOfRangeWarning& other) noexcept(false)
    : prev_time_{other.prev_time_}
    , curr_time_{other.curr_time_}
    , diff_{other.diff_}
    , set_fields_{other.set_fields_} {}

void TimeDiffOutOfRangeWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<horus::pb::LogMetadata_Timestamp>(writer, /*tag=*/ 1, prev_time_);
  }
  if (set_fields_[1]) {
    SerializeField<horus::pb::LogMetadata_Timestamp>(writer, /*tag=*/ 2, curr_time_);
  }
  if (set_fields_[2]) {
    SerializeField<horus::pb::LogMetadata_Duration>(writer, /*tag=*/ 3, diff_);
  }
}

void TimeDiffOutOfRangeWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<horus::pb::LogMetadata_Timestamp>(reader, prev_time_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<horus::pb::LogMetadata_Timestamp>(reader, curr_time_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<horus::pb::LogMetadata_Duration>(reader, diff_);
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

TensorrtLog::TensorrtLog(const TensorrtLog& other) noexcept(false)
    : log_{other.log_}
    , set_fields_{other.set_fields_} {}

void TensorrtLog::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, log_);
  }
}

void TensorrtLog::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, log_);
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





CalibrationMapNotFound::CalibrationMapNotFound(const CalibrationMapNotFound& other) noexcept(false)
    : path_{other.path_}
    , set_fields_{other.set_fields_} {}

void CalibrationMapNotFound::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
}

void CalibrationMapNotFound::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
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

CalibrationMapNotValid::CalibrationMapNotValid(const CalibrationMapNotValid& other) noexcept(false)
    : path_{other.path_}
    , set_fields_{other.set_fields_} {}

void CalibrationMapNotValid::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
}

void CalibrationMapNotValid::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
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

CalibrationMapPathAlreadyExists::CalibrationMapPathAlreadyExists(const CalibrationMapPathAlreadyExists& other) noexcept(false)
    : path_{other.path_}
    , set_fields_{other.set_fields_} {}

void CalibrationMapPathAlreadyExists::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
}

void CalibrationMapPathAlreadyExists::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
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

FailedToSaveCalibrationMap::FailedToSaveCalibrationMap(const FailedToSaveCalibrationMap& other) noexcept(false)
    : path_{other.path_}
    , set_fields_{other.set_fields_} {}

void FailedToSaveCalibrationMap::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
}

void FailedToSaveCalibrationMap::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
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

FailedToRemoveCalibrationMap::FailedToRemoveCalibrationMap(const FailedToRemoveCalibrationMap& other) noexcept(false)
    : path_{other.path_}
    , set_fields_{other.set_fields_} {}

void FailedToRemoveCalibrationMap::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
}

void FailedToRemoveCalibrationMap::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
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

FailedToIterateInDirectory::FailedToIterateInDirectory(const FailedToIterateInDirectory& other) noexcept(false)
    : directory_name_{other.directory_name_}
    , directory_path_{other.directory_path_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void FailedToIterateInDirectory::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, directory_name_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, directory_path_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, details_);
  }
}

void FailedToIterateInDirectory::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, directory_name_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, directory_path_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, details_);
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







BagStreamNotFound::BagStreamNotFound(const BagStreamNotFound& other) noexcept(false)
    : bag_path_{other.bag_path_}
    , stream_id_{other.stream_id_}
    , set_fields_{other.set_fields_} {}

void BagStreamNotFound::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, bag_path_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, stream_id_);
  }
}

void BagStreamNotFound::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, bag_path_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, stream_id_);
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

EvaluationBagStartedInfo::EvaluationBagStartedInfo(const EvaluationBagStartedInfo& other) noexcept(false)
    : bag_path_{other.bag_path_}
    , set_fields_{other.set_fields_} {}

void EvaluationBagStartedInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, bag_path_);
  }
}

void EvaluationBagStartedInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, bag_path_);
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

EvaluationBagFinishedInfo::EvaluationBagFinishedInfo(const EvaluationBagFinishedInfo& other) noexcept(false)
    : bag_path_{other.bag_path_}
    , set_fields_{other.set_fields_} {}

void EvaluationBagFinishedInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, bag_path_);
  }
}

void EvaluationBagFinishedInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, bag_path_);
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

BagNotFound::BagNotFound(const BagNotFound& other) noexcept(false)
    : bag_path_{other.bag_path_}
    , set_fields_{other.set_fields_} {}

void BagNotFound::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, bag_path_);
  }
}

void BagNotFound::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, bag_path_);
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

BuildingPipelineInfo::BuildingPipelineInfo(const BuildingPipelineInfo& other) noexcept(false)
    : pipeline_name_{other.pipeline_name_}
    , set_fields_{other.set_fields_} {}

void BuildingPipelineInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, pipeline_name_);
  }
}

void BuildingPipelineInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, pipeline_name_);
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

BagIsNotEvaluation::BagIsNotEvaluation(const BagIsNotEvaluation& other) noexcept(false)
    : bag_path_{other.bag_path_}
    , set_fields_{other.set_fields_} {}

void BagIsNotEvaluation::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, bag_path_);
  }
}

void BagIsNotEvaluation::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, bag_path_);
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



AutoGroundCalibrationWarning::AutoGroundCalibrationWarning(const AutoGroundCalibrationWarning& other) noexcept(false)
    : failed_lidar_ids_{other.failed_lidar_ids_}
    , set_fields_{other.set_fields_} {}

void AutoGroundCalibrationWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, failed_lidar_ids_);
  }
}

void AutoGroundCalibrationWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 2: {
        DeserializeField<CowBytes>(reader, failed_lidar_ids_);
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







ModelInitInProgressInfo::ModelInitInProgressInfo(const ModelInitInProgressInfo& other) noexcept(false)
    : component_{other.component_}
    , set_fields_{other.set_fields_} {}

void ModelInitInProgressInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, component_);
  }
}

void ModelInitInProgressInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, component_);
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

RpcTimeoutWithResolutionWarning::RpcTimeoutWithResolutionWarning(const RpcTimeoutWithResolutionWarning& other) noexcept(false)
    : request_name_{other.request_name_}
    , endpoint_{other.endpoint_}
    , elapsed_{other.elapsed_}
    , resolution_{other.resolution_}
    , set_fields_{other.set_fields_} {}

void RpcTimeoutWithResolutionWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, request_name_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, endpoint_);
  }
  if (set_fields_[2]) {
    SerializeField<horus::pb::LogMetadata_Duration>(writer, /*tag=*/ 3, elapsed_);
  }
  if (set_fields_[3]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 4, resolution_);
  }
}

void RpcTimeoutWithResolutionWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, request_name_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, endpoint_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<horus::pb::LogMetadata_Duration>(reader, elapsed_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<CowBytes>(reader, resolution_);
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

CalibrationWasCancelledInfo::CalibrationWasCancelledInfo(const CalibrationWasCancelledInfo& other) noexcept(false)
    : calibration_process_name_{other.calibration_process_name_}
    , set_fields_{other.set_fields_} {}

void CalibrationWasCancelledInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, calibration_process_name_);
  }
}

void CalibrationWasCancelledInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, calibration_process_name_);
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

CalibrationMapRecordingFailedToStart::CalibrationMapRecordingFailedToStart(const CalibrationMapRecordingFailedToStart& other) noexcept(false)
    : path_{other.path_}
    , set_fields_{other.set_fields_} {}

void CalibrationMapRecordingFailedToStart::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
}

void CalibrationMapRecordingFailedToStart::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
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

DetectionPipelineRequestedResetInfo::DetectionPipelineRequestedResetInfo(const DetectionPipelineRequestedResetInfo& other) noexcept(false)
    : reason_{other.reason_}
    , set_fields_{other.set_fields_} {}

void DetectionPipelineRequestedResetInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, reason_);
  }
}

void DetectionPipelineRequestedResetInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, reason_);
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

PreprocessingServicePipelineUnavailable::PreprocessingServicePipelineUnavailable(const PreprocessingServicePipelineUnavailable& other) noexcept(false)
    : reason_{other.reason_}
    , set_fields_{other.set_fields_} {}

void PreprocessingServicePipelineUnavailable::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, reason_);
  }
}

void PreprocessingServicePipelineUnavailable::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, reason_);
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

CircularRecordingDisabledWarning::CircularRecordingDisabledWarning(const CircularRecordingDisabledWarning& other) noexcept(false)
    : reason_{other.reason_}
    , set_fields_{other.set_fields_} {}

void CircularRecordingDisabledWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, reason_);
  }
}

void CircularRecordingDisabledWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, reason_);
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







InvalidLidarTimestamp::InvalidLidarTimestamp(const InvalidLidarTimestamp& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , timestamp_{other.timestamp_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void InvalidLidarTimestamp::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<horus::pb::LogMetadata_Timestamp>(writer, /*tag=*/ 2, timestamp_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, lidar_name_);
  }
}

void InvalidLidarTimestamp::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<horus::pb::LogMetadata_Timestamp>(reader, timestamp_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

CalibrationAccumulatingPointsInfo::CalibrationAccumulatingPointsInfo(const CalibrationAccumulatingPointsInfo& other) noexcept(false)
    : time_{other.time_}
    , set_fields_{other.set_fields_} {}

void CalibrationAccumulatingPointsInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<horus::pb::LogMetadata_Duration>(writer, /*tag=*/ 1, time_);
  }
}

void CalibrationAccumulatingPointsInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<horus::pb::LogMetadata_Duration>(reader, time_);
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



FileWriteWarning::FileWriteWarning(const FileWriteWarning& other) noexcept(false)
    : filename_{other.filename_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void FileWriteWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, filename_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void FileWriteWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, filename_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

LicenseForbiddenFeature::LicenseForbiddenFeature(const LicenseForbiddenFeature& other) noexcept(false)
    : feature_name_{other.feature_name_}
    , set_fields_{other.set_fields_} {}

void LicenseForbiddenFeature::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, feature_name_);
  }
}

void LicenseForbiddenFeature::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, feature_name_);
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

FailedToUpdateConfiguration::FailedToUpdateConfiguration(const FailedToUpdateConfiguration& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void FailedToUpdateConfiguration::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void FailedToUpdateConfiguration::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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



ProjectFileInvalidPermissionsError::ProjectFileInvalidPermissionsError(const ProjectFileInvalidPermissionsError& other) noexcept(false)
    : filename_{other.filename_}
    , set_fields_{other.set_fields_} {}

void ProjectFileInvalidPermissionsError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, filename_);
  }
}

void ProjectFileInvalidPermissionsError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, filename_);
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

PipelineSchedulerError::PipelineSchedulerError(const PipelineSchedulerError& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void PipelineSchedulerError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void PipelineSchedulerError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

MultiLidarCalibrationWarning::MultiLidarCalibrationWarning(const MultiLidarCalibrationWarning& other) noexcept(false)
    : failed_lidar_ids_{other.failed_lidar_ids_}
    , set_fields_{other.set_fields_} {}

void MultiLidarCalibrationWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, failed_lidar_ids_);
  }
}

void MultiLidarCalibrationWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, failed_lidar_ids_);
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



OldBagWarning::OldBagWarning(const OldBagWarning& other) noexcept(false)
    : bag_path_{other.bag_path_}
    , version_{other.version_}
    , set_fields_{other.set_fields_} {}

void OldBagWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, bag_path_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint32_t>(writer, /*tag=*/ 2, version_);
  }
}

void OldBagWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, bag_path_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint32_t>(reader, version_);
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

UpgradingBagInfo::UpgradingBagInfo(const UpgradingBagInfo& other) noexcept(false)
    : bag_path_{other.bag_path_}
    , set_fields_{other.set_fields_} {}

void UpgradingBagInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, bag_path_);
  }
}

void UpgradingBagInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, bag_path_);
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

BagCalibrationSaveFailed::BagCalibrationSaveFailed(const BagCalibrationSaveFailed& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void BagCalibrationSaveFailed::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void BagCalibrationSaveFailed::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

BagUpgradeFailed::BagUpgradeFailed(const BagUpgradeFailed& other) noexcept(false)
    : bag_path_{other.bag_path_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void BagUpgradeFailed::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, bag_path_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void BagUpgradeFailed::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, bag_path_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

UnknownLidarError::UnknownLidarError(const UnknownLidarError& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void UnknownLidarError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void UnknownLidarError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

InvalidPointCloudWarning::InvalidPointCloudWarning(const InvalidPointCloudWarning& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , reason_{other.reason_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void InvalidPointCloudWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, reason_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, lidar_name_);
  }
}

void InvalidPointCloudWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, reason_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

LidarIsDroppingPackets::LidarIsDroppingPackets(const LidarIsDroppingPackets& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , num_total_dropped_packets_{other.num_total_dropped_packets_}
    , num_total_expected_packets_{other.num_total_expected_packets_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void LidarIsDroppingPackets::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 2, num_total_dropped_packets_);
  }
  if (set_fields_[2]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 3, num_total_expected_packets_);
  }
  if (set_fields_[3]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 4, lidar_name_);
  }
}

void LidarIsDroppingPackets::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint64_t>(reader, num_total_dropped_packets_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<std::uint64_t>(reader, num_total_expected_packets_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

RemovedInvalidLidarsFromConfigWarning::RemovedInvalidLidarsFromConfigWarning(const RemovedInvalidLidarsFromConfigWarning& other) noexcept(false)
    : backup_path_{other.backup_path_}
    , set_fields_{other.set_fields_} {}

void RemovedInvalidLidarsFromConfigWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, backup_path_);
  }
}

void RemovedInvalidLidarsFromConfigWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, backup_path_);
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

CalibrationMapRecordingFailed::CalibrationMapRecordingFailed(const CalibrationMapRecordingFailed& other) noexcept(false)
    : message_{other.message_}
    , set_fields_{other.set_fields_} {}

void CalibrationMapRecordingFailed::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, message_);
  }
}

void CalibrationMapRecordingFailed::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, message_);
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

DetectionNodeNotFoundError::DetectionNodeNotFoundError(const DetectionNodeNotFoundError& other) noexcept(false)
    : node_name_{other.node_name_}
    , set_fields_{other.set_fields_} {}

void DetectionNodeNotFoundError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, node_name_);
  }
}

void DetectionNodeNotFoundError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, node_name_);
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

CreatedVersionBackupInfo::CreatedVersionBackupInfo(const CreatedVersionBackupInfo& other) noexcept(false)
    : old_horus_version_{other.old_horus_version_}
    , new_horus_version_{other.new_horus_version_}
    , backup_path_{other.backup_path_}
    , set_fields_{other.set_fields_} {}

void CreatedVersionBackupInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, old_horus_version_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, new_horus_version_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, backup_path_);
  }
}

void CreatedVersionBackupInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, old_horus_version_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, new_horus_version_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, backup_path_);
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

PlyFileLoadFailedError::PlyFileLoadFailedError(const PlyFileLoadFailedError& other) noexcept(false)
    : file_path_{other.file_path_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void PlyFileLoadFailedError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, file_path_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void PlyFileLoadFailedError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, file_path_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

HesaiDriverLifecycle::HesaiDriverLifecycle(const HesaiDriverLifecycle& other) noexcept(false)
    : action_{other.action_}
    , lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void HesaiDriverLifecycle::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, action_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_id_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, lidar_name_);
  }
}

void HesaiDriverLifecycle::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, action_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

HesaiDriverError::HesaiDriverError(const HesaiDriverError& other) noexcept(false)
    : details_{other.details_}
    , lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void HesaiDriverError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_id_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, lidar_name_);
  }
}

void HesaiDriverError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

HesaiPacketProcessingFailed::HesaiPacketProcessingFailed(const HesaiPacketProcessingFailed& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , details_{other.details_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void HesaiPacketProcessingFailed::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, lidar_name_);
  }
}

void HesaiPacketProcessingFailed::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

HesaiCorrectionFileError::HesaiCorrectionFileError(const HesaiCorrectionFileError& other) noexcept(false)
    : file_type_{other.file_type_}
    , details_{other.details_}
    , lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void HesaiCorrectionFileError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, file_type_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, lidar_id_);
  }
  if (set_fields_[3]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 4, lidar_name_);
  }
}

void HesaiCorrectionFileError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, file_type_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

HesaiPacketStatistics::HesaiPacketStatistics(const HesaiPacketStatistics& other) noexcept(false)
    : packets_received_{other.packets_received_}
    , packets_published_{other.packets_published_}
    , packets_dropped_{other.packets_dropped_}
    , packets_decode_failed_{other.packets_decode_failed_}
    , success_rate_{other.success_rate_}
    , lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void HesaiPacketStatistics::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, packets_received_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 2, packets_published_);
  }
  if (set_fields_[2]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 3, packets_dropped_);
  }
  if (set_fields_[3]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 4, packets_decode_failed_);
  }
  if (set_fields_[4]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 5, success_rate_);
  }
  if (set_fields_[5]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 6, lidar_id_);
  }
  if (set_fields_[6]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 7, lidar_name_);
  }
}

void HesaiPacketStatistics::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, packets_received_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint64_t>(reader, packets_published_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<std::uint64_t>(reader, packets_dropped_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<std::uint64_t>(reader, packets_decode_failed_);
        set_fields_[3] = true;
        break;
      }
      case 5: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, success_rate_);
        set_fields_[4] = true;
        break;
      }
      case 6: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[5] = true;
        break;
      }
      case 7: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

PlyFileWriteFailedError::PlyFileWriteFailedError(const PlyFileWriteFailedError& other) noexcept(false)
    : file_path_{other.file_path_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void PlyFileWriteFailedError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, file_path_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void PlyFileWriteFailedError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, file_path_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

ProjectSaveError::ProjectSaveError(const ProjectSaveError& other) noexcept(false)
    : error_message_{other.error_message_}
    , set_fields_{other.set_fields_} {}

void ProjectSaveError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, error_message_);
  }
}

void ProjectSaveError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, error_message_);
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

SaveStaticEnvironmentSuccess::SaveStaticEnvironmentSuccess(const SaveStaticEnvironmentSuccess& other) noexcept(false)
    : path_{other.path_}
    , set_fields_{other.set_fields_} {}

void SaveStaticEnvironmentSuccess::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
}

void SaveStaticEnvironmentSuccess::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
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

SaveStaticEnvironmentFailed::SaveStaticEnvironmentFailed(const SaveStaticEnvironmentFailed& other) noexcept(false)
    : path_{other.path_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void SaveStaticEnvironmentFailed::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void SaveStaticEnvironmentFailed::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

LoadStaticEnvironmentSuccess::LoadStaticEnvironmentSuccess(const LoadStaticEnvironmentSuccess& other) noexcept(false)
    : path_{other.path_}
    , set_fields_{other.set_fields_} {}

void LoadStaticEnvironmentSuccess::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
}

void LoadStaticEnvironmentSuccess::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
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

LoadStaticEnvironmentFailed::LoadStaticEnvironmentFailed(const LoadStaticEnvironmentFailed& other) noexcept(false)
    : path_{other.path_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void LoadStaticEnvironmentFailed::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, path_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
}

void LoadStaticEnvironmentFailed::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, path_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
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

AttemptToInjectInvalidLidarIdWarning::AttemptToInjectInvalidLidarIdWarning(const AttemptToInjectInvalidLidarIdWarning& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , set_fields_{other.set_fields_} {}

void AttemptToInjectInvalidLidarIdWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
}

void AttemptToInjectInvalidLidarIdWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
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

ResetBundledPacketDueToUnexpectedPacket::ResetBundledPacketDueToUnexpectedPacket(const ResetBundledPacketDueToUnexpectedPacket& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void ResetBundledPacketDueToUnexpectedPacket::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void ResetBundledPacketDueToUnexpectedPacket::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

PacketBundlerDroppedPacketsWarning::PacketBundlerDroppedPacketsWarning(const PacketBundlerDroppedPacketsWarning& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , num_dropped_{other.num_dropped_}
    , duration_{other.duration_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void PacketBundlerDroppedPacketsWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 2, num_dropped_);
  }
  if (set_fields_[2]) {
    SerializeField<horus::pb::LogMetadata_Duration>(writer, /*tag=*/ 3, duration_);
  }
  if (set_fields_[3]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 4, lidar_name_);
  }
}

void PacketBundlerDroppedPacketsWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint64_t>(reader, num_dropped_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<horus::pb::LogMetadata_Duration>(reader, duration_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

PacketBundlerFrameJumpWarning::PacketBundlerFrameJumpWarning(const PacketBundlerFrameJumpWarning& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , frame_id_{other.frame_id_}
    , next_frame_id_{other.next_frame_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void PacketBundlerFrameJumpWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 2, frame_id_);
  }
  if (set_fields_[2]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 3, next_frame_id_);
  }
  if (set_fields_[3]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 4, lidar_name_);
  }
}

void PacketBundlerFrameJumpWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint64_t>(reader, frame_id_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<std::uint64_t>(reader, next_frame_id_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

LidarCorrectionLoadingSuccess::LidarCorrectionLoadingSuccess(const LidarCorrectionLoadingSuccess& other) noexcept(false)
    : correction_type_{other.correction_type_}
    , lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void LidarCorrectionLoadingSuccess::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, correction_type_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_id_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, lidar_name_);
  }
}

void LidarCorrectionLoadingSuccess::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, correction_type_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

LidarCorrectionLoadingFailure::LidarCorrectionLoadingFailure(const LidarCorrectionLoadingFailure& other) noexcept(false)
    : correction_type_{other.correction_type_}
    , details_{other.details_}
    , lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void LidarCorrectionLoadingFailure::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, correction_type_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, details_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, lidar_id_);
  }
  if (set_fields_[3]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 4, lidar_name_);
  }
}

void LidarCorrectionLoadingFailure::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, correction_type_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, details_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

HesaiPacketStatisticsLidar::HesaiPacketStatisticsLidar(const HesaiPacketStatisticsLidar& other) noexcept(false)
    : packets_received_{other.packets_received_}
    , packets_published_{other.packets_published_}
    , packets_dropped_{other.packets_dropped_}
    , packets_decode_failed_{other.packets_decode_failed_}
    , success_rate_{other.success_rate_}
    , lidar_id_{other.lidar_id_}
    , set_fields_{other.set_fields_} {}

void HesaiPacketStatisticsLidar::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, packets_received_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 2, packets_published_);
  }
  if (set_fields_[2]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 3, packets_dropped_);
  }
  if (set_fields_[3]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 4, packets_decode_failed_);
  }
  if (set_fields_[4]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 5, success_rate_);
  }
  if (set_fields_[5]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 6, lidar_id_);
  }
}

void HesaiPacketStatisticsLidar::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, packets_received_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint64_t>(reader, packets_published_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<std::uint64_t>(reader, packets_dropped_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<std::uint64_t>(reader, packets_decode_failed_);
        set_fields_[3] = true;
        break;
      }
      case 5: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, success_rate_);
        set_fields_[4] = true;
        break;
      }
      case 6: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[5] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

LidarTiltDetectionAlignedToCalibrationMapInfo::LidarTiltDetectionAlignedToCalibrationMapInfo(const LidarTiltDetectionAlignedToCalibrationMapInfo& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , set_fields_{other.set_fields_} {}

void LidarTiltDetectionAlignedToCalibrationMapInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
}

void LidarTiltDetectionAlignedToCalibrationMapInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
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

LidarTiltDetectionMisalignedToCalibrationMapWarning::LidarTiltDetectionMisalignedToCalibrationMapWarning(const LidarTiltDetectionMisalignedToCalibrationMapWarning& other) noexcept(false)
    : lidar_id_{other.lidar_id_}
    , lidar_name_{other.lidar_name_}
    , angle_{other.angle_}
    , distance_{other.distance_}
    , set_fields_{other.set_fields_} {}

void LidarTiltDetectionMisalignedToCalibrationMapWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, lidar_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, lidar_name_);
  }
  if (set_fields_[2]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 3, angle_);
  }
  if (set_fields_[3]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 4, distance_);
  }
}

void LidarTiltDetectionMisalignedToCalibrationMapWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, lidar_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, lidar_name_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, angle_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, distance_);
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



RecoveredCarIdsInfo::RecoveredCarIdsInfo(const RecoveredCarIdsInfo& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void RecoveredCarIdsInfo::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void RecoveredCarIdsInfo::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

FailedToRecoverCarIds::FailedToRecoverCarIds(const FailedToRecoverCarIds& other) noexcept(false)
    : details_{other.details_}
    , set_fields_{other.set_fields_} {}

void FailedToRecoverCarIds::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, details_);
  }
}

void FailedToRecoverCarIds::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, details_);
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

FailedToEmplaceRecoveredCarId::FailedToEmplaceRecoveredCarId(const FailedToEmplaceRecoveredCarId& other) noexcept(false)
    : object_id_{other.object_id_}
    , set_fields_{other.set_fields_} {}

void FailedToEmplaceRecoveredCarId::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, object_id_);
  }
}

void FailedToEmplaceRecoveredCarId::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, object_id_);
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

PersistentStorageError::PersistentStorageError(const PersistentStorageError& other) noexcept(false)
    : operation_{other.operation_}
    , filepath_{other.filepath_}
    , set_fields_{other.set_fields_} {}

void PersistentStorageError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, operation_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, filepath_);
  }
}

void PersistentStorageError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, operation_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, filepath_);
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

TrackCapacityExceededWarning::TrackCapacityExceededWarning(const TrackCapacityExceededWarning& other) noexcept(false)
    : attempted_count_{other.attempted_count_}
    , max_tracks_{other.max_tracks_}
    , set_fields_{other.set_fields_} {}

void TrackCapacityExceededWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, attempted_count_);
  }
  if (set_fields_[1]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 2, max_tracks_);
  }
}

void TrackCapacityExceededWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, attempted_count_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<std::uint64_t>(reader, max_tracks_);
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

TrackerStatePathUnavailableWarning::TrackerStatePathUnavailableWarning(const TrackerStatePathUnavailableWarning& other) noexcept(false)
    : project_name_{other.project_name_}
    , set_fields_{other.set_fields_} {}

void TrackerStatePathUnavailableWarning::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, project_name_);
  }
}

void TrackerStatePathUnavailableWarning::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, project_name_);
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

TrackerIdRecoveryFailedError::TrackerIdRecoveryFailedError(const TrackerIdRecoveryFailedError& other) noexcept(false)
    : id_{other.id_}
    , error_message_{other.error_message_}
    , set_fields_{other.set_fields_} {}

void TrackerIdRecoveryFailedError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, error_message_);
  }
}

void TrackerIdRecoveryFailedError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, error_message_);
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

TrackerIdFastForwardFailedError::TrackerIdFastForwardFailedError(const TrackerIdFastForwardFailedError& other) noexcept(false)
    : target_id_{other.target_id_}
    , error_message_{other.error_message_}
    , set_fields_{other.set_fields_} {}

void TrackerIdFastForwardFailedError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, target_id_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, error_message_);
  }
}

void TrackerIdFastForwardFailedError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, target_id_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, error_message_);
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

CircularRecordingSnapshotCreated::CircularRecordingSnapshotCreated(const CircularRecordingSnapshotCreated& other) noexcept(false)
    : message_count_{other.message_count_}
    , actual_duration_seconds_{other.actual_duration_seconds_}
    , max_buffer_duration_seconds_{other.max_buffer_duration_seconds_}
    , set_fields_{other.set_fields_} {}

void CircularRecordingSnapshotCreated::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<std::uint64_t>(writer, /*tag=*/ 1, message_count_);
  }
  if (set_fields_[1]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 2, actual_duration_seconds_);
  }
  if (set_fields_[2]) {
    SerializeField<double, PbDeserFlags::kFixed>(writer, /*tag=*/ 3, max_buffer_duration_seconds_);
  }
}

void CircularRecordingSnapshotCreated::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<std::uint64_t>(reader, message_count_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, actual_duration_seconds_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<double, PbDeserFlags::kFixed>(reader, max_buffer_duration_seconds_);
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

CircularRecordingFileOperationError::CircularRecordingFileOperationError(const CircularRecordingFileOperationError& other) noexcept(false)
    : operation_{other.operation_}
    , file_path_{other.file_path_}
    , details_{other.details_}
    , set_fields_{other.set_fields_} {}

void CircularRecordingFileOperationError::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 1, operation_);
  }
  if (set_fields_[1]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 2, file_path_);
  }
  if (set_fields_[2]) {
    SerializeField<CowBytes>(writer, /*tag=*/ 3, details_);
  }
}

void CircularRecordingFileOperationError::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<CowBytes>(reader, operation_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<CowBytes>(reader, file_path_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        DeserializeField<CowBytes>(reader, details_);
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

}  // namespace logs
}  // namespace pb
}  // namespace sdk
}  // namespace horus
