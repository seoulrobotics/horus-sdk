#include "horus/pb/detection_merger/service_handler.h"

#include <utility>

#include "horus/future/resolved.h"

namespace horus {
namespace sdk {
namespace pb {

AnyFuture<pb::RpcMessage> DetectionMergerServiceHandler::Handle(const RpcContext& context,
                                                                pb::RpcMessage&& request) noexcept(false) {
  const pb::RpcMessage owned_request{std::move(request)};
  switch (owned_request.method_id()) {
    case 1: {
      return DoDispatch(&DetectionMergerServiceHandler::Subscribe, context, owned_request);
    }
    case 2: {
      return DoDispatch(&DetectionMergerServiceHandler::Unsubscribe, context, owned_request);
    }
    default: {
      throw horus_internal::UnknownRpcMethodError{};
    }
  }
}

AnyFuture<pb::DefaultSubscribeResponse> DetectionMergerServiceHandler::Subscribe(const RpcContext& context,
                                                                                 pb::DefaultSubscribeRequest&& request) noexcept(false) {
  static_cast<void>(pb::DefaultSubscribeRequest{std::move(request)});
  return ResolveWith(DetectionMergerSubscriberServiceSubscribers().Add<pb::DefaultSubscribeResponse>(context));
}

AnyFuture<pb::DefaultUnsubscribeResponse> DetectionMergerServiceHandler::Unsubscribe(const RpcContext& context,
                                                                                     pb::DefaultUnsubscribeRequest&& request) noexcept(false) {
  static_cast<void>(pb::DefaultUnsubscribeRequest{std::move(request)});
  return ResolveWith(DetectionMergerSubscriberServiceSubscribers().Remove<pb::DefaultUnsubscribeResponse>(context));
}

AnyFuture<pb::RpcMessage> DetectionMergerSubscriberServiceHandler::Handle(const RpcContext& context,
                                                                          pb::RpcMessage&& request) noexcept(false) {
  const pb::RpcMessage owned_request{std::move(request)};
  switch (owned_request.method_id()) {
    case 1: {
      return DoDispatch(&DetectionMergerSubscriberServiceHandler::BroadcastDetection, context, owned_request);
    }
    default: {
      throw horus_internal::UnknownRpcMethodError{};
    }
  }
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus
