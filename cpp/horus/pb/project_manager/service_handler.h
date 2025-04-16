#ifndef HORUS_PB_PROJECT_MANAGER_SERVICE_HANDLER_H_
#define HORUS_PB_PROJECT_MANAGER_SERVICE_HANDLER_H_

#include <cstdint>

#include "horus/future/any.h"
#include "horus/pb/project_manager/service_pb.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/rpc/base_handler.h"
#include "horus/rpc/endpoint.h"
#include "horus/strings/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

namespace horus {
namespace sdk {
namespace pb {

/// Service which stores configuration for all other services and coordinates updates to this configuration.
class ProjectManagerServiceHandler : public horus_internal::RpcBaseHandler {
 public:
  /// The request type of `GetHealthStatus()`.
  using GetHealthStatusRequestType = pb::GetHealthStatusRequest;
  /// The response type of `GetHealthStatus()`.
  using GetHealthStatusResponseType = pb::GetHealthStatusResponse;

  /// Default constructor.
  ProjectManagerServiceHandler() noexcept = default;

  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 3; }
  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  StringView ServiceFullName() const noexcept final { return "horus.pb.ProjectManagerService"; }

  /// @copydoc horus_internal::RpcBaseHandler::Handle()
  AnyFuture<pb::RpcMessage> Handle(const RpcContext& context,
                                   pb::RpcMessage&& request) noexcept(false) final;

 protected:

  /// Provides horus health indicators.
  virtual AnyFuture<pb::GetHealthStatusResponse> GetHealthStatus(const RpcContext& context,
                                                                 pb::GetHealthStatusRequest&& request) noexcept(false) = 0;
};

/// A `ProjectManagerServiceHandler` which forwards received messages to function objects.
template <class SharedState, class OnGetHealthStatus>
class FunctionalProjectManagerService final : public ProjectManagerServiceHandler {
 public:
  /// Constructs a `FunctionalProjectManagerService` which forwards received messages to function objects.
  FunctionalProjectManagerService(SharedState shared_state, OnGetHealthStatus&& on_get_health_status) noexcept
      : shared_state_{std::forward<SharedState>(shared_state)}
      , on_get_health_status_{std::move(on_get_health_status)} {}

  /// Returns a `FunctionalProjectManagerService` which handles `GetHealthStatus()` with `on_get_health_status()`.
  template <class F>
  FunctionalProjectManagerService<SharedState, F>
  GetHealthStatusWith(F&& on_get_health_status) && noexcept {
    return FunctionalProjectManagerService<SharedState, F>{
        std::move(shared_state_), std::forward<F>(on_get_health_status)};
  }

 protected:
  /// Forwards `GetHealthStatus()` to `OnGetHealthStatus`.
  AnyFuture<pb::GetHealthStatusResponse> GetHealthStatus(const RpcContext& context,
                                                         pb::GetHealthStatusRequest&& request) noexcept(false) final {
    return horus_internal::ForwardToFunctionalHandler<pb::GetHealthStatusResponse>(shared_state_, on_get_health_status_, context, std::move(request));
  }
 private:
  /// State shared between all callbacks.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS SharedState shared_state_;
  /// Handler of `GetHealthStatus`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnGetHealthStatus on_get_health_status_;
};

/// Returns a `FunctionalProjectManagerService` with the given shared state.
template <class SharedState>
FunctionalProjectManagerService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::GetHealthStatusRequest, pb::GetHealthStatusResponse>)>
CreateFunctionalProjectManagerService(SharedState&& shared_state) noexcept {
  return FunctionalProjectManagerService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::GetHealthStatusRequest, pb::GetHealthStatusResponse>)>{
      std::forward<SharedState>(shared_state), &horus_internal::IgnoreRpc<pb::GetHealthStatusRequest, pb::GetHealthStatusResponse>};
}

/// Returns a `FunctionalProjectManagerService` with no shared state.
inline FunctionalProjectManagerService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::GetHealthStatusRequest, pb::GetHealthStatusResponse>)>
CreateFunctionalProjectManagerService() noexcept {
  return FunctionalProjectManagerService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::GetHealthStatusRequest, pb::GetHealthStatusResponse>)>{
      nullptr, &horus_internal::IgnoreRpc<pb::GetHealthStatusRequest, pb::GetHealthStatusResponse>};
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus

#endif  // HORUS_PB_PROJECT_MANAGER_SERVICE_HANDLER_H_
