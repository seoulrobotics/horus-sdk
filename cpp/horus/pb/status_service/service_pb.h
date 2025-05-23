#ifndef HORUS_PB_STATUS_SERVICE_SERVICE_PB_H_
#define HORUS_PB_STATUS_SERVICE_SERVICE_PB_H_

/// @file
///
/// C++ type definitions for Protobuf messages in `horus/pb/status_service/service.proto`.
///
/// Generated by `/opt/actions-runner/_work/horus/horus/common/tools/sdk/pb.ts`.

#include <bitset>
#include <cstdint>
#include <utility>

#include "horus/attributes.h"
#include "horus/internal/attributes.h"
#include "horus/pb/message.h"
#include "horus/pb/serialize.h"
#include "horus/pb/status_service/messages_pb.h"
#include "horus/strings/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

// NOLINTBEGIN(readability-identifier-length)

namespace horus {
namespace sdk {
namespace pb {

// MARK: Message forward declarations

class GetVersionRequest;
class GetVersionResponse;

// MARK: Message declarations

/// No documentation.
///
/// Source: horus/pb/status_service/service.proto:22:1
class GetVersionRequest final : public PbMessage {
 public:

  /// Constructs a default-initialized `GetVersionRequest`.
  GetVersionRequest() noexcept = default;

  /// Move constructor.
  GetVersionRequest(GetVersionRequest&&) noexcept = default;
  /// Move assignment operator.
  GetVersionRequest& operator=(GetVersionRequest&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit GetVersionRequest(const GetVersionRequest&) noexcept = default;  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  GetVersionRequest& operator=(const GetVersionRequest&) = delete;

  /// Default destructor.
  ~GetVersionRequest() noexcept final = default;

  /// Creates a `GetVersionRequest` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit GetVersionRequest(PbReader& reader) noexcept(false) : PbMessage{} {
    DeserializeFrom(reader);
  }

  /// Serializes the message to `writer`.
  ///
  /// @throws std::bad_alloc If the resulting buffer failed to allocate.
  void SerializeTo(PbWriter& writer) const noexcept(false) final {
    static_cast<void>(writer);
  }

  /// Deserializes the message from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  void DeserializeFrom(PbReader& reader) noexcept(false) final {
    reader.SkipMessage();
  }

  /// Returns whether the message is empty.
  bool IsEmpty() const noexcept final { return set_fields_.none(); }

  /// The full name of the message: `horus.pb.GetVersionRequest`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.GetVersionRequest"; }

  /// The full name of the message: `horus.pb.GetVersionRequest`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

 private:

  /// The set of fields that have been given an explicit value.
  std::bitset<0> set_fields_;
};

/// No documentation.
///
/// Source: horus/pb/status_service/service.proto:24:1
class GetVersionResponse final : public PbMessage {
 public:

  /// Constructs a default-initialized `GetVersionResponse`.
  GetVersionResponse() noexcept = default;

  /// Move constructor.
  GetVersionResponse(GetVersionResponse&&) noexcept = default;
  /// Move assignment operator.
  GetVersionResponse& operator=(GetVersionResponse&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit GetVersionResponse(const GetVersionResponse& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  GetVersionResponse& operator=(const GetVersionResponse&) = delete;

  /// Default destructor.
  ~GetVersionResponse() noexcept final = default;

  /// Creates a `GetVersionResponse` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit GetVersionResponse(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.GetVersionResponse`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.GetVersionResponse"; }

  /// The full name of the message: `horus.pb.GetVersionResponse`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `version` (no 1).
  // -----

  /// No documentation.
  ///
  /// Field no: 1.
  constexpr const Version& version() const& noexcept HORUS_LIFETIME_BOUND {
    return version_;
  }

  /// If `version` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 1.
  Version version() && noexcept {
    if (!set_fields_[0]) {
      return {};
    }
    return std::move(version_);
  }

  /// No documentation.
  ///
  /// Field no: 1.
  Version& mutable_version() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[0] = true;
    return version_;
  }

  /// Returns whether `version` (no 1) is set.
  constexpr bool has_version() const noexcept { return set_fields_[0]; }

  /// Clears `version` (no 1).
  void clear_version() & noexcept {
    set_fields_[0] = false;
    version_ = {};
  }

  /// Sets `version` (no 1) and returns `*this`.
  GetVersionResponse& set_version(Version&& version) & noexcept {
    set_fields_[0] = true;
    version_ = std::move(version);
    return *this;
  }
  /// Sets `version` (no 1) and returns `*this`.
  GetVersionResponse&& set_version(Version&& version) && noexcept {
    return std::move(set_version(std::move(version)));
  }

 private:
  /// @see version()
  Version version_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<1> set_fields_;
};

}  // namespace pb
}  // namespace sdk
}  // namespace horus

// NOLINTEND(readability-identifier-length)

#endif  // HORUS_PB_STATUS_SERVICE_SERVICE_PB_H_
