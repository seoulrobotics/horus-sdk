#include "horus/pb/detection_service/detection_service_handler.h"

#include <utility>

#include "horus/future/resolved.h"

namespace horus {
namespace sdk {
namespace pb {

AnyFuture<pb::RpcMessage> DetectionServiceHandler::Handle(const RpcContext& context,
                                                          pb::RpcMessage&& request) noexcept(false) {
  const pb::RpcMessage owned_request{std::move(request)};
  switch (owned_request.method_id()) {
    case 2: {
      return DoDispatch(&DetectionServiceHandler::Subscribe, context, owned_request);
    }
    case 3: {
      return DoDispatch(&DetectionServiceHandler::Unsubscribe, context, owned_request);
    }
    default: {
      throw horus_internal::UnknownRpcMethodError{};
    }
  }
}

AnyFuture<pb::DefaultSubscribeResponse> DetectionServiceHandler::Subscribe(const RpcContext& context,
                                                                           pb::DefaultSubscribeRequest&& request) noexcept(false) {
  static_cast<void>(pb::DefaultSubscribeRequest{std::move(request)});
  return ResolveWith(DetectionSubscriberServiceSubscribers().Add<pb::DefaultSubscribeResponse>(context));
}

AnyFuture<pb::DefaultUnsubscribeResponse> DetectionServiceHandler::Unsubscribe(const RpcContext& context,
                                                                               pb::DefaultUnsubscribeRequest&& request) noexcept(false) {
  static_cast<void>(pb::DefaultUnsubscribeRequest{std::move(request)});
  return ResolveWith(DetectionSubscriberServiceSubscribers().Remove<pb::DefaultUnsubscribeResponse>(context));
}

AnyFuture<pb::RpcMessage> DetectionSubscriberServiceHandler::Handle(const RpcContext& context,
                                                                    pb::RpcMessage&& request) noexcept(false) {
  const pb::RpcMessage owned_request{std::move(request)};
  switch (owned_request.method_id()) {
    case 1: {
      return DoDispatch(&DetectionSubscriberServiceHandler::BroadcastDetection, context, owned_request);
    }
    default: {
      throw horus_internal::UnknownRpcMethodError{};
    }
  }
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus
