/// @file
///
/// The `WebSocketServer` class.

#ifndef HORUS_TESTING_WS_SERVER_H_
#define HORUS_TESTING_WS_SERVER_H_

#include <gtest/gtest.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXWebSocketMessage.h>
#include <ixwebsocket/IXWebSocketServer.h>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "horus/attributes.h"
#include "horus/future/any.h"
#include "horus/future/resolved.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/rpc/base_handler.h"
#include "horus/rpc/endpoint.h"

namespace horus {

/// A basic `RpcEndpoint` which can only send blocking messages to a server-owned `ix::WebSocket`.
class BasicWebSocketEndpoint final : public RpcEndpoint {
 public:
  explicit BasicWebSocketEndpoint(const std::weak_ptr<ix::WebSocket>& websocket) noexcept
      : websocket_{websocket} {}

  StringView Uri() const noexcept final { return "test-websocket:0"; }

  AnyFuture<void> Send(pb::RpcMessage&& message, const RpcOptions& options) noexcept(false) final {
    static_cast<void>(options);
    const std::shared_ptr<ix::WebSocket> websocket{websocket_.lock()};
    if (websocket == nullptr) {
      throw std::runtime_error{"websocket disconnected"};
    }
    std::vector<std::uint8_t> const data{message.SerializeToBuffer()};
    EXPECT_TRUE(websocket->sendBinary(data).success);
    static_cast<void>(pb::RpcMessage{std::move(message)});
    return ResolvedFuture<void>{};
  }

  AnyFuture<pb::RpcMessage> SendWithResponse(pb::RpcMessage&& message,
                                             const RpcOptions& options) noexcept(false) final {
    static_cast<void>(pb::RpcMessage{std::move(message)});
    static_cast<void>(options);
    throw std::runtime_error{"BasicWebSocketEndpoint::SendWithResponse() is not available"};
  }

 protected:
  void SetLifecycleEventCallback(void* receiver,
                                 void (*on_event)(void* receiver,
                                                  LifecycleEvent&& event)) noexcept final {
    static_cast<void>(receiver);
    static_cast<void>(on_event);
    FAIL() << "BasicWebSocketEndpoint::SetLifecycleEventCallback() is not available";
  }

 private:
  std::weak_ptr<ix::WebSocket> websocket_;
};

/// A WebSocket server used in tests.
class WebSocketServer final {
 public:
  using MessageHandler = std::function<void(const std::shared_ptr<BasicWebSocketEndpoint>&,
                                            const ix::WebSocketMessage&)>;

  explicit WebSocketServer(MessageHandler&& handler);

  /// Returns the port on which the server listens.
  constexpr std::uint16_t Port() const noexcept { return port_; }

  /// Returns the number of clients in the server.
  std::size_t ClientCount() noexcept { return server_->getClients().size(); }

 private:
  /// @see Port()
  std::uint16_t port_{0};
  /// WebSocket server.
  std::unique_ptr<ix::WebSocketServer> server_;
  /// Function to call when a message is received.
  MessageHandler message_handler_;
};

/// Returns a `MessageHandler` which handles incoming messages using the given RPC `handler`.
WebSocketServer::MessageHandler HandleMessagesWith(
    horus_internal::RpcBaseHandler& handler HORUS_LIFETIME_BOUND);

}  // namespace horus

#endif  // HORUS_TESTING_WS_SERVER_H_
