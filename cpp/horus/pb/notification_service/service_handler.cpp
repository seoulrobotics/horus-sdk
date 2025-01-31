#include "horus/pb/notification_service/service_handler.h"

#include <utility>

#include "horus/future/resolved.h"

namespace horus {
namespace sdk {
namespace pb {

AnyFuture<pb::RpcMessage> NotificationServiceHandler::Handle(const RpcContext& context,
                                                             pb::RpcMessage&& request) noexcept(false) {
  const pb::RpcMessage owned_request{std::move(request)};
  switch (owned_request.method_id()) {
    case 1: {
      return DoDispatch(&NotificationServiceHandler::LogMessage, context, owned_request);
    }
    case 3: {
      return DoDispatch(&NotificationServiceHandler::Subscribe, context, owned_request);
    }
    case 4: {
      return DoDispatch(&NotificationServiceHandler::Unsubscribe, context, owned_request);
    }
    default: {
      throw horus_internal::UnknownRpcMethodError{};
    }
  }
}

AnyFuture<pb::DefaultSubscribeResponse> NotificationServiceHandler::Subscribe(const RpcContext& context,
                                                                              pb::DefaultSubscribeRequest&& request) noexcept(false) {
  static_cast<void>(pb::DefaultSubscribeRequest{std::move(request)});
  return ResolveWith(NotificationListenerServiceSubscribers().Add<pb::DefaultSubscribeResponse>(context));
}

AnyFuture<pb::DefaultUnsubscribeResponse> NotificationServiceHandler::Unsubscribe(const RpcContext& context,
                                                                                  pb::DefaultUnsubscribeRequest&& request) noexcept(false) {
  static_cast<void>(pb::DefaultUnsubscribeRequest{std::move(request)});
  return ResolveWith(NotificationListenerServiceSubscribers().Remove<pb::DefaultUnsubscribeResponse>(context));
}

AnyFuture<pb::RpcMessage> NotificationListenerServiceHandler::Handle(const RpcContext& context,
                                                                     pb::RpcMessage&& request) noexcept(false) {
  const pb::RpcMessage owned_request{std::move(request)};
  switch (owned_request.method_id()) {
    case 1: {
      return DoDispatch(&NotificationListenerServiceHandler::NotifyLogMessage, context, owned_request);
    }
    case 2: {
      return DoDispatch(&NotificationListenerServiceHandler::NotifyProfilingInfo, context, owned_request);
    }
    case 3: {
      return DoDispatch(&NotificationListenerServiceHandler::NotifySensorInfo, context, owned_request);
    }
    default: {
      throw horus_internal::UnknownRpcMethodError{};
    }
  }
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus
