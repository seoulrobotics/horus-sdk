#ifndef HORUS_PB_LOGS_COMMON_PB_H_
#define HORUS_PB_LOGS_COMMON_PB_H_

/// @file
///
/// C++ type definitions for Protobuf messages in `horus/pb/logs/common.proto`.
///
/// Generated by `/opt/actions-runner/_work/horus/horus/common/tools/sdk/pb.ts`.

#include <bitset>
#include <cstdint>
#include <utility>

#include "horus/pb/cow_bytes.h"
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
namespace logs {

// MARK: Message forward declarations

class ValidationError;

// MARK: Message declarations

/// Validation error which maps a message to a path in a nested object.
///
/// Source: horus/pb/logs/common.proto:6:1
class ValidationError final : public PbMessage {
 public:

  /// Constructs a default-initialized `ValidationError`.
  ValidationError() noexcept = default;

  /// Move constructor.
  ValidationError(ValidationError&&) noexcept = default;
  /// Move assignment operator.
  ValidationError& operator=(ValidationError&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit ValidationError(const ValidationError& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  ValidationError& operator=(const ValidationError&) = delete;

  /// Default destructor.
  ~ValidationError() noexcept final = default;

  /// Creates a `ValidationError` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit ValidationError(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.logs.ValidationError`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.logs.ValidationError"; }

  /// The full name of the message: `horus.pb.logs.ValidationError`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `path` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr const CowBytes& path() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return path_;
  }

  /// If `path` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  CowBytes path() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(path_);
  }

  /// No documentation.
  ///
  /// Field no: 1.
  CowBytes& mutable_path() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[0] = true;
    return path_;
  }

  /// Returns whether `path` (no 1) is set.
  constexpr bool has_path() const noexcept { return set_fields_[0]; }

  /// Clears `path` (no 1).
  void clear_path() & noexcept {
    set_fields_[0] = false;
    path_ = {};
  }

  /// Sets `path` (no 1) and returns `*this`.
  ValidationError& set_path(CowBytes&& path) & noexcept {
    set_fields_[0] = true;
    path_ = std::move(path);
    return *this;
  }
  /// Sets `path` (no 1) and returns `*this`.
  ValidationError&& set_path(CowBytes&& path) && noexcept {
    return std::move(set_path(std::move(path)));
  }

  // Field `message` (no 2).
  // -----

  /// No documentation.
  ///
  /// Field no: 2.
  constexpr const CowBytes& message() const& noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    return message_;
  }

  /// If `message` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 2.
  CowBytes message() && noexcept {
    if (!set_fields_[1]) {
      return {};
    }
    return std::move(message_);
  }

  /// No documentation.
  ///
  /// Field no: 2.
  CowBytes& mutable_message() & noexcept HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND {
    set_fields_[1] = true;
    return message_;
  }

  /// Returns whether `message` (no 2) is set.
  constexpr bool has_message() const noexcept { return set_fields_[1]; }

  /// Clears `message` (no 2).
  void clear_message() & noexcept {
    set_fields_[1] = false;
    message_ = {};
  }

  /// Sets `message` (no 2) and returns `*this`.
  ValidationError& set_message(CowBytes&& message) & noexcept {
    set_fields_[1] = true;
    message_ = std::move(message);
    return *this;
  }
  /// Sets `message` (no 2) and returns `*this`.
  ValidationError&& set_message(CowBytes&& message) && noexcept {
    return std::move(set_message(std::move(message)));
  }

 private:
  /// @see path()
  CowBytes path_{};
  /// @see message()
  CowBytes message_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<2> set_fields_;
};

}  // namespace logs
}  // namespace pb
}  // namespace sdk
}  // namespace horus

// NOLINTEND(readability-identifier-length)

#endif  // HORUS_PB_LOGS_COMMON_PB_H_
