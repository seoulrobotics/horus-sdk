/// @file
///
/// The `RpcEndpoint` class and related classes.

#ifndef HORUS_RPC_ENDPOINT_H_
#define HORUS_RPC_ENDPOINT_H_

#include <exception>
#include <functional>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "horus/functional/move_only_function.h"
#include "horus/future/any.h"
#include "horus/internal/attributes.h"
#include "horus/internal/pointer_cast.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/rpc/retry_policy.h"
#include "horus/types/one_of.h"
#include "horus/types/string_view.h"

namespace horus {

/// Exception thrown when received Protobuf data is invalid.
class InvalidDataError : public std::runtime_error {
 public:
  /// Constructs an `InvalidDataError` which displays the given bytes as being invalid.
  explicit InvalidDataError(StringView invalid_message_bytes);
};

/// Exception thrown when `RpcEndpoint::Send()` or `RpcEndpoint::SendWithResponse()` is called on
/// a disconnected endpoint.
///
/// Disconnections are exposed with this exception rather than with a boolean property of
/// `RpcEndpoint` because such a property would be useless. By the time a hypothetical
/// `IsConnected()` property returns, it is possible that the disconnection will have taken place.
/// In addition, a "connected" state is typically maintained by periodically pinging the other side
/// of the connection, which is useless when we are about to send a message to the other side
/// anyway, which will check the connection.
class RpcEndpointDisconnectedError : public std::runtime_error {
 public:
  RpcEndpointDisconnectedError() : std::runtime_error("rpc endpoint disconnected") {}
};

/// Type of `RpcMessage::RequestId()`.
using RpcRequestId = decltype(std::declval<sdk::pb::RpcMessage>().request_id());

/// `RpcRequestId` used for requests which expect no response. Must be outside of
/// `[kTwoWayRpcRequestIdMin, kTwoWayRpcRequestIdMax]`.
static constexpr RpcRequestId kOneWayRpcRequestId{0UL};

/// The minimum value of a `RpcRequestId` when a response is expected.
static constexpr RpcRequestId kTwoWayRpcRequestIdMin{1UL};

/// The maximum value of a `RpcRequestId` when a response is expected.
static constexpr RpcRequestId kTwoWayRpcRequestIdMax{std::numeric_limits<RpcRequestId>::max()};

/// Type of `RpcMessage::MethodId()`.
using RpcMethodId = decltype(std::declval<sdk::pb::RpcMessage>().method_id());

/// `RpcMethodId` for responses to previously sent requests.
static constexpr RpcMethodId kRpcResponseMethodId{0UL};

/// Additional arguments used when sending RPCs.
struct RpcOptions {
  /// Constructs `RpcOptions`.
  RpcOptions(const RpcRetryPolicy& retry) noexcept : retry_policy{retry} {}  // NOLINT(*-explicit-*)

  /// The policy to follow while sending the request.
  RpcRetryPolicy retry_policy;
};

/// A generic "RPC endpoint" interface.
class RpcEndpoint {
 public:
  /// `LifecycleEvent` emitted when a connection has been established.
  struct ConnectedEvent {};
  /// `LifecycleEvent` emitted when a connection has shut down.
  struct DisconnectedEvent {
    /// The reason why the connection was severed; this may be empty, in which case the connection
    /// ended normally.
    std::string reason;
  };
  /// `LifecycleEvent` emitted when an error occurs for any reason.
  struct ErrorEvent {
    /// The (non-null) pointer to the error.
    std::exception_ptr error;
  };
  /// `LifecycleEvent` emitted when the endpoint has shut down.
  struct ShutdownEvent {};

  /// An event in the lifecycle of the endpoint.
  using LifecycleEvent = OneOf<ConnectedEvent, DisconnectedEvent, ErrorEvent, ShutdownEvent>;

  /// `RpcEndpoint`s cannot be copied.
  RpcEndpoint(const RpcEndpoint&) = delete;
  /// `RpcEndpoint`s cannot be copied.
  RpcEndpoint& operator=(const RpcEndpoint&) = delete;
  /// `RpcEndpoint`s may be moved.
  RpcEndpoint(RpcEndpoint&&) noexcept = default;
  /// `RpcEndpoint`s may be moved.
  RpcEndpoint& operator=(RpcEndpoint&&) noexcept = default;

  /// Destroys the `RpcEndpoint`.
  virtual ~RpcEndpoint() = default;

  /// Returns a URI (like "tcp://1.2.3.4" or "ws://0.0.0.0:80") representing the endpoint.
  ///
  /// This URI is intended for debugging/information purposes only. It should not be used to drive
  /// any logic as its format may change at any time.
  virtual StringView Uri() const noexcept = 0;

  /// Sends a raw message to the RPC endpoint, returning a future which will resolve when the
  /// message has been sent.
  ///
  /// @throws std::exception Any exception encountered while sending the `message`.
  /// @throws std::bad_alloc If the resulting future cannot be allocated.
  /// @throws RpcEndpointDisconnectedError Exception thrown when the endpoint is no longer connected
  /// to its target.
  virtual AnyFuture<void> Send(sdk::pb::RpcMessage&& message,
                               const RpcOptions& options) noexcept(false) = 0;

  /// Sends a raw message to the RPC endpoint, returning a future which will resolve to the response
  /// of the message sent to the endpoint as part of the specified service.
  ///
  /// This function will automatically set `message.RequestId()`.
  ///
  /// @throws std::exception Any exception encountered while sending the `message` or receiving its
  /// response.
  /// @throws std::bad_alloc If the resulting future cannot be allocated.
  /// @throws RpcEndpointDisconnectedError Exception thrown when the endpoint is no longer connected
  /// to its target.
  virtual AnyFuture<sdk::pb::RpcMessage> SendWithResponse(
      sdk::pb::RpcMessage&& message, const RpcOptions& options) noexcept(false) = 0;

  /// Sets the function to call when the lifecycle of the endpoint changes.
  ///
  /// The reference to the `receiver` must outlive the `RpcEndpoint`.
  template <class Receiver, void (Receiver::*OnEvent)(LifecycleEvent&& event) noexcept>
  void SetLifecycleEventCallback(Receiver& receiver HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept {
    SetLifecycleEventCallback(&receiver, [](void* receiver_ptr, LifecycleEvent&& event) {
      (horus_internal::UnsafePointerCast<Receiver>(receiver_ptr)->*OnEvent)(std::move(event));
    });
  }

  /// Sets the function to call when the lifecycle of the endpoint changes.
  ///
  /// The reference to `on_event` must outlive the `RpcEndpoint`.
  template <class OnEvent>
  void SetLifecycleEventCallback(OnEvent& on_event HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND) noexcept {
    SetLifecycleEventCallback<OnEvent, &OnEvent::operator()>(on_event);
  }

  /// Clears the lifecycle event set with `SetLifecycleEventCallback()`.
  void ClearLifecycleEventCallback() noexcept { SetLifecycleEventCallback(nullptr, nullptr); }

 protected:
  /// Sets the function to call when the lifecycle of the endpoint changes. `on_event()` must be
  /// noexcept.
  virtual void SetLifecycleEventCallback(void* receiver,
                                         void (*on_event)(void* receiver,
                                                          LifecycleEvent&& event)) noexcept = 0;

  /// Default-constructs an `RpcEndpoint`.
  RpcEndpoint() noexcept = default;
};

/// Context of a running RPC handler.
class RpcContext final {
 public:
  /// Constructs an `RpcContext`.
  explicit RpcContext(std::shared_ptr<RpcEndpoint>&& endpoint) noexcept
      : endpoint_{std::move(endpoint)} {}

  /// Returns a shared reference to the `RpcEndpoint` which initiated the request.
  constexpr const std::shared_ptr<RpcEndpoint>& Endpoint() const noexcept { return endpoint_; }

 private:
  /// @see Endpoint()
  std::shared_ptr<RpcEndpoint> endpoint_;
};

/// A function which can accept `RpcMessage`s.
using MessageHandler =
    MoveOnlyFunction<AnyFuture<sdk::pb::RpcMessage>(const RpcContext&, sdk::pb::RpcMessage&&)>;

/// Returns a `MessageHandler` which cannot process any received message.
MessageHandler NoMessageHandler() noexcept;

}  // namespace horus

#endif  // HORUS_RPC_ENDPOINT_H_
