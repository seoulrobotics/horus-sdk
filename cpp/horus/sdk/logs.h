/// @file
///
/// Types for subscribing to logs.

#ifndef HORUS_SDK_LOGS_H_
#define HORUS_SDK_LOGS_H_

#include <chrono>
#include <cstdint>
#include <functional>
#include <string>

#include "horus/pb/logs/message_pb.h"
#include "horus/pb/logs/metadata_pb.h"

namespace horus {
namespace sdk {

/// A wrapper around a `pb::LogMessage` with native C++ types.
struct Log {
  /// Type of `id`.
  using Id = std::uint32_t;
  /// Type of `severity`.
  using Severity = pb::LogMetadata::Severity;

  /// The identifier of the message as used in the database.
  Id id;
  /// The formatted log message.
  std::string message;

  /// The severity of the log.
  Severity severity;
  /// The time at which the log was emitted.
  std::chrono::system_clock::time_point time;
  /// The identifier of the node which emitted the log.
  std::string node_id;

  /// Constructs an empty `Log`.
  Log() noexcept : id{0}, severity{Severity::kUnspecified} {}

  /// Constructs a `Log` which refers to a `log_message`.
  ///
  /// @throws std::bad_alloc If the message cannot be copied due to a lack of available memory.
  explicit Log(const pb::LogMessage& log_message) noexcept(false);
};

/// A request to subscribe to logs.
struct LogSubscriptionRequest {
  /// Function to call when a log message is received.
  std::function<void(pb::LogMessage&&)> on_log_message;
};

}  // namespace sdk
}  // namespace horus

#endif  // HORUS_SDK_LOGS_H_
