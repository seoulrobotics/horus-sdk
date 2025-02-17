#ifndef HORUS_PB_NOTIFICATION_SERVICE_SERVICE_PB_H_
#define HORUS_PB_NOTIFICATION_SERVICE_SERVICE_PB_H_

/// @file
///
/// C++ type definitions for Protobuf messages in `horus/pb/notification_service/service.proto`.
///
/// Generated by `/home/runner/work/horus/horus/common/tools/sdk/pb.ts`.

#include <bitset>
#include <cstdint>
#include <utility>

#include "horus/pb/config/metadata_pb.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/cow_repeated.h"
#include "horus/pb/logs/message_pb.h"
#include "horus/pb/message.h"
#include "horus/pb/profiling_pb.h"
#include "horus/pb/serialize.h"
#include "horus/types/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

// NOLINTBEGIN(readability-identifier-length)

namespace horus {
namespace sdk {
namespace pb {

// MARK: Message forward declarations

class LogMessageRequest;
class LogMessageEvent;
class ProfilingInfoEvent;
class SensorInfo_PoseCorrection;
class SensorInfo;
class SensorInfoEvent;

// MARK: Message declarations

/// No documentation.
///
/// Source: horus/pb/notification_service/service.proto:72:1
class LogMessageRequest final : public PbMessage {
 public:

  /// Constructs a default-initialized `LogMessageRequest`.
  LogMessageRequest() noexcept = default;

  /// Move constructor.
  LogMessageRequest(LogMessageRequest&&) noexcept = default;
  /// Move assignment operator.
  LogMessageRequest& operator=(LogMessageRequest&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit LogMessageRequest(const LogMessageRequest& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  LogMessageRequest& operator=(const LogMessageRequest&) = delete;

  /// Default destructor.
  ~LogMessageRequest() noexcept final = default;

  /// Creates a `LogMessageRequest` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit LogMessageRequest(PbReader& reader) noexcept(false) : PbMessage{} {
    DeserializeFrom(reader);
  }

  /// Serializes the message to `writer`.
  ///
  /// @throws std::bad_alloc If the resulting buffer failed to allocate.
  void SerializeTo(PbWriter& writer) const noexcept(false) final;

  /// Deserializes the message from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  void DeserializeFrom(PbReader& reader) noexcept(false) final;

  /// Returns whether the message is empty.
  bool IsEmpty() const noexcept final { return set_fields_.none(); }

  /// The full name of the message: `horus.pb.LogMessageRequest`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.LogMessageRequest"; }

  /// The full name of the message: `horus.pb.LogMessageRequest`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `log_message` (no 1).
  // -----

  /// The message to log.
  ///
  /// Field no: 1.
  constexpr const LogMessage& log_message() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return log_message_;
  }

  /// If `log_message` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  LogMessage log_message() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(log_message_);
  }

  /// The message to log.
  ///
  /// Field no: 1.
  LogMessage& mutable_log_message() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return log_message_;
  }

  /// Returns whether `log_message` (no 1) is set.
  constexpr bool has_log_message() const noexcept { return set_fields_[0]; }

  /// Clears `log_message` (no 1).
  void clear_log_message() & noexcept {
    set_fields_[0] = false;
    log_message_ = {};
  }

  /// Sets `log_message` (no 1) and returns `*this`.
  LogMessageRequest& set_log_message(LogMessage&& log_message) & noexcept {
    set_fields_[0] = true;
    log_message_ = std::move(log_message);
    return *this;
  }
  /// Sets `log_message` (no 1) and returns `*this`.
  LogMessageRequest&& set_log_message(LogMessage&& log_message) && noexcept {
    return std::move(set_log_message(std::move(log_message)));
  }

 private:
  /// @see log_message()
  LogMessage log_message_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<1> set_fields_;
};

/// No documentation.
///
/// Source: horus/pb/notification_service/service.proto:77:1
class LogMessageEvent final : public PbMessage {
 public:

  /// Constructs a default-initialized `LogMessageEvent`.
  LogMessageEvent() noexcept = default;

  /// Move constructor.
  LogMessageEvent(LogMessageEvent&&) noexcept = default;
  /// Move assignment operator.
  LogMessageEvent& operator=(LogMessageEvent&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit LogMessageEvent(const LogMessageEvent& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  LogMessageEvent& operator=(const LogMessageEvent&) = delete;

  /// Default destructor.
  ~LogMessageEvent() noexcept final = default;

  /// Creates a `LogMessageEvent` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit LogMessageEvent(PbReader& reader) noexcept(false) : PbMessage{} {
    DeserializeFrom(reader);
  }

  /// Serializes the message to `writer`.
  ///
  /// @throws std::bad_alloc If the resulting buffer failed to allocate.
  void SerializeTo(PbWriter& writer) const noexcept(false) final;

  /// Deserializes the message from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  void DeserializeFrom(PbReader& reader) noexcept(false) final;

  /// Returns whether the message is empty.
  bool IsEmpty() const noexcept final { return set_fields_.none(); }

  /// The full name of the message: `horus.pb.LogMessageEvent`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.LogMessageEvent"; }

  /// The full name of the message: `horus.pb.LogMessageEvent`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `log_message` (no 1).
  // -----

  /// The logged message.
  ///
  /// Field no: 1.
  constexpr const LogMessage& log_message() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return log_message_;
  }

  /// If `log_message` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  LogMessage log_message() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(log_message_);
  }

  /// The logged message.
  ///
  /// Field no: 1.
  LogMessage& mutable_log_message() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return log_message_;
  }

  /// Returns whether `log_message` (no 1) is set.
  constexpr bool has_log_message() const noexcept { return set_fields_[0]; }

  /// Clears `log_message` (no 1).
  void clear_log_message() & noexcept {
    set_fields_[0] = false;
    log_message_ = {};
  }

  /// Sets `log_message` (no 1) and returns `*this`.
  LogMessageEvent& set_log_message(LogMessage&& log_message) & noexcept {
    set_fields_[0] = true;
    log_message_ = std::move(log_message);
    return *this;
  }
  /// Sets `log_message` (no 1) and returns `*this`.
  LogMessageEvent&& set_log_message(LogMessage&& log_message) && noexcept {
    return std::move(set_log_message(std::move(log_message)));
  }

 private:
  /// @see log_message()
  LogMessage log_message_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<1> set_fields_;
};

/// No documentation.
///
/// Source: horus/pb/notification_service/service.proto:82:1
class ProfilingInfoEvent final : public PbMessage {
 public:

  /// Constructs a default-initialized `ProfilingInfoEvent`.
  ProfilingInfoEvent() noexcept = default;

  /// Move constructor.
  ProfilingInfoEvent(ProfilingInfoEvent&&) noexcept = default;
  /// Move assignment operator.
  ProfilingInfoEvent& operator=(ProfilingInfoEvent&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit ProfilingInfoEvent(const ProfilingInfoEvent& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  ProfilingInfoEvent& operator=(const ProfilingInfoEvent&) = delete;

  /// Default destructor.
  ~ProfilingInfoEvent() noexcept final = default;

  /// Creates a `ProfilingInfoEvent` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit ProfilingInfoEvent(PbReader& reader) noexcept(false) : PbMessage{} {
    DeserializeFrom(reader);
  }

  /// Serializes the message to `writer`.
  ///
  /// @throws std::bad_alloc If the resulting buffer failed to allocate.
  void SerializeTo(PbWriter& writer) const noexcept(false) final;

  /// Deserializes the message from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  void DeserializeFrom(PbReader& reader) noexcept(false) final;

  /// Returns whether the message is empty.
  bool IsEmpty() const noexcept final { return set_fields_.none(); }

  /// The full name of the message: `horus.pb.ProfilingInfoEvent`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.ProfilingInfoEvent"; }

  /// The full name of the message: `horus.pb.ProfilingInfoEvent`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `profiling_info` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr const ProfilingInfo& profiling_info() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return profiling_info_;
  }

  /// If `profiling_info` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  ProfilingInfo profiling_info() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(profiling_info_);
  }

  /// No documentation.
  ///
  /// Field no: 1.
  ProfilingInfo& mutable_profiling_info() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return profiling_info_;
  }

  /// Returns whether `profiling_info` (no 1) is set.
  constexpr bool has_profiling_info() const noexcept { return set_fields_[0]; }

  /// Clears `profiling_info` (no 1).
  void clear_profiling_info() & noexcept {
    set_fields_[0] = false;
    profiling_info_ = {};
  }

  /// Sets `profiling_info` (no 1) and returns `*this`.
  ProfilingInfoEvent& set_profiling_info(ProfilingInfo&& profiling_info) & noexcept {
    set_fields_[0] = true;
    profiling_info_ = std::move(profiling_info);
    return *this;
  }
  /// Sets `profiling_info` (no 1) and returns `*this`.
  ProfilingInfoEvent&& set_profiling_info(ProfilingInfo&& profiling_info) && noexcept {
    return std::move(set_profiling_info(std::move(profiling_info)));
  }

 private:
  /// @see profiling_info()
  ProfilingInfo profiling_info_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<1> set_fields_;
};

/// No documentation.
///
/// Source: horus/pb/notification_service/service.proto:94:3
class SensorInfo_PoseCorrection final : public PbMessage {
 public:

  /// Constructs a default-initialized `SensorInfo_PoseCorrection`.
  SensorInfo_PoseCorrection() noexcept = default;

  /// Move constructor.
  SensorInfo_PoseCorrection(SensorInfo_PoseCorrection&&) noexcept = default;
  /// Move assignment operator.
  SensorInfo_PoseCorrection& operator=(SensorInfo_PoseCorrection&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit SensorInfo_PoseCorrection(const SensorInfo_PoseCorrection& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  SensorInfo_PoseCorrection& operator=(const SensorInfo_PoseCorrection&) = delete;

  /// Default destructor.
  ~SensorInfo_PoseCorrection() noexcept final = default;

  /// Creates a `SensorInfo_PoseCorrection` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit SensorInfo_PoseCorrection(PbReader& reader) noexcept(false) : PbMessage{} {
    DeserializeFrom(reader);
  }

  /// Serializes the message to `writer`.
  ///
  /// @throws std::bad_alloc If the resulting buffer failed to allocate.
  void SerializeTo(PbWriter& writer) const noexcept(false) final;

  /// Deserializes the message from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  void DeserializeFrom(PbReader& reader) noexcept(false) final;

  /// Returns whether the message is empty.
  bool IsEmpty() const noexcept final { return set_fields_.none(); }

  /// The full name of the message: `horus.pb.SensorInfo.PoseCorrection`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.SensorInfo.PoseCorrection"; }

  /// The full name of the message: `horus.pb.SensorInfo.PoseCorrection`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `translation` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr const Vector3f& translation() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return translation_;
  }

  /// If `translation` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  Vector3f translation() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(translation_);
  }

  /// No documentation.
  ///
  /// Field no: 1.
  Vector3f& mutable_translation() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return translation_;
  }

  /// Returns whether `translation` (no 1) is set.
  constexpr bool has_translation() const noexcept { return set_fields_[0]; }

  /// Clears `translation` (no 1).
  void clear_translation() & noexcept {
    set_fields_[0] = false;
    translation_ = {};
  }

  /// Sets `translation` (no 1) and returns `*this`.
  SensorInfo_PoseCorrection& set_translation(Vector3f&& translation) & noexcept {
    set_fields_[0] = true;
    translation_ = std::move(translation);
    return *this;
  }
  /// Sets `translation` (no 1) and returns `*this`.
  SensorInfo_PoseCorrection&& set_translation(Vector3f&& translation) && noexcept {
    return std::move(set_translation(std::move(translation)));
  }

  // Field `rotation` (no 2).
  // -----

  /// No documentation.
  ///
  /// Field no: 2.
  constexpr const Quaterniond& rotation() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return rotation_;
  }

  /// If `rotation` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 2.
  Quaterniond rotation() && noexcept {
    if (!set_fields_[1]) {
      return {};
    }
    return std::move(rotation_);
  }

  /// No documentation.
  ///
  /// Field no: 2.
  Quaterniond& mutable_rotation() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[1] = true;
    return rotation_;
  }

  /// Returns whether `rotation` (no 2) is set.
  constexpr bool has_rotation() const noexcept { return set_fields_[1]; }

  /// Clears `rotation` (no 2).
  void clear_rotation() & noexcept {
    set_fields_[1] = false;
    rotation_ = {};
  }

  /// Sets `rotation` (no 2) and returns `*this`.
  SensorInfo_PoseCorrection& set_rotation(Quaterniond&& rotation) & noexcept {
    set_fields_[1] = true;
    rotation_ = std::move(rotation);
    return *this;
  }
  /// Sets `rotation` (no 2) and returns `*this`.
  SensorInfo_PoseCorrection&& set_rotation(Quaterniond&& rotation) && noexcept {
    return std::move(set_rotation(std::move(rotation)));
  }

 private:
  /// @see translation()
  Vector3f translation_{};
  /// @see rotation()
  Quaterniond rotation_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<2> set_fields_;
};

/// No documentation.
///
/// Source: horus/pb/notification_service/service.proto:86:1
class SensorInfo final : public PbMessage {
 public:
  /// @see SensorInfo_PoseCorrection
  using PoseCorrection = SensorInfo_PoseCorrection;

  /// Constructs a default-initialized `SensorInfo`.
  SensorInfo() noexcept = default;

  /// Move constructor.
  SensorInfo(SensorInfo&&) noexcept = default;
  /// Move assignment operator.
  SensorInfo& operator=(SensorInfo&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit SensorInfo(const SensorInfo& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  SensorInfo& operator=(const SensorInfo&) = delete;

  /// Default destructor.
  ~SensorInfo() noexcept final = default;

  /// Creates a `SensorInfo` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit SensorInfo(PbReader& reader) noexcept(false) : PbMessage{} {
    DeserializeFrom(reader);
  }

  /// Serializes the message to `writer`.
  ///
  /// @throws std::bad_alloc If the resulting buffer failed to allocate.
  void SerializeTo(PbWriter& writer) const noexcept(false) final;

  /// Deserializes the message from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  void DeserializeFrom(PbReader& reader) noexcept(false) final;

  /// Returns whether the message is empty.
  bool IsEmpty() const noexcept final { return set_fields_.none(); }

  /// The full name of the message: `horus.pb.SensorInfo`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.SensorInfo"; }

  /// The full name of the message: `horus.pb.SensorInfo`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `lidar_id` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr const CowBytes& lidar_id() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return lidar_id_;
  }

  /// If `lidar_id` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  CowBytes lidar_id() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(lidar_id_);
  }

  /// No documentation.
  ///
  /// Field no: 1.
  CowBytes& mutable_lidar_id() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return lidar_id_;
  }

  /// Returns whether `lidar_id` (no 1) is set.
  constexpr bool has_lidar_id() const noexcept { return set_fields_[0]; }

  /// Clears `lidar_id` (no 1).
  void clear_lidar_id() & noexcept {
    set_fields_[0] = false;
    lidar_id_ = {};
  }

  /// Sets `lidar_id` (no 1) and returns `*this`.
  SensorInfo& set_lidar_id(CowBytes&& lidar_id) & noexcept {
    set_fields_[0] = true;
    lidar_id_ = std::move(lidar_id);
    return *this;
  }
  /// Sets `lidar_id` (no 1) and returns `*this`.
  SensorInfo&& set_lidar_id(CowBytes&& lidar_id) && noexcept {
    return std::move(set_lidar_id(std::move(lidar_id)));
  }

  // Field `status` (no 2).
  // -----

  /// Status as uint32, each status is combined using bitwise OR.
  ///
  /// Field no: 2.
  constexpr std::uint32_t status() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return status_;
  }

  /// Status as uint32, each status is combined using bitwise OR.
  ///
  /// Field no: 2.
  std::uint32_t& mutable_status() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[1] = true;
    return status_;
  }

  /// Returns whether `status` (no 2) is set.
  constexpr bool has_status() const noexcept { return set_fields_[1]; }

  /// Clears `status` (no 2).
  void clear_status() & noexcept {
    set_fields_[1] = false;
    status_ = {};
  }

  /// Sets `status` (no 2) and returns `*this`.
  SensorInfo& set_status(std::uint32_t status) & noexcept {
    set_fields_[1] = true;
    status_ = status;
    return *this;
  }
  /// Sets `status` (no 2) and returns `*this`.
  SensorInfo&& set_status(std::uint32_t status) && noexcept {
    return std::move(set_status(status));
  }

  // Field `measured_frequency` (no 3).
  // -----

  /// No documentation.
  ///
  /// Field no: 3.
  constexpr double measured_frequency() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return measured_frequency_;
  }

  /// No documentation.
  ///
  /// Field no: 3.
  double& mutable_measured_frequency() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[2] = true;
    return measured_frequency_;
  }

  /// Returns whether `measured_frequency` (no 3) is set.
  constexpr bool has_measured_frequency() const noexcept { return set_fields_[2]; }

  /// Clears `measured_frequency` (no 3).
  void clear_measured_frequency() & noexcept {
    set_fields_[2] = false;
    measured_frequency_ = {};
  }

  /// Sets `measured_frequency` (no 3) and returns `*this`.
  SensorInfo& set_measured_frequency(double measured_frequency) & noexcept {
    set_fields_[2] = true;
    measured_frequency_ = measured_frequency;
    return *this;
  }
  /// Sets `measured_frequency` (no 3) and returns `*this`.
  SensorInfo&& set_measured_frequency(double measured_frequency) && noexcept {
    return std::move(set_measured_frequency(measured_frequency));
  }

  // Field `pose_correction` (no 4).
  // -----

  /// No documentation.
  ///
  /// Field no: 4.
  constexpr const SensorInfo_PoseCorrection& pose_correction() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return pose_correction_;
  }

  /// If `pose_correction` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 4.
  SensorInfo_PoseCorrection pose_correction() && noexcept {
    if (!set_fields_[3]) {
      return {};
    }
    return std::move(pose_correction_);
  }

  /// No documentation.
  ///
  /// Field no: 4.
  SensorInfo_PoseCorrection& mutable_pose_correction() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[3] = true;
    return pose_correction_;
  }

  /// Returns whether `pose_correction` (no 4) is set.
  constexpr bool has_pose_correction() const noexcept { return set_fields_[3]; }

  /// Clears `pose_correction` (no 4).
  void clear_pose_correction() & noexcept {
    set_fields_[3] = false;
    pose_correction_ = {};
  }

  /// Sets `pose_correction` (no 4) and returns `*this`.
  SensorInfo& set_pose_correction(SensorInfo_PoseCorrection&& pose_correction) & noexcept {
    set_fields_[3] = true;
    pose_correction_ = std::move(pose_correction);
    return *this;
  }
  /// Sets `pose_correction` (no 4) and returns `*this`.
  SensorInfo&& set_pose_correction(SensorInfo_PoseCorrection&& pose_correction) && noexcept {
    return std::move(set_pose_correction(std::move(pose_correction)));
  }

 private:
  /// @see lidar_id()
  CowBytes lidar_id_{};
  /// @see status()
  std::uint32_t status_{};
  /// @see measured_frequency()
  double measured_frequency_{};
  /// @see pose_correction()
  SensorInfo_PoseCorrection pose_correction_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<4> set_fields_;
};

/// No documentation.
///
/// Source: horus/pb/notification_service/service.proto:100:1
class SensorInfoEvent final : public PbMessage {
 public:

  /// Constructs a default-initialized `SensorInfoEvent`.
  SensorInfoEvent() noexcept = default;

  /// Move constructor.
  SensorInfoEvent(SensorInfoEvent&&) noexcept = default;
  /// Move assignment operator.
  SensorInfoEvent& operator=(SensorInfoEvent&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit SensorInfoEvent(const SensorInfoEvent& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  SensorInfoEvent& operator=(const SensorInfoEvent&) = delete;

  /// Default destructor.
  ~SensorInfoEvent() noexcept final = default;

  /// Creates a `SensorInfoEvent` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit SensorInfoEvent(PbReader& reader) noexcept(false) : PbMessage{} {
    DeserializeFrom(reader);
  }

  /// Serializes the message to `writer`.
  ///
  /// @throws std::bad_alloc If the resulting buffer failed to allocate.
  void SerializeTo(PbWriter& writer) const noexcept(false) final;

  /// Deserializes the message from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  void DeserializeFrom(PbReader& reader) noexcept(false) final;

  /// Returns whether the message is empty.
  bool IsEmpty() const noexcept final { return set_fields_.none(); }

  /// The full name of the message: `horus.pb.SensorInfoEvent`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.SensorInfoEvent"; }

  /// The full name of the message: `horus.pb.SensorInfoEvent`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `sensor_info` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr const CowRepeated<SensorInfo>& sensor_info() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return sensor_info_;
  }

  /// If `sensor_info` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  CowRepeated<SensorInfo> sensor_info() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(sensor_info_);
  }

  /// No documentation.
  ///
  /// Field no: 1.
  CowRepeated<SensorInfo>& mutable_sensor_info() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return sensor_info_;
  }

  /// Returns whether `sensor_info` (no 1) is set.
  constexpr bool has_sensor_info() const noexcept { return set_fields_[0]; }

  /// Clears `sensor_info` (no 1).
  void clear_sensor_info() & noexcept {
    set_fields_[0] = false;
    sensor_info_ = {};
  }

  /// Sets `sensor_info` (no 1) and returns `*this`.
  SensorInfoEvent& set_sensor_info(CowRepeated<SensorInfo>&& sensor_info) & noexcept {
    set_fields_[0] = true;
    sensor_info_ = std::move(sensor_info);
    return *this;
  }
  /// Sets `sensor_info` (no 1) and returns `*this`.
  SensorInfoEvent&& set_sensor_info(CowRepeated<SensorInfo>&& sensor_info) && noexcept {
    return std::move(set_sensor_info(std::move(sensor_info)));
  }

 private:
  /// @see sensor_info()
  CowRepeated<SensorInfo> sensor_info_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<1> set_fields_;
};

}  // namespace pb
}  // namespace sdk
}  // namespace horus

// NOLINTEND(readability-identifier-length)

#endif  // HORUS_PB_NOTIFICATION_SERVICE_SERVICE_PB_H_
