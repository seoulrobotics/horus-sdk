#ifndef HORUS_PB_POINT_AGGREGATOR_POINT_AGGREGATOR_SERVICE_HANDLER_H_
#define HORUS_PB_POINT_AGGREGATOR_POINT_AGGREGATOR_SERVICE_HANDLER_H_

#include <cstdint>

#include "horus/future/any.h"
#include "horus/internal/attributes.h"
#include "horus/pb/point/point_message_pb.h"
#include "horus/pb/point_aggregator/point_aggregator_service_client.h"
#include "horus/pb/point_aggregator/point_aggregator_service_pb.h"
#include "horus/pb/preprocessing/messages_pb.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/pb/rpc_pb.h"
#include "horus/rpc/base_handler.h"
#include "horus/rpc/endpoint.h"
#include "horus/rpc/internal/subscriber_set.h"
#include "horus/strings/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

namespace horus {
namespace sdk {
namespace pb {

/// Service for receiving pre-processed point clouds.
class PointAggregatorServiceHandler : public horus_internal::RpcBaseHandler {
 public:
  /// The request type of `Subscribe()`.
  using SubscribeRequestType = pb::DefaultSubscribeRequest;
  /// The response type of `Subscribe()`.
  using SubscribeResponseType = pb::DefaultSubscribeResponse;
  /// The request type of `Unsubscribe()`.
  using UnsubscribeRequestType = pb::DefaultUnsubscribeRequest;
  /// The response type of `Unsubscribe()`.
  using UnsubscribeResponseType = pb::DefaultUnsubscribeResponse;

  /// Default constructor.
  PointAggregatorServiceHandler() noexcept = default;

  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 6; }
  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  StringView ServiceFullName() const noexcept final { return "horus.pb.PointAggregatorService"; }

  /// @copydoc horus_internal::RpcBaseHandler::Handle()
  AnyFuture<pb::RpcMessage> Handle(const RpcContext& context,
                                   pb::RpcMessage&& request) noexcept(false) final;

 protected:

  /// Subscribes to receive future processed point clouds as they are made available.
  virtual AnyFuture<pb::DefaultSubscribeResponse> Subscribe(const RpcContext& context,
                                                            pb::DefaultSubscribeRequest&& request) noexcept(false);

  /// Unsubscribe following a call to `Subscribe()`.
  virtual AnyFuture<pb::DefaultUnsubscribeResponse> Unsubscribe(const RpcContext& context,
                                                                pb::DefaultUnsubscribeRequest&& request) noexcept(false);

  /// Returns a reference to the `SubscriberSet` of `PointAggregatorSubscriberService`s.
  constexpr horus_internal::SubscriberSet<PointAggregatorSubscriberServiceClient>& PointAggregatorSubscriberServiceSubscribers() noexcept {
    return point_aggregator_subscriber_service_subscribers_;
  }
  /// Returns a reference to the `SubscriberSet` of `PointAggregatorSubscriberService`s.
  constexpr const horus_internal::SubscriberSet<PointAggregatorSubscriberServiceClient>& PointAggregatorSubscriberServiceSubscribers() const noexcept {
    return point_aggregator_subscriber_service_subscribers_;
  }

 private:
  /// `PointAggregatorSubscriberService` subscribers.
  horus_internal::SubscriberSet<PointAggregatorSubscriberServiceClient> point_aggregator_subscriber_service_subscribers_;
};

/// A `PointAggregatorServiceHandler` which forwards received messages to function objects.
template <class SharedState, class OnSubscribe, class OnUnsubscribe>
class FunctionalPointAggregatorService final : public PointAggregatorServiceHandler {
 public:
  /// Constructs a `FunctionalPointAggregatorService` which forwards received messages to function objects.
  FunctionalPointAggregatorService(SharedState shared_state, OnSubscribe&& on_subscribe, OnUnsubscribe&& on_unsubscribe) noexcept
      : shared_state_{std::forward<SharedState>(shared_state)}
      , on_subscribe_{std::move(on_subscribe)}
      , on_unsubscribe_{std::move(on_unsubscribe)} {}

  /// Returns a `FunctionalPointAggregatorService` which handles `Subscribe()` with `on_subscribe()`.
  template <class F>
  FunctionalPointAggregatorService<SharedState, F, OnUnsubscribe>
  SubscribeWith(F&& on_subscribe) && noexcept {
    return FunctionalPointAggregatorService<SharedState, F, OnUnsubscribe>{
        std::move(shared_state_), std::forward<F>(on_subscribe), std::move(on_unsubscribe_)};
  }

  /// Returns a `FunctionalPointAggregatorService` which handles `Unsubscribe()` with `on_unsubscribe()`.
  template <class F>
  FunctionalPointAggregatorService<SharedState, OnSubscribe, F>
  UnsubscribeWith(F&& on_unsubscribe) && noexcept {
    return FunctionalPointAggregatorService<SharedState, OnSubscribe, F>{
        std::move(shared_state_), std::move(on_subscribe_), std::forward<F>(on_unsubscribe)};
  }

 protected:
  /// Forwards `Subscribe()` to `OnSubscribe`.
  AnyFuture<pb::DefaultSubscribeResponse> Subscribe(const RpcContext& context,
                                                    pb::DefaultSubscribeRequest&& request) noexcept(false) final {
    return horus_internal::ForwardToFunctionalHandler<pb::DefaultSubscribeResponse>(shared_state_, on_subscribe_, context, std::move(request));
  }
  /// Forwards `Unsubscribe()` to `OnUnsubscribe`.
  AnyFuture<pb::DefaultUnsubscribeResponse> Unsubscribe(const RpcContext& context,
                                                        pb::DefaultUnsubscribeRequest&& request) noexcept(false) final {
    return horus_internal::ForwardToFunctionalHandler<pb::DefaultUnsubscribeResponse>(shared_state_, on_unsubscribe_, context, std::move(request));
  }
 private:
  /// State shared between all callbacks.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS SharedState shared_state_;
  /// Handler of `Subscribe`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnSubscribe on_subscribe_;
  /// Handler of `Unsubscribe`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnUnsubscribe on_unsubscribe_;
};

/// Returns a `FunctionalPointAggregatorService` with the given shared state.
template <class SharedState>
FunctionalPointAggregatorService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>
CreateFunctionalPointAggregatorService(SharedState&& shared_state) noexcept {
  return FunctionalPointAggregatorService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>{
      std::forward<SharedState>(shared_state), &horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>, &horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>};
}

/// Returns a `FunctionalPointAggregatorService` with no shared state.
inline FunctionalPointAggregatorService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>
CreateFunctionalPointAggregatorService() noexcept {
  return FunctionalPointAggregatorService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>{
      nullptr, &horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>, &horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>};
}

/// Service exposed by clients of the `PointAggregatorService` that wish to receive updates when new processed points are received.
class PointAggregatorSubscriberServiceHandler : public horus_internal::RpcBaseHandler {
 public:
  /// The request type of `BroadcastProcessedPoints()`.
  using BroadcastProcessedPointsRequestType = pb::AggregatedPointEvents;
  /// The response type of `BroadcastProcessedPoints()`.
  using BroadcastProcessedPointsResponseType = void;
  /// The request type of `BroadcastOccupancyGrid()`.
  using BroadcastOccupancyGridRequestType = pb::OccupancyGridEvent;
  /// The response type of `BroadcastOccupancyGrid()`.
  using BroadcastOccupancyGridResponseType = void;

  /// Default constructor.
  PointAggregatorSubscriberServiceHandler() noexcept = default;

  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 5; }
  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  StringView ServiceFullName() const noexcept final { return "horus.pb.PointAggregatorSubscriberService"; }

  /// @copydoc horus_internal::RpcBaseHandler::Handle()
  AnyFuture<pb::RpcMessage> Handle(const RpcContext& context,
                                   pb::RpcMessage&& request) noexcept(false) final;

 protected:

  /// Notify new processed points input.
  virtual AnyFuture<void> BroadcastProcessedPoints(const RpcContext& context,
                                                   pb::AggregatedPointEvents&& request) noexcept(false) = 0;

  /// Notify new occupancy grid input.
  virtual AnyFuture<void> BroadcastOccupancyGrid(const RpcContext& context,
                                                 pb::OccupancyGridEvent&& request) noexcept(false) = 0;
};

/// A `PointAggregatorSubscriberServiceHandler` which forwards received messages to function objects.
template <class SharedState, class OnBroadcastProcessedPoints, class OnBroadcastOccupancyGrid>
class FunctionalPointAggregatorSubscriberService final : public PointAggregatorSubscriberServiceHandler {
 public:
  /// Constructs a `FunctionalPointAggregatorSubscriberService` which forwards received messages to function objects.
  FunctionalPointAggregatorSubscriberService(SharedState shared_state, OnBroadcastProcessedPoints&& on_broadcast_processed_points, OnBroadcastOccupancyGrid&& on_broadcast_occupancy_grid) noexcept
      : shared_state_{std::forward<SharedState>(shared_state)}
      , on_broadcast_processed_points_{std::move(on_broadcast_processed_points)}
      , on_broadcast_occupancy_grid_{std::move(on_broadcast_occupancy_grid)} {}

  /// Returns a `FunctionalPointAggregatorSubscriberService` which handles `BroadcastProcessedPoints()` with `on_broadcast_processed_points()`.
  template <class F>
  FunctionalPointAggregatorSubscriberService<SharedState, F, OnBroadcastOccupancyGrid>
  BroadcastProcessedPointsWith(F&& on_broadcast_processed_points) && noexcept {
    return FunctionalPointAggregatorSubscriberService<SharedState, F, OnBroadcastOccupancyGrid>{
        std::move(shared_state_), std::forward<F>(on_broadcast_processed_points), std::move(on_broadcast_occupancy_grid_)};
  }

  /// Returns a `FunctionalPointAggregatorSubscriberService` which handles `BroadcastOccupancyGrid()` with `on_broadcast_occupancy_grid()`.
  template <class F>
  FunctionalPointAggregatorSubscriberService<SharedState, OnBroadcastProcessedPoints, F>
  BroadcastOccupancyGridWith(F&& on_broadcast_occupancy_grid) && noexcept {
    return FunctionalPointAggregatorSubscriberService<SharedState, OnBroadcastProcessedPoints, F>{
        std::move(shared_state_), std::move(on_broadcast_processed_points_), std::forward<F>(on_broadcast_occupancy_grid)};
  }

 protected:
  /// Forwards `BroadcastProcessedPoints()` to `OnBroadcastProcessedPoints`.
  AnyFuture<void> BroadcastProcessedPoints(const RpcContext& context,
                                           pb::AggregatedPointEvents&& request) noexcept(false) final {
    return horus_internal::ForwardToFunctionalHandler<void>(shared_state_, on_broadcast_processed_points_, context, std::move(request));
  }
  /// Forwards `BroadcastOccupancyGrid()` to `OnBroadcastOccupancyGrid`.
  AnyFuture<void> BroadcastOccupancyGrid(const RpcContext& context,
                                         pb::OccupancyGridEvent&& request) noexcept(false) final {
    return horus_internal::ForwardToFunctionalHandler<void>(shared_state_, on_broadcast_occupancy_grid_, context, std::move(request));
  }
 private:
  /// State shared between all callbacks.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS SharedState shared_state_;
  /// Handler of `BroadcastProcessedPoints`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnBroadcastProcessedPoints on_broadcast_processed_points_;
  /// Handler of `BroadcastOccupancyGrid`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnBroadcastOccupancyGrid on_broadcast_occupancy_grid_;
};

/// Returns a `FunctionalPointAggregatorSubscriberService` with the given shared state.
template <class SharedState>
FunctionalPointAggregatorSubscriberService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::AggregatedPointEvents, void>), decltype(&horus_internal::IgnoreRpc<pb::OccupancyGridEvent, void>)>
CreateFunctionalPointAggregatorSubscriberService(SharedState&& shared_state) noexcept {
  return FunctionalPointAggregatorSubscriberService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::AggregatedPointEvents, void>), decltype(&horus_internal::IgnoreRpc<pb::OccupancyGridEvent, void>)>{
      std::forward<SharedState>(shared_state), &horus_internal::IgnoreRpc<pb::AggregatedPointEvents, void>, &horus_internal::IgnoreRpc<pb::OccupancyGridEvent, void>};
}

/// Returns a `FunctionalPointAggregatorSubscriberService` with no shared state.
inline FunctionalPointAggregatorSubscriberService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::AggregatedPointEvents, void>), decltype(&horus_internal::IgnoreRpc<pb::OccupancyGridEvent, void>)>
CreateFunctionalPointAggregatorSubscriberService() noexcept {
  return FunctionalPointAggregatorSubscriberService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::AggregatedPointEvents, void>), decltype(&horus_internal::IgnoreRpc<pb::OccupancyGridEvent, void>)>{
      nullptr, &horus_internal::IgnoreRpc<pb::AggregatedPointEvents, void>, &horus_internal::IgnoreRpc<pb::OccupancyGridEvent, void>};
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus

#endif  // HORUS_PB_POINT_AGGREGATOR_POINT_AGGREGATOR_SERVICE_HANDLER_H_
