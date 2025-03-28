#ifndef HORUS_PB_POINT_AGGREGATOR_POINT_AGGREGATOR_SERVICE_CLIENT_H_
#define HORUS_PB_POINT_AGGREGATOR_POINT_AGGREGATOR_SERVICE_CLIENT_H_

#include <cstdint>

#include "horus/future/any.h"
#include "horus/pb/point/point_message_pb.h"
#include "horus/pb/point_aggregator/point_aggregator_service_pb.h"
#include "horus/pb/preprocessing/messages_pb.h"
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

/// Service for receiving pre-processed point clouds.
class PointAggregatorServiceClient : public horus_internal::RpcBaseClient {
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
  std::uint16_t ServiceId() const noexcept final { return 6; }
  /// @copydoc horus_internal::RpcBaseClient::ServiceFullName()
  StringView ServiceFullName() const noexcept final { return "horus.pb.PointAggregatorService"; }

  /// Subscribes to receive future processed point clouds as they are made available.
  AnyFuture<pb::DefaultSubscribeResponse> Subscribe(const pb::DefaultSubscribeRequest& request, const RpcOptions& options) noexcept(false) {
    return InvokeRpc<pb::DefaultSubscribeResponse>(5, request, options);
  }

  /// Unsubscribe following a call to `Subscribe()`.
  AnyFuture<pb::DefaultUnsubscribeResponse> Unsubscribe(const pb::DefaultUnsubscribeRequest& request, const RpcOptions& options) noexcept(false) {
    return InvokeRpc<pb::DefaultUnsubscribeResponse>(6, request, options);
  }
};

/// Service exposed by clients of the `PointAggregatorService` that wish to receive updates when new processed points are received.
class PointAggregatorSubscriberServiceClient : public horus_internal::RpcBaseClient {
 public:
  /// The request type of `BroadcastProcessedPoints()`.
  using BroadcastProcessedPointsRequestType = pb::AggregatedPointEvents;
  /// The response type of `BroadcastProcessedPoints()`.
  using BroadcastProcessedPointsResponseType = void;
  /// The request type of `BroadcastOccupancyGrid()`.
  using BroadcastOccupancyGridRequestType = pb::OccupancyGridEvent;
  /// The response type of `BroadcastOccupancyGrid()`.
  using BroadcastOccupancyGridResponseType = void;

  /// @copydoc horus_internal::RpcBaseClient::RpcBaseClient()
  using horus_internal::RpcBaseClient::RpcBaseClient;

  /// @copydoc horus_internal::RpcBaseClient::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 5; }
  /// @copydoc horus_internal::RpcBaseClient::ServiceFullName()
  StringView ServiceFullName() const noexcept final { return "horus.pb.PointAggregatorSubscriberService"; }

  /// Notify new processed points input.
  AnyFuture<void> BroadcastProcessedPoints(const pb::AggregatedPointEvents& request, const RpcOptions& options) noexcept(false) {
    return InvokeOneWayRpc(2, request, options);
  }

  /// Notify new occupancy grid input.
  AnyFuture<void> BroadcastOccupancyGrid(const pb::OccupancyGridEvent& request, const RpcOptions& options) noexcept(false) {
    return InvokeOneWayRpc(3, request, options);
  }
};

}  // namespace pb
}  // namespace sdk
}  // namespace horus

#endif  // HORUS_PB_POINT_AGGREGATOR_POINT_AGGREGATOR_SERVICE_CLIENT_H_
