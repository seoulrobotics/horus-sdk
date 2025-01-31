#ifndef HORUS_PB_CONFIG_METADATA_PB_H_
#define HORUS_PB_CONFIG_METADATA_PB_H_

/// @file
///
/// C++ type definitions for Protobuf messages in `horus/pb/config/metadata.proto`.
///
/// Generated by `/home/runner/work/horus/horus/common/tools/sdk/pb.ts`.

#include <bitset>
#include <cstdint>
#include <utility>

#include "horus/pb/cow_repeated.h"
#include "horus/pb/cow_span.h"
#include "horus/pb/message.h"
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

class Vector2f;
class Vector3f;
class Range;
class UInt32List;
class Timestamp;
class Duration;
class AffineTransform3f;

// MARK: Message declarations

/// A vector with 2 `float` fields.
///
/// Source: horus/pb/config/metadata.proto:10:1
class Vector2f final : public PbMessage {
 public:

  /// Constructs a default-initialized `Vector2f`.
  Vector2f() noexcept = default;

  /// Move constructor.
  Vector2f(Vector2f&&) noexcept = default;
  /// Move assignment operator.
  Vector2f& operator=(Vector2f&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit Vector2f(const Vector2f& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  Vector2f& operator=(const Vector2f&) = delete;

  /// Default destructor.
  ~Vector2f() noexcept final = default;

  /// Creates a `Vector2f` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit Vector2f(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.Vector2f`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.Vector2f"; }

  /// The full name of the message: `horus.pb.Vector2f`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `x` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr float x() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return x_;
  }

  /// No documentation.
  ///
  /// Field no: 1.
  float& mutable_x() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return x_;
  }

  /// Returns whether `x` (no 1) is set.
  constexpr bool has_x() const noexcept { return set_fields_[0]; }

  /// Clears `x` (no 1).
  void clear_x() & noexcept {
    set_fields_[0] = false;
    x_ = {};
  }

  /// Sets `x` (no 1) and returns `*this`.
  Vector2f& set_x(float x) & noexcept {
    set_fields_[0] = true;
    x_ = x;
    return *this;
  }
  /// Sets `x` (no 1) and returns `*this`.
  Vector2f&& set_x(float x) && noexcept {
    return std::move(set_x(x));
  }

  // Field `y` (no 2).
  // -----

  /// No documentation.
  ///
  /// Field no: 2.
  constexpr float y() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return y_;
  }

  /// No documentation.
  ///
  /// Field no: 2.
  float& mutable_y() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[1] = true;
    return y_;
  }

  /// Returns whether `y` (no 2) is set.
  constexpr bool has_y() const noexcept { return set_fields_[1]; }

  /// Clears `y` (no 2).
  void clear_y() & noexcept {
    set_fields_[1] = false;
    y_ = {};
  }

  /// Sets `y` (no 2) and returns `*this`.
  Vector2f& set_y(float y) & noexcept {
    set_fields_[1] = true;
    y_ = y;
    return *this;
  }
  /// Sets `y` (no 2) and returns `*this`.
  Vector2f&& set_y(float y) && noexcept {
    return std::move(set_y(y));
  }

 private:
  /// @see x()
  float x_{};
  /// @see y()
  float y_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<2> set_fields_;
};

/// A vector with 3 `float` fields.
///
/// Source: horus/pb/config/metadata.proto:17:1
class Vector3f final : public PbMessage {
 public:

  /// Constructs a default-initialized `Vector3f`.
  Vector3f() noexcept = default;

  /// Move constructor.
  Vector3f(Vector3f&&) noexcept = default;
  /// Move assignment operator.
  Vector3f& operator=(Vector3f&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit Vector3f(const Vector3f& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  Vector3f& operator=(const Vector3f&) = delete;

  /// Default destructor.
  ~Vector3f() noexcept final = default;

  /// Creates a `Vector3f` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit Vector3f(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.Vector3f`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.Vector3f"; }

  /// The full name of the message: `horus.pb.Vector3f`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `x` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr float x() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return x_;
  }

  /// No documentation.
  ///
  /// Field no: 1.
  float& mutable_x() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return x_;
  }

  /// Returns whether `x` (no 1) is set.
  constexpr bool has_x() const noexcept { return set_fields_[0]; }

  /// Clears `x` (no 1).
  void clear_x() & noexcept {
    set_fields_[0] = false;
    x_ = {};
  }

  /// Sets `x` (no 1) and returns `*this`.
  Vector3f& set_x(float x) & noexcept {
    set_fields_[0] = true;
    x_ = x;
    return *this;
  }
  /// Sets `x` (no 1) and returns `*this`.
  Vector3f&& set_x(float x) && noexcept {
    return std::move(set_x(x));
  }

  // Field `y` (no 2).
  // -----

  /// No documentation.
  ///
  /// Field no: 2.
  constexpr float y() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return y_;
  }

  /// No documentation.
  ///
  /// Field no: 2.
  float& mutable_y() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[1] = true;
    return y_;
  }

  /// Returns whether `y` (no 2) is set.
  constexpr bool has_y() const noexcept { return set_fields_[1]; }

  /// Clears `y` (no 2).
  void clear_y() & noexcept {
    set_fields_[1] = false;
    y_ = {};
  }

  /// Sets `y` (no 2) and returns `*this`.
  Vector3f& set_y(float y) & noexcept {
    set_fields_[1] = true;
    y_ = y;
    return *this;
  }
  /// Sets `y` (no 2) and returns `*this`.
  Vector3f&& set_y(float y) && noexcept {
    return std::move(set_y(y));
  }

  // Field `z` (no 3).
  // -----

  /// No documentation.
  ///
  /// Field no: 3.
  constexpr float z() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return z_;
  }

  /// No documentation.
  ///
  /// Field no: 3.
  float& mutable_z() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[2] = true;
    return z_;
  }

  /// Returns whether `z` (no 3) is set.
  constexpr bool has_z() const noexcept { return set_fields_[2]; }

  /// Clears `z` (no 3).
  void clear_z() & noexcept {
    set_fields_[2] = false;
    z_ = {};
  }

  /// Sets `z` (no 3) and returns `*this`.
  Vector3f& set_z(float z) & noexcept {
    set_fields_[2] = true;
    z_ = z;
    return *this;
  }
  /// Sets `z` (no 3) and returns `*this`.
  Vector3f&& set_z(float z) && noexcept {
    return std::move(set_z(z));
  }

 private:
  /// @see x()
  float x_{};
  /// @see y()
  float y_{};
  /// @see z()
  float z_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<3> set_fields_;
};

/// A vector with two `double` fields representing a range (where the `start`
///  must be lower than or equal to the `end`).
///
/// Source: horus/pb/config/metadata.proto:26:1
class Range final : public PbMessage {
 public:

  /// Constructs a default-initialized `Range`.
  Range() noexcept = default;

  /// Move constructor.
  Range(Range&&) noexcept = default;
  /// Move assignment operator.
  Range& operator=(Range&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit Range(const Range& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  Range& operator=(const Range&) = delete;

  /// Default destructor.
  ~Range() noexcept final = default;

  /// Creates a `Range` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit Range(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.Range`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.Range"; }

  /// The full name of the message: `horus.pb.Range`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `start` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr double start() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return start_;
  }

  /// No documentation.
  ///
  /// Field no: 1.
  double& mutable_start() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return start_;
  }

  /// Returns whether `start` (no 1) is set.
  constexpr bool has_start() const noexcept { return set_fields_[0]; }

  /// Clears `start` (no 1).
  void clear_start() & noexcept {
    set_fields_[0] = false;
    start_ = {};
  }

  /// Sets `start` (no 1) and returns `*this`.
  Range& set_start(double start) & noexcept {
    set_fields_[0] = true;
    start_ = start;
    return *this;
  }
  /// Sets `start` (no 1) and returns `*this`.
  Range&& set_start(double start) && noexcept {
    return std::move(set_start(start));
  }

  // Field `end` (no 2).
  // -----

  /// No documentation.
  ///
  /// Field no: 2.
  constexpr double end() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return end_;
  }

  /// No documentation.
  ///
  /// Field no: 2.
  double& mutable_end() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[1] = true;
    return end_;
  }

  /// Returns whether `end` (no 2) is set.
  constexpr bool has_end() const noexcept { return set_fields_[1]; }

  /// Clears `end` (no 2).
  void clear_end() & noexcept {
    set_fields_[1] = false;
    end_ = {};
  }

  /// Sets `end` (no 2) and returns `*this`.
  Range& set_end(double end) & noexcept {
    set_fields_[1] = true;
    end_ = end;
    return *this;
  }
  /// Sets `end` (no 2) and returns `*this`.
  Range&& set_end(double end) && noexcept {
    return std::move(set_end(end));
  }

 private:
  /// @see start()
  double start_{};
  /// @see end()
  double end_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<2> set_fields_;
};

/// Contains a list of `uint32` values. Used when a list of repeated `uint32`
///  values should be `optional`.
///
/// Source: horus/pb/config/metadata.proto:34:1
class UInt32List final : public PbMessage {
 public:

  /// Constructs a default-initialized `UInt32List`.
  UInt32List() noexcept = default;

  /// Move constructor.
  UInt32List(UInt32List&&) noexcept = default;
  /// Move assignment operator.
  UInt32List& operator=(UInt32List&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit UInt32List(const UInt32List& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  UInt32List& operator=(const UInt32List&) = delete;

  /// Default destructor.
  ~UInt32List() noexcept final = default;

  /// Creates a `UInt32List` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit UInt32List(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.UInt32List`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.UInt32List"; }

  /// The full name of the message: `horus.pb.UInt32List`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `values` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr const CowRepeated<std::uint32_t>& values() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return values_;
  }

  /// If `values` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  CowRepeated<std::uint32_t> values() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(values_);
  }

  /// No documentation.
  ///
  /// Field no: 1.
  CowRepeated<std::uint32_t>& mutable_values() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return values_;
  }

  /// Returns whether `values` (no 1) is set.
  constexpr bool has_values() const noexcept { return set_fields_[0]; }

  /// Clears `values` (no 1).
  void clear_values() & noexcept {
    set_fields_[0] = false;
    values_ = {};
  }

  /// Sets `values` (no 1) and returns `*this`.
  UInt32List& set_values(CowRepeated<std::uint32_t>&& values) & noexcept {
    set_fields_[0] = true;
    values_ = std::move(values);
    return *this;
  }
  /// Sets `values` (no 1) and returns `*this`.
  UInt32List&& set_values(CowRepeated<std::uint32_t>&& values) && noexcept {
    return std::move(set_values(std::move(values)));
  }

 private:
  /// @see values()
  CowRepeated<std::uint32_t> values_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<1> set_fields_;
};

/// A timestamp.
/// 
///  We do not use google.protobuf.Timestamp as it is defined in libprotobuf, but
///  not libprotobuf-lite, and generating metadata for such messages would lead to
///  binary conflicts.
///
/// Source: horus/pb/config/metadata.proto:44:1
class Timestamp final : public PbMessage {
 public:

  /// Constructs a default-initialized `Timestamp`.
  Timestamp() noexcept = default;

  /// Move constructor.
  Timestamp(Timestamp&&) noexcept = default;
  /// Move assignment operator.
  Timestamp& operator=(Timestamp&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit Timestamp(const Timestamp& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  Timestamp& operator=(const Timestamp&) = delete;

  /// Default destructor.
  ~Timestamp() noexcept final = default;

  /// Creates a `Timestamp` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit Timestamp(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.Timestamp`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.Timestamp"; }

  /// The full name of the message: `horus.pb.Timestamp`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `seconds` (no 1).
  // -----

  /// Number of seconds between the represented time and the epoch
  ///  (1970-01-01 00:00) in UTC.
  ///
  /// Field no: 1.
  constexpr std::int64_t seconds() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return seconds_;
  }

  /// Number of seconds between the represented time and the epoch
  ///  (1970-01-01 00:00) in UTC.
  ///
  /// Field no: 1.
  std::int64_t& mutable_seconds() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return seconds_;
  }

  /// Returns whether `seconds` (no 1) is set.
  constexpr bool has_seconds() const noexcept { return set_fields_[0]; }

  /// Clears `seconds` (no 1).
  void clear_seconds() & noexcept {
    set_fields_[0] = false;
    seconds_ = {};
  }

  /// Sets `seconds` (no 1) and returns `*this`.
  Timestamp& set_seconds(std::int64_t seconds) & noexcept {
    set_fields_[0] = true;
    seconds_ = seconds;
    return *this;
  }
  /// Sets `seconds` (no 1) and returns `*this`.
  Timestamp&& set_seconds(std::int64_t seconds) && noexcept {
    return std::move(set_seconds(seconds));
  }

  // Field `nanos` (no 2).
  // -----

  /// Non-negative fractions of a second at nanosecond resolution.
  ///
  /// Field no: 2.
  constexpr std::int32_t nanos() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return nanos_;
  }

  /// Non-negative fractions of a second at nanosecond resolution.
  ///
  /// Field no: 2.
  std::int32_t& mutable_nanos() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[1] = true;
    return nanos_;
  }

  /// Returns whether `nanos` (no 2) is set.
  constexpr bool has_nanos() const noexcept { return set_fields_[1]; }

  /// Clears `nanos` (no 2).
  void clear_nanos() & noexcept {
    set_fields_[1] = false;
    nanos_ = {};
  }

  /// Sets `nanos` (no 2) and returns `*this`.
  Timestamp& set_nanos(std::int32_t nanos) & noexcept {
    set_fields_[1] = true;
    nanos_ = nanos;
    return *this;
  }
  /// Sets `nanos` (no 2) and returns `*this`.
  Timestamp&& set_nanos(std::int32_t nanos) && noexcept {
    return std::move(set_nanos(nanos));
  }

 private:
  /// @see seconds()
  std::int64_t seconds_{};
  /// @see nanos()
  std::int32_t nanos_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<2> set_fields_;
};

/// A duration.
/// 
///  We do not use google.protobuf.Duration as it is defined in libprotobuf, but
///  not libprotobuf-lite, and generating metadata for such messages would lead to
///  binary conflicts.
///
/// Source: horus/pb/config/metadata.proto:58:1
class Duration final : public PbMessage {
 public:

  /// Constructs a default-initialized `Duration`.
  Duration() noexcept = default;

  /// Move constructor.
  Duration(Duration&&) noexcept = default;
  /// Move assignment operator.
  Duration& operator=(Duration&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit Duration(const Duration& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  Duration& operator=(const Duration&) = delete;

  /// Default destructor.
  ~Duration() noexcept final = default;

  /// Creates a `Duration` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit Duration(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.Duration`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.Duration"; }

  /// The full name of the message: `horus.pb.Duration`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `seconds` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr std::int64_t seconds() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return seconds_;
  }

  /// No documentation.
  ///
  /// Field no: 1.
  std::int64_t& mutable_seconds() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return seconds_;
  }

  /// Returns whether `seconds` (no 1) is set.
  constexpr bool has_seconds() const noexcept { return set_fields_[0]; }

  /// Clears `seconds` (no 1).
  void clear_seconds() & noexcept {
    set_fields_[0] = false;
    seconds_ = {};
  }

  /// Sets `seconds` (no 1) and returns `*this`.
  Duration& set_seconds(std::int64_t seconds) & noexcept {
    set_fields_[0] = true;
    seconds_ = seconds;
    return *this;
  }
  /// Sets `seconds` (no 1) and returns `*this`.
  Duration&& set_seconds(std::int64_t seconds) && noexcept {
    return std::move(set_seconds(seconds));
  }

  // Field `nanos` (no 2).
  // -----

  /// No documentation.
  ///
  /// Field no: 2.
  constexpr std::int32_t nanos() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return nanos_;
  }

  /// No documentation.
  ///
  /// Field no: 2.
  std::int32_t& mutable_nanos() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[1] = true;
    return nanos_;
  }

  /// Returns whether `nanos` (no 2) is set.
  constexpr bool has_nanos() const noexcept { return set_fields_[1]; }

  /// Clears `nanos` (no 2).
  void clear_nanos() & noexcept {
    set_fields_[1] = false;
    nanos_ = {};
  }

  /// Sets `nanos` (no 2) and returns `*this`.
  Duration& set_nanos(std::int32_t nanos) & noexcept {
    set_fields_[1] = true;
    nanos_ = nanos;
    return *this;
  }
  /// Sets `nanos` (no 2) and returns `*this`.
  Duration&& set_nanos(std::int32_t nanos) && noexcept {
    return std::move(set_nanos(nanos));
  }

 private:
  /// @see seconds()
  std::int64_t seconds_{};
  /// @see nanos()
  std::int32_t nanos_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<2> set_fields_;
};

/// Matrix types
///  ------------
///
/// Source: horus/pb/config/metadata.proto:67:1
class AffineTransform3f final : public PbMessage {
 public:

  /// Constructs a default-initialized `AffineTransform3f`.
  AffineTransform3f() noexcept = default;

  /// Move constructor.
  AffineTransform3f(AffineTransform3f&&) noexcept = default;
  /// Move assignment operator.
  AffineTransform3f& operator=(AffineTransform3f&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit AffineTransform3f(const AffineTransform3f& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  AffineTransform3f& operator=(const AffineTransform3f&) = delete;

  /// Default destructor.
  ~AffineTransform3f() noexcept final = default;

  /// Creates a `AffineTransform3f` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit AffineTransform3f(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.AffineTransform3f`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.AffineTransform3f"; }

  /// The full name of the message: `horus.pb.AffineTransform3f`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `data` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr const CowSpan<float>& data() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return data_;
  }

  /// If `data` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  CowSpan<float> data() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(data_);
  }

  /// No documentation.
  ///
  /// Field no: 1.
  CowSpan<float>& mutable_data() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return data_;
  }

  /// Returns whether `data` (no 1) is set.
  constexpr bool has_data() const noexcept { return set_fields_[0]; }

  /// Clears `data` (no 1).
  void clear_data() & noexcept {
    set_fields_[0] = false;
    data_ = {};
  }

  /// Sets `data` (no 1) and returns `*this`.
  AffineTransform3f& set_data(CowSpan<float>&& data) & noexcept {
    set_fields_[0] = true;
    data_ = std::move(data);
    return *this;
  }
  /// Sets `data` (no 1) and returns `*this`.
  AffineTransform3f&& set_data(CowSpan<float>&& data) && noexcept {
    return std::move(set_data(std::move(data)));
  }

 private:
  /// @see data()
  CowSpan<float> data_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<1> set_fields_;
};

}  // namespace pb
}  // namespace sdk
}  // namespace horus

// NOLINTEND(readability-identifier-length)

#endif  // HORUS_PB_CONFIG_METADATA_PB_H_
