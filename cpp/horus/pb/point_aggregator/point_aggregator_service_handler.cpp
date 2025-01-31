#include "horus/pb/point_aggregator/point_aggregator_service_handler.h"

#include <utility>

#include "horus/future/resolved.h"

namespace horus {
namespace sdk {
namespace pb {

AnyFuture<pb::RpcMessage> PointAggregatorServiceHandler::Handle(const RpcContext& context,
                                                                pb::RpcMessage&& request) noexcept(false) {
  const pb::RpcMessage owned_request{std::move(request)};
  switch (owned_request.method_id()) {
    case 5: {
      return DoDispatch(&PointAggregatorServiceHandler::Subscribe, context, owned_request);
    }
    case 6: {
      return DoDispatch(&PointAggregatorServiceHandler::Unsubscribe, context, owned_request);
    }
    default: {
      throw horus_internal::UnknownRpcMethodError{};
    }
  }
}

AnyFuture<pb::DefaultSubscribeResponse> PointAggregatorServiceHandler::Subscribe(const RpcContext& context,
                                                                                 pb::DefaultSubscribeRequest&& request) noexcept(false) {
  static_cast<void>(pb::DefaultSubscribeRequest{std::move(request)});
  return ResolveWith(PointAggregatorSubscriberServiceSubscribers().Add<pb::DefaultSubscribeResponse>(context));
}

AnyFuture<pb::DefaultUnsubscribeResponse> PointAggregatorServiceHandler::Unsubscribe(const RpcContext& context,
                                                                                     pb::DefaultUnsubscribeRequest&& request) noexcept(false) {
  static_cast<void>(pb::DefaultUnsubscribeRequest{std::move(request)});
  return ResolveWith(PointAggregatorSubscriberServiceSubscribers().Remove<pb::DefaultUnsubscribeResponse>(context));
}

AnyFuture<pb::RpcMessage> PointAggregatorSubscriberServiceHandler::Handle(const RpcContext& context,
                                                                          pb::RpcMessage&& request) noexcept(false) {
  const pb::RpcMessage owned_request{std::move(request)};
  switch (owned_request.method_id()) {
    case 2: {
      return DoDispatch(&PointAggregatorSubscriberServiceHandler::BroadcastProcessedPoints, context, owned_request);
    }
    case 3: {
      return DoDispatch(&PointAggregatorSubscriberServiceHandler::BroadcastOccupancyGrid, context, owned_request);
    }
    default: {
      throw horus_internal::UnknownRpcMethodError{};
    }
  }
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus
