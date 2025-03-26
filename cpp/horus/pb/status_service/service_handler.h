#ifndef HORUS_PB_STATUS_SERVICE_SERVICE_HANDLER_H_
#define HORUS_PB_STATUS_SERVICE_SERVICE_HANDLER_H_

#include <cstdint>

#include "horus/future/any.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/pb/status_service/service_pb.h"
#include "horus/rpc/base_handler.h"
#include "horus/rpc/endpoint.h"
#include "horus/strings/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

namespace horus {
namespace sdk {
namespace pb {

/// Service running in every binary, returning status information.
class StatusServiceHandler : public horus_internal::RpcBaseHandler {
 public:
  /// The request type of `GetVersion()`.
  using GetVersionRequestType = pb::GetVersionRequest;
  /// The response type of `GetVersion()`.
  using GetVersionResponseType = pb::GetVersionResponse;

  /// Default constructor.
  StatusServiceHandler() noexcept = default;

  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 15; }
  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  StringView ServiceFullName() const noexcept final { return "horus.pb.StatusService"; }

  /// @copydoc horus_internal::RpcBaseHandler::Handle()
  AnyFuture<pb::RpcMessage> Handle(const RpcContext& context,
                                   pb::RpcMessage&& request) noexcept(false) final;

 protected:

  /// Returns the current version of horus.
  virtual AnyFuture<pb::GetVersionResponse> GetVersion(const RpcContext& context,
                                                       pb::GetVersionRequest&& request) noexcept(false) = 0;
};

/// A `StatusServiceHandler` which forwards received messages to function objects.
template <class SharedState, class OnGetVersion>
class FunctionalStatusService final : public StatusServiceHandler {
 public:
  /// Constructs a `FunctionalStatusService` which forwards received messages to function objects.
  FunctionalStatusService(SharedState shared_state, OnGetVersion&& on_get_version) noexcept
      : shared_state_{std::forward<SharedState>(shared_state)}
      , on_get_version_{std::move(on_get_version)} {}

  /// Returns a `FunctionalStatusService` which handles `GetVersion()` with `on_get_version()`.
  template <class F>
  FunctionalStatusService<SharedState, F>
  GetVersionWith(F&& on_get_version) && noexcept {
    return FunctionalStatusService<SharedState, F>{
        std::move(shared_state_), std::forward<F>(on_get_version)};
  }

 protected:
  /// Forwards `GetVersion()` to `OnGetVersion`.
  AnyFuture<pb::GetVersionResponse> GetVersion(const RpcContext& context,
                                               pb::GetVersionRequest&& request) noexcept(false) final {
    return horus_internal::ForwardToFunctionalHandler<pb::GetVersionResponse>(shared_state_, on_get_version_, context, std::move(request));
  }
 private:
  /// State shared between all callbacks.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS SharedState shared_state_;
  /// Handler of `GetVersion`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnGetVersion on_get_version_;
};

/// Returns a `FunctionalStatusService` with the given shared state.
template <class SharedState>
FunctionalStatusService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::GetVersionRequest, pb::GetVersionResponse>)>
CreateFunctionalStatusService(SharedState&& shared_state) noexcept {
  return FunctionalStatusService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::GetVersionRequest, pb::GetVersionResponse>)>{
      std::forward<SharedState>(shared_state), &horus_internal::IgnoreRpc<pb::GetVersionRequest, pb::GetVersionResponse>};
}

/// Returns a `FunctionalStatusService` with no shared state.
inline FunctionalStatusService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::GetVersionRequest, pb::GetVersionResponse>)>
CreateFunctionalStatusService() noexcept {
  return FunctionalStatusService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::GetVersionRequest, pb::GetVersionResponse>)>{
      nullptr, &horus_internal::IgnoreRpc<pb::GetVersionRequest, pb::GetVersionResponse>};
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus

#endif  // HORUS_PB_STATUS_SERVICE_SERVICE_HANDLER_H_
