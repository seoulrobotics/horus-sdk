/// @file
///
/// The RPC `RpcBaseClient` class.

#ifndef HORUS_RPC_BASE_CLIENT_H_
#define HORUS_RPC_BASE_CLIENT_H_

#include <cstdint>
#include <memory>
#include <utility>

#include "horus/future/any.h"
#include "horus/future/map.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/message.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/pb/serialize.h"
#include "horus/rpc/endpoint.h"
#include "horus/types/string_view.h"

namespace horus {
namespace horus_internal {

/// Base class for generated RPC clients.
class RpcBaseClient {
 public:
  /// Constructs a client which communicates to the remote service using the given `endpoint`.
  ///
  /// `endpoint` may be null, in which case RPCs may **not** be sent.
  explicit RpcBaseClient(std::shared_ptr<RpcEndpoint> endpoint) noexcept
      : endpoint_{std::move(endpoint)} {}

  /// Virtual destructor for inheritance.
  virtual ~RpcBaseClient() = default;

  /// The internal identifier of the service this client communicates with.
  virtual std::uint16_t ServiceId() const noexcept = 0;

  /// The full name of the service this client communicates with, e.g. "horus.pb.FooService".
  virtual StringView ServiceFullName() const noexcept = 0;

  /// The short name of the service this client communicates with, e.g. "FooService".
  StringView ServiceName() const noexcept;

  /// The endpoint the client communicates with.
  std::shared_ptr<RpcEndpoint> Endpoint() const noexcept { return endpoint_; }

 protected:
  /// Copyable.
  RpcBaseClient(const RpcBaseClient&) noexcept = default;
  /// Copyable.
  RpcBaseClient& operator=(const RpcBaseClient&) noexcept = default;
  /// Movable.
  RpcBaseClient(RpcBaseClient&&) noexcept = default;
  /// Movable.
  RpcBaseClient& operator=(RpcBaseClient&&) noexcept = default;

  /// Sends the given `request` to the underlying endpoint as a call to the specified method,
  /// returning a future which will complete with the response.
  template <class Response>
  AnyFuture<Response> InvokeRpc(std::uint16_t method_id, const PbMessage& request,
                                const RpcOptions& options) {
    return Map(endpoint_->SendWithResponse(MakeRequestMessage(method_id, request), options),
               [](sdk::pb::RpcMessage&& message) -> Response {
                 sdk::pb::RpcMessage owned_message{std::move(message)};
                 PbReader reader{std::move(owned_message.mutable_message_bytes()).View()};
                 return Response{reader};
               });
  }

  /// Sends the given `request` to the underlying endpoint as a call to the specified method,
  /// returning a future which will complete when the message has been sent.
  AnyFuture<void> InvokeOneWayRpc(std::uint16_t method_id, const PbMessage& request,
                                  const RpcOptions& options) {
    return endpoint_->Send(MakeRequestMessage(method_id, request), options);
  }

 private:
  /// Constructs a `RpcMessage` which can be given to `Invoke*Rpc()`.
  sdk::pb::RpcMessage MakeRequestMessage(std::uint16_t method_id, const PbMessage& request) const;

  /// The endpoint to communicate with.
  std::shared_ptr<RpcEndpoint> endpoint_;
};

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_RPC_BASE_CLIENT_H_
