/// @file
///
/// The `WebSocketConnect()` function.

#ifndef HORUS_RPC_WS_H_
#define HORUS_RPC_WS_H_

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "horus/event_loop/event_loop.h"
#include "horus/future/any.h"
#include "horus/rpc/endpoint.h"
#include "horus/strings/string_view.h"

namespace horus {
namespace horus_internal {

/// Converts an address/port pair to an URL.
std::string AddressPortPairToUrl(StringView address, std::uint16_t port);

}  // namespace horus_internal

/// Returns an `RpcEndpoint` which connects via WebSocket to the given URL.
std::shared_ptr<RpcEndpoint> WebSocketConnect(horus_internal::EventLoop& event_loop,
                                              std::string&& url,
                                              MessageHandler&& message_handler) noexcept(false);

/// Returns an `RpcEndpoint` which connects via WebSocket to the given address and port.
inline std::shared_ptr<RpcEndpoint> WebSocketConnect(
    horus_internal::EventLoop& event_loop, StringView address, std::uint16_t port,
    MessageHandler&& message_handler) noexcept(false) {
  return WebSocketConnect(event_loop, horus_internal::AddressPortPairToUrl(address, port),
                          std::move(message_handler));
}

/// Returns a future which resolves with a `RpcEndpoint` connecting via WebSocket to the given URL.
AnyFuture<std::shared_ptr<RpcEndpoint>> ConnectingWebSocket(
    std::string&& url, MessageHandler&& message_handler) noexcept(false);

/// Returns a future which resolves with a `RpcEndpoint` connecting via WebSocket to the given
/// address and port.
inline AnyFuture<std::shared_ptr<RpcEndpoint>> ConnectingWebSocket(
    StringView address, std::uint16_t port, MessageHandler&& message_handler) noexcept(false) {
  return ConnectingWebSocket(horus_internal::AddressPortPairToUrl(address, port),
                             std::move(message_handler));
}

/// Returns a future which resolves with a `RpcEndpoint` connected via WebSocket to the given URL.
AnyFuture<std::shared_ptr<RpcEndpoint>> ConnectedWebSocket(
    std::string&& url, MessageHandler&& message_handler) noexcept(false);

/// Returns a future which resolves with a `RpcEndpoint` connected via WebSocket to the given
/// address and port.
inline AnyFuture<std::shared_ptr<RpcEndpoint>> ConnectedWebSocket(
    StringView address, std::uint16_t port, MessageHandler&& message_handler) noexcept(false) {
  return ConnectedWebSocket(horus_internal::AddressPortPairToUrl(address, port),
                            std::move(message_handler));
}

}  // namespace horus

#endif  // HORUS_RPC_WS_H_
