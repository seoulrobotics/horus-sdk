#ifndef HORUS_PB_STATUS_SERVICE_SERVICE_CLIENT_H_
#define HORUS_PB_STATUS_SERVICE_SERVICE_CLIENT_H_

#include <cstdint>

#include "horus/future/any.h"
#include "horus/pb/status_service/service_pb.h"
#include "horus/rpc/base_client.h"
#include "horus/rpc/endpoint.h"
#include "horus/strings/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

namespace horus {
namespace sdk {
namespace pb {

/// Service running in every binary, returning status information.
class StatusServiceClient : public horus_internal::RpcBaseClient {
 public:
  /// The request type of `GetVersion()`.
  using GetVersionRequestType = pb::GetVersionRequest;
  /// The response type of `GetVersion()`.
  using GetVersionResponseType = pb::GetVersionResponse;

  /// @copydoc horus_internal::RpcBaseClient::RpcBaseClient()
  using horus_internal::RpcBaseClient::RpcBaseClient;

  /// @copydoc horus_internal::RpcBaseClient::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 15; }
  /// @copydoc horus_internal::RpcBaseClient::ServiceFullName()
  StringView ServiceFullName() const noexcept final { return "horus.pb.StatusService"; }

  /// Returns the current version of horus.
  AnyFuture<pb::GetVersionResponse> GetVersion(const pb::GetVersionRequest& request, const RpcOptions& options) noexcept(false) {
    return InvokeRpc<pb::GetVersionResponse>(1, request, options);
  }
};

}  // namespace pb
}  // namespace sdk
}  // namespace horus

#endif  // HORUS_PB_STATUS_SERVICE_SERVICE_CLIENT_H_
