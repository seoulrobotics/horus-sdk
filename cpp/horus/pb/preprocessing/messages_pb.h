#ifndef HORUS_PB_PREPROCESSING_MESSAGES_PB_H_
#define HORUS_PB_PREPROCESSING_MESSAGES_PB_H_

/// @file
///
/// C++ type definitions for Protobuf messages in `horus/pb/preprocessing/messages.proto`.
///
/// Generated by `/home/runner/work/horus/horus/common/tools/sdk/pb.ts`.

#include <bitset>
#include <cstdint>
#include <utility>

#include "horus/attributes.h"
#include "horus/pb/config/metadata_pb.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/cow_repeated.h"
#include "horus/pb/message.h"
#include "horus/pb/serialize.h"
#include "horus/pb/types.h"
#include "horus/strings/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

// NOLINTBEGIN(readability-identifier-length)

namespace horus {
namespace sdk {
namespace pb {

// MARK: Enum declarations

/// / The classification of the occupancy grid cell.
/// / @note The maximum number of classification enum values supported is 2^3=8.
/// / @see `OccupancyGrid` message and `occupancy_map_constants` for more details.
///
/// Source: horus/pb/preprocessing/messages.proto:10:1
enum class OccupancyClassification : PbEnum {  // NOLINT(*-enum-size)
  /// No documentation.
  kOccupancyclassificationUnspecified = 0,
  /// No documentation.
  kFree = 1,
  /// No documentation.
  kOccluded = 2,
  /// No documentation.
  kStationaryOccupied = 3,
  /// No documentation.
  kExcluded = 4,

  /// Unknown value read from the wire.
  kUnknownWireValue = 5,
};

/// Sensor status enum.
///  @note Each enum value should be a power of two.
///  @note No status should have the same value.
///
/// Source: horus/pb/preprocessing/messages.proto:62:1
enum class SensorStatus : PbEnum {  // NOLINT(*-enum-size)
  /// No documentation.
  kUnspecified = 0,
  /// No documentation.
  kNoData = 1,
  /// No documentation.
  kReceivingData = 2,
  /// No documentation.
  kLowFrequency = 4,
  /// No documentation.
  kHighFrequency = 8,
  /// No documentation.
  kTilted = 16,
  /// No documentation.
  kObstructed = 32,

  /// Unknown value read from the wire.
  kUnknownWireValue = 33,
};

// MARK: Message forward declarations

class OccupancyGrid;
class OccupancyGridEvent;
class SensorInfo_PoseCorrection;
class SensorInfo;

// MARK: Message declarations

/// / The data of the occupancy map.
/// / The data is formated with a run-length encoding (RLE) in row-major order.
/// / Consecutive cells of the same classification are compressed into a single
/// / uint32.
///
/// Source: horus/pb/preprocessing/messages.proto:23:1
class OccupancyGrid final : public PbMessage {
 public:

  /// Constructs a default-initialized `OccupancyGrid`.
  OccupancyGrid() noexcept = default;

  /// Move constructor.
  OccupancyGrid(OccupancyGrid&&) noexcept = default;
  /// Move assignment operator.
  OccupancyGrid& operator=(OccupancyGrid&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit OccupancyGrid(const OccupancyGrid& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  OccupancyGrid& operator=(const OccupancyGrid&) = delete;

  /// Default destructor.
  ~OccupancyGrid() noexcept final = default;

  /// Creates a `OccupancyGrid` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit OccupancyGrid(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.OccupancyGrid`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.OccupancyGrid"; }

  /// The full name of the message: `horus.pb.OccupancyGrid`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `rows` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr std::uint32_t rows() const& noexcept HORUS_LIFETIME_BOUND {
    return rows_;
  }

  /// No documentation.
  ///
  /// Field no: 1.
  std::uint32_t& mutable_rows() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[0] = true;
    return rows_;
  }

  /// Returns whether `rows` (no 1) is set.
  constexpr bool has_rows() const noexcept { return set_fields_[0]; }

  /// Clears `rows` (no 1).
  void clear_rows() & noexcept {
    set_fields_[0] = false;
    rows_ = {};
  }

  /// Sets `rows` (no 1) and returns `*this`.
  OccupancyGrid& set_rows(std::uint32_t rows) & noexcept {
    set_fields_[0] = true;
    rows_ = rows;
    return *this;
  }
  /// Sets `rows` (no 1) and returns `*this`.
  OccupancyGrid&& set_rows(std::uint32_t rows) && noexcept {
    return std::move(set_rows(rows));
  }

  // Field `cols` (no 2).
  // -----

  /// No documentation.
  ///
  /// Field no: 2.
  constexpr std::uint32_t cols() const& noexcept HORUS_LIFETIME_BOUND {
    return cols_;
  }

  /// No documentation.
  ///
  /// Field no: 2.
  std::uint32_t& mutable_cols() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[1] = true;
    return cols_;
  }

  /// Returns whether `cols` (no 2) is set.
  constexpr bool has_cols() const noexcept { return set_fields_[1]; }

  /// Clears `cols` (no 2).
  void clear_cols() & noexcept {
    set_fields_[1] = false;
    cols_ = {};
  }

  /// Sets `cols` (no 2) and returns `*this`.
  OccupancyGrid& set_cols(std::uint32_t cols) & noexcept {
    set_fields_[1] = true;
    cols_ = cols;
    return *this;
  }
  /// Sets `cols` (no 2) and returns `*this`.
  OccupancyGrid&& set_cols(std::uint32_t cols) && noexcept {
    return std::move(set_cols(cols));
  }

  // Field `cells` (no 3).
  // -----

  /// Packed representation of cells: Encodes <value, length>. The three most
  ///  significant bits of the uint32 encodes the value of
  ///  `OccupancyClassification`. The remaining 29 bits encode the run length.
  ///  @note The maximum number of classification enum values supported is 2^3=8.
  ///  @see `occupancy_map_constants` for more details.
  ///
  /// Field no: 3.
  constexpr const CowRepeated<std::uint32_t>& cells() const& noexcept HORUS_LIFETIME_BOUND {
    return cells_;
  }

  /// If `cells` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 3.
  CowRepeated<std::uint32_t> cells() && noexcept {
    if (!set_fields_[2]) {
      return {};
    }
    return std::move(cells_);
  }

  /// Packed representation of cells: Encodes <value, length>. The three most
  ///  significant bits of the uint32 encodes the value of
  ///  `OccupancyClassification`. The remaining 29 bits encode the run length.
  ///  @note The maximum number of classification enum values supported is 2^3=8.
  ///  @see `occupancy_map_constants` for more details.
  ///
  /// Field no: 3.
  CowRepeated<std::uint32_t>& mutable_cells() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[2] = true;
    return cells_;
  }

  /// Returns whether `cells` (no 3) is set.
  constexpr bool has_cells() const noexcept { return set_fields_[2]; }

  /// Clears `cells` (no 3).
  void clear_cells() & noexcept {
    set_fields_[2] = false;
    cells_ = {};
  }

  /// Sets `cells` (no 3) and returns `*this`.
  OccupancyGrid& set_cells(CowRepeated<std::uint32_t>&& cells) & noexcept {
    set_fields_[2] = true;
    cells_ = std::move(cells);
    return *this;
  }
  /// Sets `cells` (no 3) and returns `*this`.
  OccupancyGrid&& set_cells(CowRepeated<std::uint32_t>&& cells) && noexcept {
    return std::move(set_cells(std::move(cells)));
  }

 private:
  /// @see rows()
  std::uint32_t rows_{};
  /// @see cols()
  std::uint32_t cols_{};
  /// @see cells()
  CowRepeated<std::uint32_t> cells_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<3> set_fields_;
};

/// / The occupancy grid event.
/// / Contains the occupancy grid, the range of the grid and the resolution.
/// /
/// / Grid Coverage:
/// /   The grid covers the x and y range inclusively with the specified
/// /   resolution.
/// /
/// / Grid Dims Calculation:
/// /   Computed as:
/// /     `x_dims = floor((x_max - x_min) / resolution) + 1`
/// /
/// / Projecting a point onto the Grid:
/// /   To determine the grid cell corresponding to a point in space:
/// /     `grid_x = floor((point.x - x_min) / resolution)`
/// /     `grid_y = floor((point.y - y_min) / resolution)`
///
/// Source: horus/pb/preprocessing/messages.proto:49:1
class OccupancyGridEvent final : public PbMessage {
 public:

  /// Constructs a default-initialized `OccupancyGridEvent`.
  OccupancyGridEvent() noexcept = default;

  /// Move constructor.
  OccupancyGridEvent(OccupancyGridEvent&&) noexcept = default;
  /// Move assignment operator.
  OccupancyGridEvent& operator=(OccupancyGridEvent&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit OccupancyGridEvent(const OccupancyGridEvent& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  OccupancyGridEvent& operator=(const OccupancyGridEvent&) = delete;

  /// Default destructor.
  ~OccupancyGridEvent() noexcept final = default;

  /// Creates a `OccupancyGridEvent` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit OccupancyGridEvent(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.OccupancyGridEvent`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.OccupancyGridEvent"; }

  /// The full name of the message: `horus.pb.OccupancyGridEvent`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `grid` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr const OccupancyGrid& grid() const& noexcept HORUS_LIFETIME_BOUND {
    return grid_;
  }

  /// If `grid` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  OccupancyGrid grid() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(grid_);
  }

  /// No documentation.
  ///
  /// Field no: 1.
  OccupancyGrid& mutable_grid() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[0] = true;
    return grid_;
  }

  /// Returns whether `grid` (no 1) is set.
  constexpr bool has_grid() const noexcept { return set_fields_[0]; }

  /// Clears `grid` (no 1).
  void clear_grid() & noexcept {
    set_fields_[0] = false;
    grid_ = {};
  }

  /// Sets `grid` (no 1) and returns `*this`.
  OccupancyGridEvent& set_grid(OccupancyGrid&& grid) & noexcept {
    set_fields_[0] = true;
    grid_ = std::move(grid);
    return *this;
  }
  /// Sets `grid` (no 1) and returns `*this`.
  OccupancyGridEvent&& set_grid(OccupancyGrid&& grid) && noexcept {
    return std::move(set_grid(std::move(grid)));
  }

  // Field `x_min` (no 2).
  // -----

  /// No documentation.
  ///
  /// Field no: 2.
  constexpr float x_min() const& noexcept HORUS_LIFETIME_BOUND {
    return x_min_;
  }

  /// No documentation.
  ///
  /// Field no: 2.
  float& mutable_x_min() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[1] = true;
    return x_min_;
  }

  /// Returns whether `x_min` (no 2) is set.
  constexpr bool has_x_min() const noexcept { return set_fields_[1]; }

  /// Clears `x_min` (no 2).
  void clear_x_min() & noexcept {
    set_fields_[1] = false;
    x_min_ = {};
  }

  /// Sets `x_min` (no 2) and returns `*this`.
  OccupancyGridEvent& set_x_min(float x_min) & noexcept {
    set_fields_[1] = true;
    x_min_ = x_min;
    return *this;
  }
  /// Sets `x_min` (no 2) and returns `*this`.
  OccupancyGridEvent&& set_x_min(float x_min) && noexcept {
    return std::move(set_x_min(x_min));
  }

  // Field `x_max` (no 3).
  // -----

  /// No documentation.
  ///
  /// Field no: 3.
  constexpr float x_max() const& noexcept HORUS_LIFETIME_BOUND {
    return x_max_;
  }

  /// No documentation.
  ///
  /// Field no: 3.
  float& mutable_x_max() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[2] = true;
    return x_max_;
  }

  /// Returns whether `x_max` (no 3) is set.
  constexpr bool has_x_max() const noexcept { return set_fields_[2]; }

  /// Clears `x_max` (no 3).
  void clear_x_max() & noexcept {
    set_fields_[2] = false;
    x_max_ = {};
  }

  /// Sets `x_max` (no 3) and returns `*this`.
  OccupancyGridEvent& set_x_max(float x_max) & noexcept {
    set_fields_[2] = true;
    x_max_ = x_max;
    return *this;
  }
  /// Sets `x_max` (no 3) and returns `*this`.
  OccupancyGridEvent&& set_x_max(float x_max) && noexcept {
    return std::move(set_x_max(x_max));
  }

  // Field `y_min` (no 4).
  // -----

  /// No documentation.
  ///
  /// Field no: 4.
  constexpr float y_min() const& noexcept HORUS_LIFETIME_BOUND {
    return y_min_;
  }

  /// No documentation.
  ///
  /// Field no: 4.
  float& mutable_y_min() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[3] = true;
    return y_min_;
  }

  /// Returns whether `y_min` (no 4) is set.
  constexpr bool has_y_min() const noexcept { return set_fields_[3]; }

  /// Clears `y_min` (no 4).
  void clear_y_min() & noexcept {
    set_fields_[3] = false;
    y_min_ = {};
  }

  /// Sets `y_min` (no 4) and returns `*this`.
  OccupancyGridEvent& set_y_min(float y_min) & noexcept {
    set_fields_[3] = true;
    y_min_ = y_min;
    return *this;
  }
  /// Sets `y_min` (no 4) and returns `*this`.
  OccupancyGridEvent&& set_y_min(float y_min) && noexcept {
    return std::move(set_y_min(y_min));
  }

  // Field `y_max` (no 5).
  // -----

  /// No documentation.
  ///
  /// Field no: 5.
  constexpr float y_max() const& noexcept HORUS_LIFETIME_BOUND {
    return y_max_;
  }

  /// No documentation.
  ///
  /// Field no: 5.
  float& mutable_y_max() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[4] = true;
    return y_max_;
  }

  /// Returns whether `y_max` (no 5) is set.
  constexpr bool has_y_max() const noexcept { return set_fields_[4]; }

  /// Clears `y_max` (no 5).
  void clear_y_max() & noexcept {
    set_fields_[4] = false;
    y_max_ = {};
  }

  /// Sets `y_max` (no 5) and returns `*this`.
  OccupancyGridEvent& set_y_max(float y_max) & noexcept {
    set_fields_[4] = true;
    y_max_ = y_max;
    return *this;
  }
  /// Sets `y_max` (no 5) and returns `*this`.
  OccupancyGridEvent&& set_y_max(float y_max) && noexcept {
    return std::move(set_y_max(y_max));
  }

  // Field `resolution` (no 6).
  // -----

  /// No documentation.
  ///
  /// Field no: 6.
  constexpr float resolution() const& noexcept HORUS_LIFETIME_BOUND {
    return resolution_;
  }

  /// No documentation.
  ///
  /// Field no: 6.
  float& mutable_resolution() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[5] = true;
    return resolution_;
  }

  /// Returns whether `resolution` (no 6) is set.
  constexpr bool has_resolution() const noexcept { return set_fields_[5]; }

  /// Clears `resolution` (no 6).
  void clear_resolution() & noexcept {
    set_fields_[5] = false;
    resolution_ = {};
  }

  /// Sets `resolution` (no 6) and returns `*this`.
  OccupancyGridEvent& set_resolution(float resolution) & noexcept {
    set_fields_[5] = true;
    resolution_ = resolution;
    return *this;
  }
  /// Sets `resolution` (no 6) and returns `*this`.
  OccupancyGridEvent&& set_resolution(float resolution) && noexcept {
    return std::move(set_resolution(resolution));
  }

  // Field `timestamp` (no 7).
  // -----

  /// No documentation.
  ///
  /// Field no: 7.
  constexpr const Timestamp& timestamp() const& noexcept HORUS_LIFETIME_BOUND {
    return timestamp_;
  }

  /// If `timestamp` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 7.
  Timestamp timestamp() && noexcept {
    if (!set_fields_[6]) {
      return {};
    }
    return std::move(timestamp_);
  }

  /// No documentation.
  ///
  /// Field no: 7.
  Timestamp& mutable_timestamp() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[6] = true;
    return timestamp_;
  }

  /// Returns whether `timestamp` (no 7) is set.
  constexpr bool has_timestamp() const noexcept { return set_fields_[6]; }

  /// Clears `timestamp` (no 7).
  void clear_timestamp() & noexcept {
    set_fields_[6] = false;
    timestamp_ = {};
  }

  /// Sets `timestamp` (no 7) and returns `*this`.
  OccupancyGridEvent& set_timestamp(Timestamp&& timestamp) & noexcept {
    set_fields_[6] = true;
    timestamp_ = std::move(timestamp);
    return *this;
  }
  /// Sets `timestamp` (no 7) and returns `*this`.
  OccupancyGridEvent&& set_timestamp(Timestamp&& timestamp) && noexcept {
    return std::move(set_timestamp(std::move(timestamp)));
  }

 private:
  /// @see grid()
  OccupancyGrid grid_{};
  /// @see x_min()
  float x_min_{};
  /// @see x_max()
  float x_max_{};
  /// @see y_min()
  float y_min_{};
  /// @see y_max()
  float y_max_{};
  /// @see resolution()
  float resolution_{};
  /// @see timestamp()
  Timestamp timestamp_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<7> set_fields_;
};

/// No documentation.
///
/// Source: horus/pb/preprocessing/messages.proto:80:3
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
  constexpr const Vector3f& translation() const& noexcept HORUS_LIFETIME_BOUND {
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
  Vector3f& mutable_translation() & noexcept HORUS_LIFETIME_BOUND {
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
  constexpr const Quaterniond& rotation() const& noexcept HORUS_LIFETIME_BOUND {
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
  Quaterniond& mutable_rotation() & noexcept HORUS_LIFETIME_BOUND {
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
/// Source: horus/pb/preprocessing/messages.proto:72:1
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
  constexpr const CowBytes& lidar_id() const& noexcept HORUS_LIFETIME_BOUND {
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
  CowBytes& mutable_lidar_id() & noexcept HORUS_LIFETIME_BOUND {
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
  constexpr std::uint32_t status() const& noexcept HORUS_LIFETIME_BOUND {
    return status_;
  }

  /// Status as uint32, each status is combined using bitwise OR.
  ///
  /// Field no: 2.
  std::uint32_t& mutable_status() & noexcept HORUS_LIFETIME_BOUND {
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
  constexpr double measured_frequency() const& noexcept HORUS_LIFETIME_BOUND {
    return measured_frequency_;
  }

  /// No documentation.
  ///
  /// Field no: 3.
  double& mutable_measured_frequency() & noexcept HORUS_LIFETIME_BOUND {
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
  constexpr const SensorInfo_PoseCorrection& pose_correction() const& noexcept HORUS_LIFETIME_BOUND {
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
  SensorInfo_PoseCorrection& mutable_pose_correction() & noexcept HORUS_LIFETIME_BOUND {
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

}  // namespace pb
}  // namespace sdk
}  // namespace horus

// MARK: Enum traits

namespace horus {

template <>
class PbEnumTraits<horus::sdk::pb::OccupancyClassification> final {
 public:
  /// The full name of the enum: `horus.sdk.pb.OccupancyClassification`.
  static constexpr StringView EnumName() noexcept { return "horus.sdk.pb.OccupancyClassification"; }

  /// Returns the name of the given enumerator, or an empty string.
  static constexpr StringView NameOf(horus::sdk::pb::OccupancyClassification value) noexcept {
    switch (value) {
      case horus::sdk::pb::OccupancyClassification::kOccupancyclassificationUnspecified: {
        return "OCCUPANCYCLASSIFICATION_UNSPECIFIED";
      }
      case horus::sdk::pb::OccupancyClassification::kFree: {
        return "FREE";
      }
      case horus::sdk::pb::OccupancyClassification::kOccluded: {
        return "OCCLUDED";
      }
      case horus::sdk::pb::OccupancyClassification::kStationaryOccupied: {
        return "STATIONARY_OCCUPIED";
      }
      case horus::sdk::pb::OccupancyClassification::kExcluded: {
        return "EXCLUDED";
      }
      case horus::sdk::pb::OccupancyClassification::kUnknownWireValue:
      default: {
        return "";
      }
    }
  }

  /// Returns the value corresponding to the given name, or `default_value`.
  static constexpr horus::sdk::pb::OccupancyClassification ValueOf(PbEnum value, horus::sdk::pb::OccupancyClassification default_value = horus::sdk::pb::OccupancyClassification::kUnknownWireValue) noexcept {
    switch (value) {
      case 0: {
        return horus::sdk::pb::OccupancyClassification::kOccupancyclassificationUnspecified;
      }
      case 1: {
        return horus::sdk::pb::OccupancyClassification::kFree;
      }
      case 2: {
        return horus::sdk::pb::OccupancyClassification::kOccluded;
      }
      case 3: {
        return horus::sdk::pb::OccupancyClassification::kStationaryOccupied;
      }
      case 4: {
        return horus::sdk::pb::OccupancyClassification::kExcluded;
      }
      default: {
        return default_value;
      }
    }
  }

  /// Returns the value corresponding to the given name, or `default_value`.
  static constexpr horus::sdk::pb::OccupancyClassification ValueOf(StringView name, horus::sdk::pb::OccupancyClassification default_value = horus::sdk::pb::OccupancyClassification::kUnknownWireValue) noexcept {
    if (name == "OCCUPANCYCLASSIFICATION_UNSPECIFIED") {
      return horus::sdk::pb::OccupancyClassification::kOccupancyclassificationUnspecified;
    }
    if (name == "FREE") {
      return horus::sdk::pb::OccupancyClassification::kFree;
    }
    if (name == "OCCLUDED") {
      return horus::sdk::pb::OccupancyClassification::kOccluded;
    }
    if (name == "STATIONARY_OCCUPIED") {
      return horus::sdk::pb::OccupancyClassification::kStationaryOccupied;
    }
    if (name == "EXCLUDED") {
      return horus::sdk::pb::OccupancyClassification::kExcluded;
    }
    return default_value;
  }
};

template <>
class PbTraits<horus::sdk::pb::OccupancyClassification> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, horus::sdk::pb::OccupancyClassification value) {
    writer.Writer().add_enum(tag, static_cast<PbEnum>(value));
  }

  /// Deserializes `horus::sdk::pb::OccupancyClassification` from `reader`.
  static horus::sdk::pb::OccupancyClassification Deserialize(PbReader& reader) {
    return PbEnumTraits<horus::sdk::pb::OccupancyClassification>::ValueOf(reader.Reader().get_enum());
  }
};

template <>
class PbEnumTraits<horus::sdk::pb::SensorStatus> final {
 public:
  /// The full name of the enum: `horus.sdk.pb.SensorStatus`.
  static constexpr StringView EnumName() noexcept { return "horus.sdk.pb.SensorStatus"; }

  /// Returns the name of the given enumerator, or an empty string.
  static constexpr StringView NameOf(horus::sdk::pb::SensorStatus value) noexcept {
    switch (value) {
      case horus::sdk::pb::SensorStatus::kUnspecified: {
        return "SENSOR_STATUS_UNSPECIFIED";
      }
      case horus::sdk::pb::SensorStatus::kNoData: {
        return "NO_DATA";
      }
      case horus::sdk::pb::SensorStatus::kReceivingData: {
        return "RECEIVING_DATA";
      }
      case horus::sdk::pb::SensorStatus::kLowFrequency: {
        return "LOW_FREQUENCY";
      }
      case horus::sdk::pb::SensorStatus::kHighFrequency: {
        return "HIGH_FREQUENCY";
      }
      case horus::sdk::pb::SensorStatus::kTilted: {
        return "TILTED";
      }
      case horus::sdk::pb::SensorStatus::kObstructed: {
        return "OBSTRUCTED";
      }
      case horus::sdk::pb::SensorStatus::kUnknownWireValue:
      default: {
        return "";
      }
    }
  }

  /// Returns the value corresponding to the given name, or `default_value`.
  static constexpr horus::sdk::pb::SensorStatus ValueOf(PbEnum value, horus::sdk::pb::SensorStatus default_value = horus::sdk::pb::SensorStatus::kUnknownWireValue) noexcept {
    switch (value) {
      case 0: {
        return horus::sdk::pb::SensorStatus::kUnspecified;
      }
      case 1: {
        return horus::sdk::pb::SensorStatus::kNoData;
      }
      case 2: {
        return horus::sdk::pb::SensorStatus::kReceivingData;
      }
      case 4: {
        return horus::sdk::pb::SensorStatus::kLowFrequency;
      }
      case 8: {
        return horus::sdk::pb::SensorStatus::kHighFrequency;
      }
      case 16: {
        return horus::sdk::pb::SensorStatus::kTilted;
      }
      case 32: {
        return horus::sdk::pb::SensorStatus::kObstructed;
      }
      default: {
        return default_value;
      }
    }
  }

  /// Returns the value corresponding to the given name, or `default_value`.
  static constexpr horus::sdk::pb::SensorStatus ValueOf(StringView name, horus::sdk::pb::SensorStatus default_value = horus::sdk::pb::SensorStatus::kUnknownWireValue) noexcept {
    if (name == "SENSOR_STATUS_UNSPECIFIED") {
      return horus::sdk::pb::SensorStatus::kUnspecified;
    }
    if (name == "NO_DATA") {
      return horus::sdk::pb::SensorStatus::kNoData;
    }
    if (name == "RECEIVING_DATA") {
      return horus::sdk::pb::SensorStatus::kReceivingData;
    }
    if (name == "LOW_FREQUENCY") {
      return horus::sdk::pb::SensorStatus::kLowFrequency;
    }
    if (name == "HIGH_FREQUENCY") {
      return horus::sdk::pb::SensorStatus::kHighFrequency;
    }
    if (name == "TILTED") {
      return horus::sdk::pb::SensorStatus::kTilted;
    }
    if (name == "OBSTRUCTED") {
      return horus::sdk::pb::SensorStatus::kObstructed;
    }
    return default_value;
  }
};

template <>
class PbTraits<horus::sdk::pb::SensorStatus> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, horus::sdk::pb::SensorStatus value) {
    writer.Writer().add_enum(tag, static_cast<PbEnum>(value));
  }

  /// Deserializes `horus::sdk::pb::SensorStatus` from `reader`.
  static horus::sdk::pb::SensorStatus Deserialize(PbReader& reader) {
    return PbEnumTraits<horus::sdk::pb::SensorStatus>::ValueOf(reader.Reader().get_enum());
  }
};

}  // namespace horus

namespace horus {
namespace sdk {
namespace pb {

/// Appends `value` to `sink`.
template <class Sink>
void HorusStringify(Sink& sink, OccupancyClassification value) noexcept(noexcept(sink.Append(StringView{}))) {
  sink.Append(PbEnumTraits<OccupancyClassification>::NameOf(value));
}

/// Appends `value` to `sink`.
template <class Sink>
void HorusStringify(Sink& sink, SensorStatus value) noexcept(noexcept(sink.Append(StringView{}))) {
  sink.Append(PbEnumTraits<SensorStatus>::NameOf(value));
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus

// NOLINTEND(readability-identifier-length)

#endif  // HORUS_PB_PREPROCESSING_MESSAGES_PB_H_
