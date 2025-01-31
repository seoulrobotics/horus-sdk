/// @file
///
/// Generated functions for emitting logs.

#ifndef HORUS_LOGS_FORMAT_H_
#define HORUS_LOGS_FORMAT_H_

#include "horus/pb/logs/message_pb.h"
#include "horus/pb/logs/metadata_pb.h"
#include "horus/strings/str_sink_erased.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

namespace horus {
namespace sdk {
namespace pb {

/// Formats a `duration` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LogMetadata_Duration& duration);

/// Formats a `source_location` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LogMetadata_SourceLocation& source_location);

/// Formats a `timestamp` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LogMetadata_Timestamp& timestamp);

/// Formats the message of a `LogData` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LogData& log_data);

}  // namespace pb
}  // namespace sdk
}  // namespace horus

#endif  // HORUS_LOGS_FORMAT_H_
