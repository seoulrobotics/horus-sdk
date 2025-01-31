#ifndef HORUS_PB_CONFIG_SCHEMA_GENERAL_PB_H_
#define HORUS_PB_CONFIG_SCHEMA_GENERAL_PB_H_

/// @file
///
/// C++ type definitions for Protobuf messages in `horus/pb/config/schema/general.proto`.
///
/// Generated by `/opt/actions-runner/_work/horus/horus/common/tools/sdk/pb.ts`.

#include <bitset>
#include <cstdint>
#include <utility>

#include "horus/pb/config/metadata_pb.h"
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

class DetectionRange;

// MARK: Message declarations

/// No documentation.
///
/// Source: horus/pb/config/schema/general.proto:7:1
class DetectionRange final : public PbMessage {
 public:

  /// Constructs a default-initialized `DetectionRange`.
  DetectionRange() noexcept = default;

  /// Move constructor.
  DetectionRange(DetectionRange&&) noexcept = default;
  /// Move assignment operator.
  DetectionRange& operator=(DetectionRange&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit DetectionRange(const DetectionRange& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  DetectionRange& operator=(const DetectionRange&) = delete;

  /// Default destructor.
  ~DetectionRange() noexcept final = default;

  /// Creates a `DetectionRange` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit DetectionRange(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.DetectionRange`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.DetectionRange"; }

  /// The full name of the message: `horus.pb.DetectionRange`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `x_range` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr const Range& x_range() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return x_range_;
  }

  /// If `x_range` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  Range x_range() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(x_range_);
  }

  /// No documentation.
  ///
  /// Field no: 1.
  Range& mutable_x_range() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return x_range_;
  }

  /// Returns whether `x_range` (no 1) is set.
  constexpr bool has_x_range() const noexcept { return set_fields_[0]; }

  /// Clears `x_range` (no 1).
  void clear_x_range() & noexcept {
    set_fields_[0] = false;
    x_range_ = {};
  }

  /// Sets `x_range` (no 1) and returns `*this`.
  DetectionRange& set_x_range(Range&& x_range) & noexcept {
    set_fields_[0] = true;
    x_range_ = std::move(x_range);
    return *this;
  }
  /// Sets `x_range` (no 1) and returns `*this`.
  DetectionRange&& set_x_range(Range&& x_range) && noexcept {
    return std::move(set_x_range(std::move(x_range)));
  }

  // Field `y_range` (no 2).
  // -----

  /// No documentation.
  ///
  /// Field no: 2.
  constexpr const Range& y_range() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return y_range_;
  }

  /// If `y_range` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 2.
  Range y_range() && noexcept {
    if (!set_fields_[1]) {
      return {};
    }
    return std::move(y_range_);
  }

  /// No documentation.
  ///
  /// Field no: 2.
  Range& mutable_y_range() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[1] = true;
    return y_range_;
  }

  /// Returns whether `y_range` (no 2) is set.
  constexpr bool has_y_range() const noexcept { return set_fields_[1]; }

  /// Clears `y_range` (no 2).
  void clear_y_range() & noexcept {
    set_fields_[1] = false;
    y_range_ = {};
  }

  /// Sets `y_range` (no 2) and returns `*this`.
  DetectionRange& set_y_range(Range&& y_range) & noexcept {
    set_fields_[1] = true;
    y_range_ = std::move(y_range);
    return *this;
  }
  /// Sets `y_range` (no 2) and returns `*this`.
  DetectionRange&& set_y_range(Range&& y_range) && noexcept {
    return std::move(set_y_range(std::move(y_range)));
  }

  // Field `z_range` (no 3).
  // -----

  /// No documentation.
  ///
  /// Field no: 3.
  constexpr const Range& z_range() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return z_range_;
  }

  /// If `z_range` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 3.
  Range z_range() && noexcept {
    if (!set_fields_[2]) {
      return {};
    }
    return std::move(z_range_);
  }

  /// No documentation.
  ///
  /// Field no: 3.
  Range& mutable_z_range() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[2] = true;
    return z_range_;
  }

  /// Returns whether `z_range` (no 3) is set.
  constexpr bool has_z_range() const noexcept { return set_fields_[2]; }

  /// Clears `z_range` (no 3).
  void clear_z_range() & noexcept {
    set_fields_[2] = false;
    z_range_ = {};
  }

  /// Sets `z_range` (no 3) and returns `*this`.
  DetectionRange& set_z_range(Range&& z_range) & noexcept {
    set_fields_[2] = true;
    z_range_ = std::move(z_range);
    return *this;
  }
  /// Sets `z_range` (no 3) and returns `*this`.
  DetectionRange&& set_z_range(Range&& z_range) && noexcept {
    return std::move(set_z_range(std::move(z_range)));
  }

 private:
  /// @see x_range()
  Range x_range_{};
  /// @see y_range()
  Range y_range_{};
  /// @see z_range()
  Range z_range_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<3> set_fields_;
};

}  // namespace pb
}  // namespace sdk
}  // namespace horus

// NOLINTEND(readability-identifier-length)

#endif  // HORUS_PB_CONFIG_SCHEMA_GENERAL_PB_H_
