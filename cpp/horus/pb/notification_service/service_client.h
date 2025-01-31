#ifndef HORUS_PB_NOTIFICATION_SERVICE_SERVICE_CLIENT_H_
#define HORUS_PB_NOTIFICATION_SERVICE_SERVICE_CLIENT_H_

#include <cstdint>

#include "horus/future/any.h"
#include "horus/pb/notification_service/service_pb.h"
#include "horus/pb/rpc_pb.h"
#include "horus/rpc/base_client.h"
#include "horus/rpc/endpoint.h"
#include "horus/types/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

namespace horus {
namespace sdk {
namespace pb {

/// Service which receives notifications from services, storing and forwarding them.
class NotificationServiceClient : public horus_internal::RpcBaseClient {
 public:
  /// The request type of `LogMessage()`.
  using LogMessageRequestType = pb::LogMessageRequest;
  /// The response type of `LogMessage()`.
  using LogMessageResponseType = void;
  /// The request type of `Subscribe()`.
  using SubscribeRequestType = pb::DefaultSubscribeRequest;
  /// The response type of `Subscribe()`.
  using SubscribeResponseType = pb::DefaultSubscribeResponse;
  /// The request type of `Unsubscribe()`.
  using UnsubscribeRequestType = pb::DefaultUnsubscribeRequest;
  /// The response type of `Unsubscribe()`.
  using UnsubscribeResponseType = pb::DefaultUnsubscribeResponse;

  /// @copydoc horus_internal::RpcBaseClient::RpcBaseClient()
  using horus_internal::RpcBaseClient::RpcBaseClient;

  /// @copydoc horus_internal::RpcBaseClient::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 2; }
  /// @copydoc horus_internal::RpcBaseClient::ServiceFullName()
  StringView ServiceFullName() const noexcept final { return "horus.pb.NotificationService"; }

  /// Logs a message, storing it and broadcasting it to subscribers.
  AnyFuture<void> LogMessage(const pb::LogMessageRequest& request, const RpcOptions& options) noexcept(false) {
    return InvokeOneWayRpc(1, request, options);
  }

  /// Subscribes to new log and profiling events.
  AnyFuture<pb::DefaultSubscribeResponse> Subscribe(const pb::DefaultSubscribeRequest& request, const RpcOptions& options) noexcept(false) {
    return InvokeRpc<pb::DefaultSubscribeResponse>(3, request, options);
  }

  /// Unsubscribe following a call to `Subscribe()`.
  AnyFuture<pb::DefaultUnsubscribeResponse> Unsubscribe(const pb::DefaultUnsubscribeRequest& request, const RpcOptions& options) noexcept(false) {
    return InvokeRpc<pb::DefaultUnsubscribeResponse>(4, request, options);
  }
};

/// Service which listens for notifications forwarded by the `NotificationService`.
class NotificationListenerServiceClient : public horus_internal::RpcBaseClient {
 public:
  /// The request type of `NotifyLogMessage()`.
  using NotifyLogMessageRequestType = pb::LogMessageEvent;
  /// The response type of `NotifyLogMessage()`.
  using NotifyLogMessageResponseType = void;
  /// The request type of `NotifyProfilingInfo()`.
  using NotifyProfilingInfoRequestType = pb::ProfilingInfoEvent;
  /// The response type of `NotifyProfilingInfo()`.
  using NotifyProfilingInfoResponseType = void;
  /// The request type of `NotifySensorInfo()`.
  using NotifySensorInfoRequestType = pb::SensorInfoEvent;
  /// The response type of `NotifySensorInfo()`.
  using NotifySensorInfoResponseType = void;

  /// @copydoc horus_internal::RpcBaseClient::RpcBaseClient()
  using horus_internal::RpcBaseClient::RpcBaseClient;

  /// @copydoc horus_internal::RpcBaseClient::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 10; }
  /// @copydoc horus_internal::RpcBaseClient::ServiceFullName()
  StringView ServiceFullName() const noexcept final { return "horus.pb.NotificationListenerService"; }

  /// Received when a log message is received by the `NotificationService`.
  AnyFuture<void> NotifyLogMessage(const pb::LogMessageEvent& request, const RpcOptions& options) noexcept(false) {
    return InvokeOneWayRpc(1, request, options);
  }

  /// Received when a profiling info is received by the `NotificationService`.
  AnyFuture<void> NotifyProfilingInfo(const pb::ProfilingInfoEvent& request, const RpcOptions& options) noexcept(false) {
    return InvokeOneWayRpc(2, request, options);
  }

  /// Received when a sensor info is received by the `NotificationService`.
  AnyFuture<void> NotifySensorInfo(const pb::SensorInfoEvent& request, const RpcOptions& options) noexcept(false) {
    return InvokeOneWayRpc(3, request, options);
  }
};

}  // namespace pb
}  // namespace sdk
}  // namespace horus

#endif  // HORUS_PB_NOTIFICATION_SERVICE_SERVICE_CLIENT_H_
