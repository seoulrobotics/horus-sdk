/// @file
///
/// The `CreateClientHandler()` function

#ifndef HORUS_RPC_CLIENT_HANDLER_H_
#define HORUS_RPC_CLIENT_HANDLER_H_

#include <array>
#include <cstddef>
#include <memory>
#include <tuple>
#include <utility>

#include "horus/future/any.h"
#include "horus/future/resolved.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/rpc/base_handler.h"
#include "horus/rpc/endpoint.h"

namespace horus {
namespace horus_internal {

/// Converts a tuple of handlers into an array of pointers to these handlers.
template <class... Handlers, std::size_t... Indices>
std::array<RpcBaseHandler*, sizeof...(Handlers)> HandlersToArray(
    std::tuple<Handlers...>& handlers, std::index_sequence<Indices...> /* indices */) noexcept {
  return std::array<RpcBaseHandler*, sizeof...(Handlers)>{&std::get<Indices>(handlers)...};
}

}  // namespace horus_internal

/// Returns a `MessageHandler` which dispatches received messages to the corresponding handler.
template <class... Handlers>  // NOLINTNEXTLINE(*-missing-std-forward): false positive
MessageHandler CreateClientHandler(Handlers&&... handlers) noexcept(false) {
  return [tuple{std::make_shared<std::tuple<Handlers...>>(std::forward<Handlers>(handlers)...)}](
             const RpcContext& context,
             pb::RpcMessage&& message) mutable -> AnyFuture<pb::RpcMessage> {
    for (horus_internal::RpcBaseHandler* const handler :
         horus_internal::HandlersToArray(*tuple, std::make_index_sequence<sizeof...(Handlers)>{})) {
      if (handler->ServiceId() == message.service_id()) {
        try {
          return handler->Handle(context, std::move(message));
        } catch (const horus_internal::UnknownRpcMethodError&) {
          if (message.request_id() != kOneWayRpcRequestId) {
            throw;
          }
          // We received a one-way event the SDK isn't built to handle, which doesn't really matter.
        }
      }
    }
    static_cast<void>(pb::RpcMessage{std::move(message)});
    return ResolveWith(pb::RpcMessage{});
  };
}

}  // namespace horus

#endif  // HORUS_RPC_CLIENT_HANDLER_H_
