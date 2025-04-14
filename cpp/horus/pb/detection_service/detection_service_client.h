#ifndef HORUS_PB_DETECTION_SERVICE_DETECTION_SERVICE_CLIENT_H_
#define HORUS_PB_DETECTION_SERVICE_DETECTION_SERVICE_CLIENT_H_

#include <cstdint>

#include "horus/future/any.h"
#include "horus/pb/detection_service/detection_pb.h"
#include "horus/pb/detection_service/detection_service_pb.h"
#include "horus/pb/rpc_pb.h"
#include "horus/rpc/base_client.h"
#include "horus/rpc/endpoint.h"
#include "horus/strings/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

namespace horus {
namespace sdk {
namespace pb {

/// Service for receiving pre-processed foreground points and sending detection results.
class DetectionServiceClient : public horus_internal::RpcBaseClient {
 public:
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
  std::uint16_t ServiceId() const noexcept final { return 7; }
  /// @copydoc horus_internal::RpcBaseClient::ServiceFullName()
  StringView ServiceFullName() const noexcept final { return "horus.pb.DetectionService"; }

  /// Subscribes to receive future detection results as they are made available.
  AnyFuture<pb::DefaultSubscribeResponse> Subscribe(const pb::DefaultSubscribeRequest& request, const RpcOptions& options) noexcept(false) {
    return InvokeRpc<pb::DefaultSubscribeResponse>(2, request, options);
  }

  /// Unsubscribe following a call to `Subscribe()`.
  AnyFuture<pb::DefaultUnsubscribeResponse> Unsubscribe(const pb::DefaultUnsubscribeRequest& request, const RpcOptions& options) noexcept(false) {
    return InvokeRpc<pb::DefaultUnsubscribeResponse>(3, request, options);
  }
};

/// Service exposed by clients of the `DetectionService` that wish to receive updates when new detection results are processed.
class DetectionSubscriberServiceClient : public horus_internal::RpcBaseClient {
 public:
  /// The request type of `BroadcastDetection()`.
  using BroadcastDetectionRequestType = pb::DetectionEvent;
  /// The response type of `BroadcastDetection()`.
  using BroadcastDetectionResponseType = void;

  /// @copydoc horus_internal::RpcBaseClient::RpcBaseClient()
  using horus_internal::RpcBaseClient::RpcBaseClient;

  /// @copydoc horus_internal::RpcBaseClient::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 12; }
  /// @copydoc horus_internal::RpcBaseClient::ServiceFullName()
  StringView ServiceFullName() const noexcept final { return "horus.pb.DetectionSubscriberService"; }

  /// Notify new detection results.
  AnyFuture<void> BroadcastDetection(const pb::DetectionEvent& request, const RpcOptions& options) noexcept(false) {
    return InvokeOneWayRpc(1, request, options);
  }
};

}  // namespace pb
}  // namespace sdk
}  // namespace horus

#endif  // HORUS_PB_DETECTION_SERVICE_DETECTION_SERVICE_CLIENT_H_
