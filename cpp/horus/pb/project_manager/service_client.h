#ifndef HORUS_PB_PROJECT_MANAGER_SERVICE_CLIENT_H_
#define HORUS_PB_PROJECT_MANAGER_SERVICE_CLIENT_H_

#include <cstdint>

#include "horus/future/any.h"
#include "horus/pb/project_manager/service_pb.h"
#include "horus/rpc/base_client.h"
#include "horus/rpc/endpoint.h"
#include "horus/strings/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

namespace horus {
namespace sdk {
namespace pb {

/// Service which stores configuration for all other services and coordinates updates to this configuration.
class ProjectManagerServiceClient : public horus_internal::RpcBaseClient {
 public:
  /// The request type of `GetHealthStatus()`.
  using GetHealthStatusRequestType = pb::GetHealthStatusRequest;
  /// The response type of `GetHealthStatus()`.
  using GetHealthStatusResponseType = pb::GetHealthStatusResponse;

  /// @copydoc horus_internal::RpcBaseClient::RpcBaseClient()
  using horus_internal::RpcBaseClient::RpcBaseClient;

  /// @copydoc horus_internal::RpcBaseClient::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 3; }
  /// @copydoc horus_internal::RpcBaseClient::ServiceFullName()
  StringView ServiceFullName() const noexcept final { return "horus.pb.ProjectManagerService"; }

  /// Provides horus health indicators.
  AnyFuture<pb::GetHealthStatusResponse> GetHealthStatus(const pb::GetHealthStatusRequest& request, const RpcOptions& options) noexcept(false) {
    return InvokeRpc<pb::GetHealthStatusResponse>(38, request, options);
  }
};

}  // namespace pb
}  // namespace sdk
}  // namespace horus

#endif  // HORUS_PB_PROJECT_MANAGER_SERVICE_CLIENT_H_
