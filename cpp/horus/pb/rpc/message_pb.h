#ifndef HORUS_PB_RPC_MESSAGE_PB_H_
#define HORUS_PB_RPC_MESSAGE_PB_H_

/// @file
///
/// C++ type definitions for Protobuf messages in `horus/pb/rpc/message.proto`.
///
/// Generated by `/home/runner/work/horus/horus/common/tools/sdk/pb.ts`.

#include <bitset>
#include <cstdint>
#include <utility>

#include "horus/attributes.h"
#include "horus/internal/attributes.h"
#include "horus/pb/cow_bytes.h"
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

/// No documentation.
///
/// Source: horus/pb/rpc/message.proto:8:3
enum class RpcMessage_Version : PbEnum {  // NOLINT(*-enum-size)
  /// No documentation.
  kUnspecified = 0,
  /// No documentation.
  kOne = 1,

  /// Unknown value read from the wire.
  kUnknownWireValue = 2,
};

// MARK: Message forward declarations

class RpcMessage;

// MARK: Message declarations

/// A message used to implement RPCs, sent from the client to the server or
///  vice-versa.
///
/// Source: horus/pb/rpc/message.proto:7:1
class RpcMessage final : public PbMessage {
 public:
  /// @see RpcMessage_Version
  using Version = RpcMessage_Version;

  /// Constructs a default-initialized `RpcMessage`.
  RpcMessage() noexcept = default;

  /// Move constructor.
  RpcMessage(RpcMessage&&) noexcept = default;
  /// Move assignment operator.
  RpcMessage& operator=(RpcMessage&&) noexcept = default;

  /// Constructs a clone of `other`.
  ///
  /// @throws std::bad_alloc If `other` owns heap-allocated data which could not be cloned due to a
  /// lack of available memory.
  explicit RpcMessage(const RpcMessage& other) noexcept(false);  // NOLINT(*-explicit-*)

  /// Cannot copy-assign to avoid implicit allocations.
  RpcMessage& operator=(const RpcMessage&) = delete;

  /// Default destructor.
  ~RpcMessage() noexcept final = default;

  /// Creates a `RpcMessage` whose contents are read from `reader`.
  ///
  /// @throws InvalidProtobufMessage If the `reader` contains an invalid Protobuf message.
  explicit RpcMessage(PbReader& reader) noexcept(false) : PbMessage{} {
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

  /// The full name of the message: `horus.pb.RpcMessage`.
  static constexpr StringView TypeName() noexcept { return "horus.pb.RpcMessage"; }

  /// The full name of the message: `horus.pb.RpcMessage`.
  StringView MessageTypeName() const noexcept final { return TypeName(); }

  // Field `version` (no 1).
  // -----

  /// The version of the message.
  ///
  /// Field no: 1.
  constexpr RpcMessage_Version version() const& noexcept HORUS_LIFETIME_BOUND {
    return version_;
  }

  /// The version of the message.
  ///
  /// Field no: 1.
  RpcMessage_Version& mutable_version() & noexcept HORUS_LIFETIME_BOUND {
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
  RpcMessage& set_version(RpcMessage_Version version) & noexcept {
    set_fields_[0] = true;
    version_ = version;
    return *this;
  }
  /// Sets `version` (no 1) and returns `*this`.
  RpcMessage&& set_version(RpcMessage_Version version) && noexcept {
    return std::move(set_version(version));
  }

  // Field `service_id` (no 2).
  // -----

  /// The ID of the service being talked to.
  ///
  /// Field no: 2.
  constexpr std::uint32_t service_id() const& noexcept HORUS_LIFETIME_BOUND {
    return service_id_;
  }

  /// The ID of the service being talked to.
  ///
  /// Field no: 2.
  std::uint32_t& mutable_service_id() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[1] = true;
    return service_id_;
  }

  /// Returns whether `service_id` (no 2) is set.
  constexpr bool has_service_id() const noexcept { return set_fields_[1]; }

  /// Clears `service_id` (no 2).
  void clear_service_id() & noexcept {
    set_fields_[1] = false;
    service_id_ = {};
  }

  /// Sets `service_id` (no 2) and returns `*this`.
  RpcMessage& set_service_id(std::uint32_t service_id) & noexcept {
    set_fields_[1] = true;
    service_id_ = service_id;
    return *this;
  }
  /// Sets `service_id` (no 2) and returns `*this`.
  RpcMessage&& set_service_id(std::uint32_t service_id) && noexcept {
    return std::move(set_service_id(service_id));
  }

  // Field `message_bytes` (no 3).
  // -----

  /// The bytes making up the request or response. The type is determined by the
  ///  receiver.
  ///
  /// Field no: 3.
  constexpr const CowBytes& message_bytes() const& noexcept HORUS_LIFETIME_BOUND {
    return message_bytes_;
  }

  /// If `message_bytes` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 3.
  CowBytes message_bytes() && noexcept {
    if (!set_fields_[2]) {
      return {};
    }
    return std::move(message_bytes_);
  }

  /// The bytes making up the request or response. The type is determined by the
  ///  receiver.
  ///
  /// Field no: 3.
  CowBytes& mutable_message_bytes() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[2] = true;
    return message_bytes_;
  }

  /// Returns whether `message_bytes` (no 3) is set.
  constexpr bool has_message_bytes() const noexcept { return set_fields_[2]; }

  /// Clears `message_bytes` (no 3).
  void clear_message_bytes() & noexcept {
    set_fields_[2] = false;
    message_bytes_ = {};
  }

  /// Sets `message_bytes` (no 3) and returns `*this`.
  RpcMessage& set_message_bytes(CowBytes&& message_bytes) & noexcept {
    set_fields_[2] = true;
    message_bytes_ = std::move(message_bytes);
    return *this;
  }
  /// Sets `message_bytes` (no 3) and returns `*this`.
  RpcMessage&& set_message_bytes(CowBytes&& message_bytes) && noexcept {
    return std::move(set_message_bytes(std::move(message_bytes)));
  }

  // Field `request_id` (no 4).
  // -----

  /// The ID of the request (may also be set for responses to specify which
  ///  request is being replied to). Specify 0 to indicate this request expects no
  ///  response.
  ///
  /// Field no: 4.
  constexpr std::uint32_t request_id() const& noexcept HORUS_LIFETIME_BOUND {
    return request_id_;
  }

  /// The ID of the request (may also be set for responses to specify which
  ///  request is being replied to). Specify 0 to indicate this request expects no
  ///  response.
  ///
  /// Field no: 4.
  std::uint32_t& mutable_request_id() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[3] = true;
    return request_id_;
  }

  /// Returns whether `request_id` (no 4) is set.
  constexpr bool has_request_id() const noexcept { return set_fields_[3]; }

  /// Clears `request_id` (no 4).
  void clear_request_id() & noexcept {
    set_fields_[3] = false;
    request_id_ = {};
  }

  /// Sets `request_id` (no 4) and returns `*this`.
  RpcMessage& set_request_id(std::uint32_t request_id) & noexcept {
    set_fields_[3] = true;
    request_id_ = request_id;
    return *this;
  }
  /// Sets `request_id` (no 4) and returns `*this`.
  RpcMessage&& set_request_id(std::uint32_t request_id) && noexcept {
    return std::move(set_request_id(request_id));
  }

  // Field `method_id` (no 5).
  // -----

  /// The ID of the method being called. Only set in requests.
  ///
  /// Field no: 5.
  constexpr std::uint32_t method_id() const& noexcept HORUS_LIFETIME_BOUND {
    return method_id_;
  }

  /// The ID of the method being called. Only set in requests.
  ///
  /// Field no: 5.
  std::uint32_t& mutable_method_id() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[4] = true;
    return method_id_;
  }

  /// Returns whether `method_id` (no 5) is set.
  constexpr bool has_method_id() const noexcept { return set_fields_[4]; }

  /// Clears `method_id` (no 5).
  void clear_method_id() & noexcept {
    set_fields_[4] = false;
    method_id_ = {};
  }

  /// Sets `method_id` (no 5) and returns `*this`.
  RpcMessage& set_method_id(std::uint32_t method_id) & noexcept {
    set_fields_[4] = true;
    method_id_ = method_id;
    return *this;
  }
  /// Sets `method_id` (no 5) and returns `*this`.
  RpcMessage&& set_method_id(std::uint32_t method_id) && noexcept {
    return std::move(set_method_id(method_id));
  }

  // Field `error` (no 6).
  // -----

  /// An error encountered while processing a request which expects a response.
  ///  This field will be set when the handler of a request fails to provide a
  ///  response for any reason. Such errors are **internal errors**, and should
  ///  never occur. However, RPC endpoints need a mechanism to stop awaiting
  ///  pending requests in case of error, which this field provides.
  ///
  /// Field no: 6.
  constexpr const CowBytes& error() const& noexcept HORUS_LIFETIME_BOUND {
    return error_;
  }

  /// If `error` is set, moves it out of the message (without marking it as unset).
  ///
  /// Otherwise, returns a default-initialized value.
  ///
  /// Field no: 6.
  CowBytes error() && noexcept {
    if (!set_fields_[5]) {
      return {};
    }
    return std::move(error_);
  }

  /// An error encountered while processing a request which expects a response.
  ///  This field will be set when the handler of a request fails to provide a
  ///  response for any reason. Such errors are **internal errors**, and should
  ///  never occur. However, RPC endpoints need a mechanism to stop awaiting
  ///  pending requests in case of error, which this field provides.
  ///
  /// Field no: 6.
  CowBytes& mutable_error() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[5] = true;
    return error_;
  }

  /// Returns whether `error` (no 6) is set.
  constexpr bool has_error() const noexcept { return set_fields_[5]; }

  /// Clears `error` (no 6).
  void clear_error() & noexcept {
    set_fields_[5] = false;
    error_ = {};
  }

  /// Sets `error` (no 6) and returns `*this`.
  RpcMessage& set_error(CowBytes&& error) & noexcept {
    set_fields_[5] = true;
    error_ = std::move(error);
    return *this;
  }
  /// Sets `error` (no 6) and returns `*this`.
  RpcMessage&& set_error(CowBytes&& error) && noexcept {
    return std::move(set_error(std::move(error)));
  }

  // Field `cancel` (no 7).
  // -----

  /// Whether this message is a cancellation request, in which case
  ///  `request_id` must be set and non-zero. The response to the relevant
  ///  request will be ignored, but **must nonetheless be sent to the client**.
  /// 
  ///  A response must still be provided following a cancellation request
  ///  because it may already be on its way to the client. If the client
  ///  immediately made the `request_id` available for new requests without
  ///  waiting for a response, it would be possible for the following to happen:
  /// 
  ///  1. Alice sends a request Foo with ID #1 to Bob.
  ///  2. Bob computes a response and enqueues it.
  ///  3. Alice sends a cancellation request for #1, making #1 available for
  ///     re-use.
  ///  4. Alice sends a new request Bar with the same reused ID #1 to Bob.
  ///  5. Bob responds to Foo.
  ///  6. Alice receives a FooResponse while it expects a BarResponse.
  ///
  /// Field no: 7.
  constexpr bool cancel() const& noexcept HORUS_LIFETIME_BOUND {
    return cancel_;
  }

  /// Whether this message is a cancellation request, in which case
  ///  `request_id` must be set and non-zero. The response to the relevant
  ///  request will be ignored, but **must nonetheless be sent to the client**.
  /// 
  ///  A response must still be provided following a cancellation request
  ///  because it may already be on its way to the client. If the client
  ///  immediately made the `request_id` available for new requests without
  ///  waiting for a response, it would be possible for the following to happen:
  /// 
  ///  1. Alice sends a request Foo with ID #1 to Bob.
  ///  2. Bob computes a response and enqueues it.
  ///  3. Alice sends a cancellation request for #1, making #1 available for
  ///     re-use.
  ///  4. Alice sends a new request Bar with the same reused ID #1 to Bob.
  ///  5. Bob responds to Foo.
  ///  6. Alice receives a FooResponse while it expects a BarResponse.
  ///
  /// Field no: 7.
  bool& mutable_cancel() & noexcept HORUS_LIFETIME_BOUND {
    set_fields_[6] = true;
    return cancel_;
  }

  /// Returns whether `cancel` (no 7) is set.
  constexpr bool has_cancel() const noexcept { return set_fields_[6]; }

  /// Clears `cancel` (no 7).
  void clear_cancel() & noexcept {
    set_fields_[6] = false;
    cancel_ = {};
  }

  /// Sets `cancel` (no 7) and returns `*this`.
  RpcMessage& set_cancel(bool cancel) & noexcept {
    set_fields_[6] = true;
    cancel_ = cancel;
    return *this;
  }
  /// Sets `cancel` (no 7) and returns `*this`.
  RpcMessage&& set_cancel(bool cancel) && noexcept {
    return std::move(set_cancel(cancel));
  }

 private:
  /// @see version()
  RpcMessage_Version version_{};
  /// @see service_id()
  std::uint32_t service_id_{};
  /// @see message_bytes()
  CowBytes message_bytes_{};
  /// @see request_id()
  std::uint32_t request_id_{};
  /// @see method_id()
  std::uint32_t method_id_{};
  /// @see error()
  CowBytes error_{};
  /// @see cancel()
  bool cancel_{};

  /// The set of fields that have been given an explicit value.
  std::bitset<7> set_fields_;
};

}  // namespace pb
}  // namespace sdk
}  // namespace horus

// MARK: Enum traits

namespace horus {

template <>
class PbEnumTraits<horus::sdk::pb::RpcMessage_Version> final {
 public:
  /// The full name of the enum: `horus.sdk.pb.RpcMessage.Version`.
  static constexpr StringView EnumName() noexcept { return "horus.sdk.pb.RpcMessage.Version"; }

  /// Returns the name of the given enumerator, or an empty string.
  static constexpr StringView NameOf(horus::sdk::pb::RpcMessage_Version value) noexcept {
    switch (value) {
      case horus::sdk::pb::RpcMessage_Version::kUnspecified: {
        return "VERSION_UNSPECIFIED";
      }
      case horus::sdk::pb::RpcMessage_Version::kOne: {
        return "VERSION_ONE";
      }
      case horus::sdk::pb::RpcMessage_Version::kUnknownWireValue:
      default: {
        return "";
      }
    }
  }

  /// Returns the value corresponding to the given name, or `default_value`.
  static constexpr horus::sdk::pb::RpcMessage_Version ValueOf(PbEnum value, horus::sdk::pb::RpcMessage_Version default_value = horus::sdk::pb::RpcMessage_Version::kUnknownWireValue) noexcept {
    switch (value) {
      case 0: {
        return horus::sdk::pb::RpcMessage_Version::kUnspecified;
      }
      case 1: {
        return horus::sdk::pb::RpcMessage_Version::kOne;
      }
      default: {
        return default_value;
      }
    }
  }

  /// Returns the value corresponding to the given name, or `default_value`.
  static constexpr horus::sdk::pb::RpcMessage_Version ValueOf(StringView name, horus::sdk::pb::RpcMessage_Version default_value = horus::sdk::pb::RpcMessage_Version::kUnknownWireValue) noexcept {
    if (name == "VERSION_UNSPECIFIED") {
      return horus::sdk::pb::RpcMessage_Version::kUnspecified;
    }
    if (name == "VERSION_ONE") {
      return horus::sdk::pb::RpcMessage_Version::kOne;
    }
    return default_value;
  }
};

template <>
class PbTraits<horus::sdk::pb::RpcMessage_Version> final {
 public:
  /// Serializes `value` into `writer`.
  static void Serialize(PbWriter& writer, PbTag tag, horus::sdk::pb::RpcMessage_Version value) {
    writer.Writer().add_enum(tag, static_cast<PbEnum>(value));
  }

  /// Deserializes `horus::sdk::pb::RpcMessage_Version` from `reader`.
  static horus::sdk::pb::RpcMessage_Version Deserialize(PbReader& reader) {
    return PbEnumTraits<horus::sdk::pb::RpcMessage_Version>::ValueOf(reader.Reader().get_enum());
  }
};

}  // namespace horus

namespace horus {
namespace sdk {
namespace pb {

/// Appends `value` to `sink`.
template <class Sink>
void HorusStringify(Sink& sink, RpcMessage_Version value) noexcept(noexcept(sink.Append(StringView{}))) {
  sink.Append(PbEnumTraits<RpcMessage_Version>::NameOf(value));
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus

// NOLINTEND(readability-identifier-length)

#endif  // HORUS_PB_RPC_MESSAGE_PB_H_
