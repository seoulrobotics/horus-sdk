#include "horus/pb/project_manager/service_handler.h"

#include <utility>

namespace horus {
namespace sdk {
namespace pb {

AnyFuture<pb::RpcMessage> ProjectManagerServiceHandler::Handle(const RpcContext& context,
                                                               pb::RpcMessage&& request) noexcept(false) {
  const pb::RpcMessage owned_request{std::move(request)};
  switch (owned_request.method_id()) {
    case 38: {
      return DoDispatch(&ProjectManagerServiceHandler::GetHealthStatus, context, owned_request);
    }
    default: {
      throw horus_internal::UnknownRpcMethodError{};
    }
  }
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus
