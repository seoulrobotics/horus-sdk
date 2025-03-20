#include "horus/pb/status_service/service_handler.h"

#include <utility>

namespace horus {
namespace sdk {
namespace pb {

AnyFuture<pb::RpcMessage> StatusServiceHandler::Handle(const RpcContext& context,
                                                       pb::RpcMessage&& request) noexcept(false) {
  const pb::RpcMessage owned_request{std::move(request)};
  switch (owned_request.method_id()) {
    case 1: {
      return DoDispatch(&StatusServiceHandler::GetVersion, context, owned_request);
    }
    default: {
      throw horus_internal::UnknownRpcMethodError{};
    }
  }
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus
