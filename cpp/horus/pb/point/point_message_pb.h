#ifndef HORUS_PB_POINT_POINT_MESSAGE_PB_H_
#define HORUS_PB_POINT_POINT_MESSAGE_PB_H_

/// @file
///
/// C++ type definitions for Protobuf messages in `horus/pb/point/point_message.proto`.
///
/// Generated by `/home/runner/work/horus/horus/common/tools/sdk/pb.ts`.

#include <bitset>
#include <cstdint>
#include <utility>

#include "horus/pb/config/metadata_pb.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/cow_repeated.h"
#include "horus/pb/cow_span.h"
#include "horus/pb/message.h"
#include "horus/pb/serialize.h"
#include "horus/pb/types.h"
#include "horus/types/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

// NOLINTBEGIN(readability-identifier-length)

namespace horus {
namespace sdk {
namespace pb {

// MARK: Enum declarations

/// PointAttribute enum
///  @note Each enum value should be a power of two.
///  @note No attributes should have the same value.
///  @note The enums are sorted by priority so that the first element has the
///  highest priority. ATTRIBUTE_NONE is a special case and can not be combined
///  with others.
///  @note For visualization to be applied correctly, any update to this enum
///  also needs to be applied to the fragment shader
///  `attribute-pointcloud-fragment.glsl`.
///
/// Source: horus/pb/point/point_message.proto:16:1
enum class PointAttribute : PbEnum {  // NOLINT(*-enum-size)
  /// No documentation.
  kAttributeNone = 0,
  /// No documentation.
  kOutOfRange = 1,
  /// No documentation.
  kGround = 2,
  /// No documentation.
  kLowIntensity = 4,
  /// No documentation.
  kExclusionZone = 8,
  /// No documentation.
  kStatic = 16,
  /// No documentation.
  kStaticDetectionZone = 32,
  /// No documentation.
  kReflectionZone = 64,

  /// Unknown value read from the wire.
  kUnknownWireValue = 65,
};

// MARK: Message forward declarations

class PointFrame_Header;
class AttributedPoints;
class PointFrame;
class ProcessedPointsEvent;
class AggregatedPointEvents;

// MARK: Message declarations

/// Header for storing per-frame info of the point cloud frame
///
/// Source: horus/pb/point/point_message.proto:35:3
class PointFrame_Header final : public PbMessage {
 public:

  /// Constructs a default-initialized `PointFrame_Header`.
  PointFrame_Header() noexcept = default;

  /// Move constructor.
  PointFrame_Header(PointFrame_Header&&) noexcept = default;
  /// Move assignment operator.
  PointFrame_Header& operator=(PointFrame_Header&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit PointFrame_Header(const PointFrame_Header& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  PointFrame_Header& operator=(const PointFrame_Header&) = delete;

  /// Default destructor.
  ~PointFrame_Header() noexcept final = default;

  /// Creates a `PointFrame_Header` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit PointFrame_Header(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.PointFrame.Header`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.PointFrame.Header"; }

  /// The full name of the message: `horus.pb.PointFrame.Header`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `calibration_transform` (no 1).
  // -----

  /// / The calibration transformation (from sensor to origin coordinates)
  ///
  /// Field no: 1.
  constexpr const AffineTransform3f& calibration_transform() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return calibration_transform_;
  }

  /// If `calibration_transform` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  AffineTransform3f calibration_transform() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(calibration_transform_);
  }

  /// / The calibration transformation (from sensor to origin coordinates)
  ///
  /// Field no: 1.
  AffineTransform3f& mutable_calibration_transform() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return calibration_transform_;
  }

  /// Returns whether `calibration_transform` (no 1) is set.
  constexpr bool has_calibration_transform() const noexcept { return set_fields_[0]; }

  /// Clears `calibration_transform` (no 1).
  void clear_calibration_transform() & noexcept {
    set_fields_[0] = false;
    calibration_transform_ = {};
  }

  /// Sets `calibration_transform` (no 1) and returns `*this`.
  PointFrame_Header& set_calibration_transform(AffineTransform3f&& calibration_transform) & noexcept {
    set_fields_[0] = true;
    calibration_transform_ = std::move(calibration_transform);
    return *this;
  }
  /// Sets `calibration_transform` (no 1) and returns `*this`.
  PointFrame_Header&& set_calibration_transform(AffineTransform3f&& calibration_transform) && noexcept {
    return std::move(set_calibration_transform(std::move(calibration_transform)));
  }

  // Field `lidar_id` (no 2).
  // -----

  /// Lidar ID should be unique among all lidars in the project.
  ///
  /// Field no: 2.
  constexpr const CowBytes& lidar_id() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return lidar_id_;
  }

  /// If `lidar_id` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 2.
  CowBytes lidar_id() && noexcept {
    if (!set_fields_[1]) {
      return {};
    }
    return std::move(lidar_id_);
  }

  /// Lidar ID should be unique among all lidars in the project.
  ///
  /// Field no: 2.
  CowBytes& mutable_lidar_id() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[1] = true;
    return lidar_id_;
  }

  /// Returns whether `lidar_id` (no 2) is set.
  constexpr bool has_lidar_id() const noexcept { return set_fields_[1]; }

  /// Clears `lidar_id` (no 2).
  void clear_lidar_id() & noexcept {
    set_fields_[1] = false;
    lidar_id_ = {};
  }

  /// Sets `lidar_id` (no 2) and returns `*this`.
  PointFrame_Header& set_lidar_id(CowBytes&& lidar_id) & noexcept {
    set_fields_[1] = true;
    lidar_id_ = std::move(lidar_id);
    return *this;
  }
  /// Sets `lidar_id` (no 2) and returns `*this`.
  PointFrame_Header&& set_lidar_id(CowBytes&& lidar_id) && noexcept {
    return std::move(set_lidar_id(std::move(lidar_id)));
  }

  // Field `point_cloud_creation_timestamp` (no 4).
  // -----

  /// / The point cloud creation timestamp.
  ///
  /// Field no: 4.
  constexpr const Timestamp& point_cloud_creation_timestamp() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return point_cloud_creation_timestamp_;
  }

  /// If `point_cloud_creation_timestamp` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 4.
  Timestamp point_cloud_creation_timestamp() && noexcept {
    if (!set_fields_[2]) {
      return {};
    }
    return std::move(point_cloud_creation_timestamp_);
  }

  /// / The point cloud creation timestamp.
  ///
  /// Field no: 4.
  Timestamp& mutable_point_cloud_creation_timestamp() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[2] = true;
    return point_cloud_creation_timestamp_;
  }

  /// Returns whether `point_cloud_creation_timestamp` (no 4) is set.
  constexpr bool has_point_cloud_creation_timestamp() const noexcept { return set_fields_[2]; }

  /// Clears `point_cloud_creation_timestamp` (no 4).
  void clear_point_cloud_creation_timestamp() & noexcept {
    set_fields_[2] = false;
    point_cloud_creation_timestamp_ = {};
  }

  /// Sets `point_cloud_creation_timestamp` (no 4) and returns `*this`.
  PointFrame_Header& set_point_cloud_creation_timestamp(Timestamp&& point_cloud_creation_timestamp) & noexcept {
    set_fields_[2] = true;
    point_cloud_creation_timestamp_ = std::move(point_cloud_creation_timestamp);
    return *this;
  }
  /// Sets `point_cloud_creation_timestamp` (no 4) and returns `*this`.
  PointFrame_Header&& set_point_cloud_creation_timestamp(Timestamp&& point_cloud_creation_timestamp) && noexcept {
    return std::move(set_point_cloud_creation_timestamp(std::move(point_cloud_creation_timestamp)));
  }

 private:
  /// @see calibration_transform()
  AffineTransform3f calibration_transform_{};
  /// @see lidar_id()
  CowBytes lidar_id_{};
  /// @see point_cloud_creation_timestamp()
  Timestamp point_cloud_creation_timestamp_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<3> set_fields_;
};

/// / Point cloud data with attributes.
/// /
/// / @note The attributes are all or none. If the attributes are nonempty, the
/// / number of attributes should be equal to the number of points.
///
/// Source: horus/pb/point/point_message.proto:58:1
class AttributedPoints final : public PbMessage {
 public:

  /// Constructs a default-initialized `AttributedPoints`.
  AttributedPoints() noexcept = default;

  /// Move constructor.
  AttributedPoints(AttributedPoints&&) noexcept = default;
  /// Move assignment operator.
  AttributedPoints& operator=(AttributedPoints&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit AttributedPoints(const AttributedPoints& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  AttributedPoints& operator=(const AttributedPoints&) = delete;

  /// Default destructor.
  ~AttributedPoints() noexcept final = default;

  /// Creates a `AttributedPoints` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit AttributedPoints(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.AttributedPoints`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.AttributedPoints"; }

  /// The full name of the message: `horus.pb.AttributedPoints`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `flattened_points` (no 1).
  // -----

  /// 3D point buffer of which stride length is 3 floats (12 bytes).
  ///
  /// Field no: 1.
  constexpr const CowSpan<float>& flattened_points() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return flattened_points_;
  }

  /// If `flattened_points` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  CowSpan<float> flattened_points() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(flattened_points_);
  }

  /// 3D point buffer of which stride length is 3 floats (12 bytes).
  ///
  /// Field no: 1.
  CowSpan<float>& mutable_flattened_points() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return flattened_points_;
  }

  /// Returns whether `flattened_points` (no 1) is set.
  constexpr bool has_flattened_points() const noexcept { return set_fields_[0]; }

  /// Clears `flattened_points` (no 1).
  void clear_flattened_points() & noexcept {
    set_fields_[0] = false;
    flattened_points_ = {};
  }

  /// Sets `flattened_points` (no 1) and returns `*this`.
  AttributedPoints& set_flattened_points(CowSpan<float>&& flattened_points) & noexcept {
    set_fields_[0] = true;
    flattened_points_ = std::move(flattened_points);
    return *this;
  }
  /// Sets `flattened_points` (no 1) and returns `*this`.
  AttributedPoints&& set_flattened_points(CowSpan<float>&& flattened_points) && noexcept {
    return std::move(set_flattened_points(std::move(flattened_points)));
  }

  // Field `attributes` (no 2).
  // -----

  /// Point attributes such as ground and static. Each element consists of two
  ///  `uint16` attributes to save space. We have to use fixed32 as uint16 is not
  ///  available in Protobuf.
  ///  The first two bytes (low) are occupied by the even index of the attributes
  ///  and the rest (high) are occupied by the odd index. Each element can be
  ///  decomposed as follows:
  ///  ```cpp
  ///  // direct memory copy
  ///  std::memcpy(point_attributes.data(), attributes.data(), num_attributes *
  ///  sizeof(std::uint16_t));
  /// 
  ///  // iterative decomposition
  ///  point_attributes[2 * i + 0] = attributes[i] & 0xFFFF
  ///  point_attributes[2 * i + 1] = (attributes[i] >> 16) & 0xFFFF
  ///  ```
  ///
  /// Field no: 2.
  constexpr const CowSpan<std::uint32_t>& attributes() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return attributes_;
  }

  /// If `attributes` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 2.
  CowSpan<std::uint32_t> attributes() && noexcept {
    if (!set_fields_[1]) {
      return {};
    }
    return std::move(attributes_);
  }

  /// Point attributes such as ground and static. Each element consists of two
  ///  `uint16` attributes to save space. We have to use fixed32 as uint16 is not
  ///  available in Protobuf.
  ///  The first two bytes (low) are occupied by the even index of the attributes
  ///  and the rest (high) are occupied by the odd index. Each element can be
  ///  decomposed as follows:
  ///  ```cpp
  ///  // direct memory copy
  ///  std::memcpy(point_attributes.data(), attributes.data(), num_attributes *
  ///  sizeof(std::uint16_t));
  /// 
  ///  // iterative decomposition
  ///  point_attributes[2 * i + 0] = attributes[i] & 0xFFFF
  ///  point_attributes[2 * i + 1] = (attributes[i] >> 16) & 0xFFFF
  ///  ```
  ///
  /// Field no: 2.
  CowSpan<std::uint32_t>& mutable_attributes() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[1] = true;
    return attributes_;
  }

  /// Returns whether `attributes` (no 2) is set.
  constexpr bool has_attributes() const noexcept { return set_fields_[1]; }

  /// Clears `attributes` (no 2).
  void clear_attributes() & noexcept {
    set_fields_[1] = false;
    attributes_ = {};
  }

  /// Sets `attributes` (no 2) and returns `*this`.
  AttributedPoints& set_attributes(CowSpan<std::uint32_t>&& attributes) & noexcept {
    set_fields_[1] = true;
    attributes_ = std::move(attributes);
    return *this;
  }
  /// Sets `attributes` (no 2) and returns `*this`.
  AttributedPoints&& set_attributes(CowSpan<std::uint32_t>&& attributes) && noexcept {
    return std::move(set_attributes(std::move(attributes)));
  }

  // Field `intensities` (no 3).
  // -----

  /// / Bytes corresponding to packed uint16 intensities for each point.
  /// / We have to use fixed32 as uint16 is not available in Protobuf.
  ///
  /// Field no: 3.
  constexpr const CowSpan<std::uint32_t>& intensities() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return intensities_;
  }

  /// If `intensities` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 3.
  CowSpan<std::uint32_t> intensities() && noexcept {
    if (!set_fields_[2]) {
      return {};
    }
    return std::move(intensities_);
  }

  /// / Bytes corresponding to packed uint16 intensities for each point.
  /// / We have to use fixed32 as uint16 is not available in Protobuf.
  ///
  /// Field no: 3.
  CowSpan<std::uint32_t>& mutable_intensities() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[2] = true;
    return intensities_;
  }

  /// Returns whether `intensities` (no 3) is set.
  constexpr bool has_intensities() const noexcept { return set_fields_[2]; }

  /// Clears `intensities` (no 3).
  void clear_intensities() & noexcept {
    set_fields_[2] = false;
    intensities_ = {};
  }

  /// Sets `intensities` (no 3) and returns `*this`.
  AttributedPoints& set_intensities(CowSpan<std::uint32_t>&& intensities) & noexcept {
    set_fields_[2] = true;
    intensities_ = std::move(intensities);
    return *this;
  }
  /// Sets `intensities` (no 3) and returns `*this`.
  AttributedPoints&& set_intensities(CowSpan<std::uint32_t>&& intensities) && noexcept {
    return std::move(set_intensities(std::move(intensities)));
  }

 private:
  /// @see flattened_points()
  CowSpan<float> flattened_points_{};
  /// @see attributes()
  CowSpan<std::uint32_t> attributes_{};
  /// @see intensities()
  CowSpan<std::uint32_t> intensities_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<3> set_fields_;
};

/// Message frame for single point cloud.
/// 
///  Warning: All the timestamps used in the point frame messages are system clock
///  timestamps. This means that the services exchanging point frame messages
///  must be time synced in order to use these timestamps.
///
/// Source: horus/pb/point/point_message.proto:33:1
class PointFrame final : public PbMessage {
 public:
  /// @see PointFrame_Header
  using Header = PointFrame_Header;

  /// Constructs a default-initialized `PointFrame`.
  PointFrame() noexcept = default;

  /// Move constructor.
  PointFrame(PointFrame&&) noexcept = default;
  /// Move assignment operator.
  PointFrame& operator=(PointFrame&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit PointFrame(const PointFrame& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  PointFrame& operator=(const PointFrame&) = delete;

  /// Default destructor.
  ~PointFrame() noexcept final = default;

  /// Creates a `PointFrame` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit PointFrame(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.PointFrame`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.PointFrame"; }

  /// The full name of the message: `horus.pb.PointFrame`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `header` (no 2).
  // -----

  /// No documentation.
  ///
  /// Field no: 2.
  constexpr const PointFrame_Header& header() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return header_;
  }

  /// If `header` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 2.
  PointFrame_Header header() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(header_);
  }

  /// No documentation.
  ///
  /// Field no: 2.
  PointFrame_Header& mutable_header() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return header_;
  }

  /// Returns whether `header` (no 2) is set.
  constexpr bool has_header() const noexcept { return set_fields_[0]; }

  /// Clears `header` (no 2).
  void clear_header() & noexcept {
    set_fields_[0] = false;
    header_ = {};
  }

  /// Sets `header` (no 2) and returns `*this`.
  PointFrame& set_header(PointFrame_Header&& header) & noexcept {
    set_fields_[0] = true;
    header_ = std::move(header);
    return *this;
  }
  /// Sets `header` (no 2) and returns `*this`.
  PointFrame&& set_header(PointFrame_Header&& header) && noexcept {
    return std::move(set_header(std::move(header)));
  }

  // Field `points` (no 9).
  // -----

  /// No documentation.
  ///
  /// Field no: 9.
  constexpr const AttributedPoints& points() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return points_;
  }

  /// If `points` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 9.
  AttributedPoints points() && noexcept {
    if (!set_fields_[1]) {
      return {};
    }
    return std::move(points_);
  }

  /// No documentation.
  ///
  /// Field no: 9.
  AttributedPoints& mutable_points() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[1] = true;
    return points_;
  }

  /// Returns whether `points` (no 9) is set.
  constexpr bool has_points() const noexcept { return set_fields_[1]; }

  /// Clears `points` (no 9).
  void clear_points() & noexcept {
    set_fields_[1] = false;
    points_ = {};
  }

  /// Sets `points` (no 9) and returns `*this`.
  PointFrame& set_points(AttributedPoints&& points) & noexcept {
    set_fields_[1] = true;
    points_ = std::move(points);
    return *this;
  }
  /// Sets `points` (no 9) and returns `*this`.
  PointFrame&& set_points(AttributedPoints&& points) && noexcept {
    return std::move(set_points(std::move(points)));
  }

 private:
  /// @see header()
  PointFrame_Header header_{};
  /// @see points()
  AttributedPoints points_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<2> set_fields_;
};

/// Processed point cloud data from a single lidar.
///
/// Source: horus/pb/point/point_message.proto:85:1
class ProcessedPointsEvent final : public PbMessage {
 public:

  /// Constructs a default-initialized `ProcessedPointsEvent`.
  ProcessedPointsEvent() noexcept = default;

  /// Move constructor.
  ProcessedPointsEvent(ProcessedPointsEvent&&) noexcept = default;
  /// Move assignment operator.
  ProcessedPointsEvent& operator=(ProcessedPointsEvent&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit ProcessedPointsEvent(const ProcessedPointsEvent& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  ProcessedPointsEvent& operator=(const ProcessedPointsEvent&) = delete;

  /// Default destructor.
  ~ProcessedPointsEvent() noexcept final = default;

  /// Creates a `ProcessedPointsEvent` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit ProcessedPointsEvent(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.ProcessedPointsEvent`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.ProcessedPointsEvent"; }

  /// The full name of the message: `horus.pb.ProcessedPointsEvent`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `point_frame` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr const PointFrame& point_frame() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return point_frame_;
  }

  /// If `point_frame` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  PointFrame point_frame() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(point_frame_);
  }

  /// No documentation.
  ///
  /// Field no: 1.
  PointFrame& mutable_point_frame() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return point_frame_;
  }

  /// Returns whether `point_frame` (no 1) is set.
  constexpr bool has_point_frame() const noexcept { return set_fields_[0]; }

  /// Clears `point_frame` (no 1).
  void clear_point_frame() & noexcept {
    set_fields_[0] = false;
    point_frame_ = {};
  }

  /// Sets `point_frame` (no 1) and returns `*this`.
  ProcessedPointsEvent& set_point_frame(PointFrame&& point_frame) & noexcept {
    set_fields_[0] = true;
    point_frame_ = std::move(point_frame);
    return *this;
  }
  /// Sets `point_frame` (no 1) and returns `*this`.
  ProcessedPointsEvent&& set_point_frame(PointFrame&& point_frame) && noexcept {
    return std::move(set_point_frame(std::move(point_frame)));
  }

 private:
  /// @see point_frame()
  PointFrame point_frame_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<1> set_fields_;
};

/// No documentation.
///
/// Source: horus/pb/point/point_message.proto:89:1
class AggregatedPointEvents final : public PbMessage {
 public:

  /// Constructs a default-initialized `AggregatedPointEvents`.
  AggregatedPointEvents() noexcept = default;

  /// Move constructor.
  AggregatedPointEvents(AggregatedPointEvents&&) noexcept = default;
  /// Move assignment operator.
  AggregatedPointEvents& operator=(AggregatedPointEvents&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit AggregatedPointEvents(const AggregatedPointEvents& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  AggregatedPointEvents& operator=(const AggregatedPointEvents&) = delete;

  /// Default destructor.
  ~AggregatedPointEvents() noexcept final = default;

  /// Creates a `AggregatedPointEvents` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit AggregatedPointEvents(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.AggregatedPointEvents`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.AggregatedPointEvents"; }

  /// The full name of the message: `horus.pb.AggregatedPointEvents`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `events` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr const CowRepeated<ProcessedPointsEvent>& events() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return events_;
  }

  /// If `events` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  CowRepeated<ProcessedPointsEvent> events() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(events_);
  }

  /// No documentation.
  ///
  /// Field no: 1.
  CowRepeated<ProcessedPointsEvent>& mutable_events() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return events_;
  }

  /// Returns whether `events` (no 1) is set.
  constexpr bool has_events() const noexcept { return set_fields_[0]; }

  /// Clears `events` (no 1).
  void clear_events() & noexcept {
    set_fields_[0] = false;
    events_ = {};
  }

  /// Sets `events` (no 1) and returns `*this`.
  AggregatedPointEvents& set_events(CowRepeated<ProcessedPointsEvent>&& events) & noexcept {
    set_fields_[0] = true;
    events_ = std::move(events);
    return *this;
  }
  /// Sets `events` (no 1) and returns `*this`.
  AggregatedPointEvents&& set_events(CowRepeated<ProcessedPointsEvent>&& events) && noexcept {
    return std::move(set_events(std::move(events)));
  }

 private:
  /// @see events()
  CowRepeated<ProcessedPointsEvent> events_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<1> set_fields_;
};

}  // namespace pb
}  // namespace sdk
}  // namespace horus

// MARK: Enum traits

namespace horus {

template <>
class PbEnumTraits<horus::sdk::pb::PointAttribute> final {
 public:
  /// The full name of the enum: `horus.sdk.pb.PointAttribute`.
  static constexpr StringView EnumName() noexcept { return "horus.sdk.pb.PointAttribute"; }

  /// Returns the name of the given enumerator, or an empty string.
  static constexpr StringView NameOf(horus::sdk::pb::PointAttribute value) noexcept {
    switch (value) {
      case horus::sdk::pb::PointAttribute::kAttributeNone: {
        return "ATTRIBUTE_NONE";
      }
      case horus::sdk::pb::PointAttribute::kOutOfRange: {
        return "OUT_OF_RANGE";
      }
      case horus::sdk::pb::PointAttribute::kGround: {
        return "GROUND";
      }
      case horus::sdk::pb::PointAttribute::kLowIntensity: {
        return "LOW_INTENSITY";
      }
      case horus::sdk::pb::PointAttribute::kExclusionZone: {
        return "EXCLUSION_ZONE";
      }
      case horus::sdk::pb::PointAttribute::kStatic: {
        return "STATIC";
      }
      case horus::sdk::pb::PointAttribute::kStaticDetectionZone: {
        return "STATIC_DETECTION_ZONE";
      }
      case horus::sdk::pb::PointAttribute::kReflectionZone: {
        return "REFLECTION_ZONE";
      }
      case horus::sdk::pb::PointAttribute::kUnknownWireValue:
      default: {
        return "";
      }
    }
  }

  /// Returns the value corresponding to the given name, or `default_value`.
  static constexpr horus::sdk::pb::PointAttribute ValueOf(PbEnum value, horus::sdk::pb::PointAttribute default_value = horus::sdk::pb::PointAttribute::kUnknownWireValue) noexcept {
    switch (value) {
      case 0: {
        return horus::sdk::pb::PointAttribute::kAttributeNone;
      }
      case 1: {
        return horus::sdk::pb::PointAttribute::kOutOfRange;
      }
      case 2: {
        return horus::sdk::pb::PointAttribute::kGround;
      }
      case 4: {
        return horus::sdk::pb::PointAttribute::kLowIntensity;
      }
      case 8: {
        return horus::sdk::pb::PointAttribute::kExclusionZone;
      }
      case 16: {
        return horus::sdk::pb::PointAttribute::kStatic;
      }
      case 32: {
        return horus::sdk::pb::PointAttribute::kStaticDetectionZone;
      }
      case 64: {
        return horus::sdk::pb::PointAttribute::kReflectionZone;
      }
      default: {
        return default_value;
      }
    }
  }

  /// Returns the value corresponding to the given name, or `default_value`.
  static constexpr horus::sdk::pb::PointAttribute ValueOf(StringView name, horus::sdk::pb::PointAttribute default_value = horus::sdk::pb::PointAttribute::kUnknownWireValue) noexcept {
    if (name == "ATTRIBUTE_NONE") {
      return horus::sdk::pb::PointAttribute::kAttributeNone;
    }
    if (name == "OUT_OF_RANGE") {
      return horus::sdk::pb::PointAttribute::kOutOfRange;
    }
    if (name == "GROUND") {
      return horus::sdk::pb::PointAttribute::kGround;
    }
    if (name == "LOW_INTENSITY") {
      return horus::sdk::pb::PointAttribute::kLowIntensity;
    }
    if (name == "EXCLUSION_ZONE") {
      return horus::sdk::pb::PointAttribute::kExclusionZone;
    }
    if (name == "STATIC") {
      return horus::sdk::pb::PointAttribute::kStatic;
    }
    if (name == "STATIC_DETECTION_ZONE") {
      return horus::sdk::pb::PointAttribute::kStaticDetectionZone;
    }
    if (name == "REFLECTION_ZONE") {
      return horus::sdk::pb::PointAttribute::kReflectionZone;
    }
    return default_value;
  }
};

template <>
class PbTraits<horus::sdk::pb::PointAttribute> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, horus::sdk::pb::PointAttribute value) {
    writer.Writer().add_enum(tag, static_cast<PbEnum>(value));
  }

  /// Deserializes `horus::sdk::pb::PointAttribute` from `reader`.
  static horus::sdk::pb::PointAttribute Deserialize(PbReader& reader) {
    return PbEnumTraits<horus::sdk::pb::PointAttribute>::ValueOf(reader.Reader().get_enum());
  }
};

}  // namespace horus

namespace horus {
namespace sdk {
namespace pb {

/// Appends `value` to `sink`.
template <class Sink>
void HorusStringify(Sink& sink, PointAttribute value) noexcept(noexcept(sink.Append(StringView{}))) {
  sink.Append(PbEnumTraits<PointAttribute>::NameOf(value));
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus

// NOLINTEND(readability-identifier-length)

#endif  // HORUS_PB_POINT_POINT_MESSAGE_PB_H_
