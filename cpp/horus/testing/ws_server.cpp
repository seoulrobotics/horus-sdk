#include "horus/testing/ws_server.h"

#include <ixwebsocket/IXConnectionState.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXWebSocketMessage.h>
#include <ixwebsocket/IXWebSocketMessageType.h>
#include <ixwebsocket/IXWebSocketServer.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <uv.h>

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>

#include "horus/event_loop/uv.h"
#include "horus/internal/pointer_cast.h"
#include "horus/pb/buffer.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/pb/serialize.h"
#include "horus/rpc/base_handler.h"
#include "horus/rpc/endpoint.h"
#include "horus/rpc/retry_policy.h"
#include "horus/strings/str_cat.h"
#include "horus/testing/event_loop.h"

namespace horus {
namespace {

/// Finds a free port to listen to for the test WS server.
///
/// `ix::WebSocketServer` _can_ start with `port=0`, but it doesn't expose which port it is actually
/// listening to anywhere. Therefore to connect to the server, we need to first bind to any TCP port
/// using libuv, then close the server, then start the server again on the same port, hoping that
/// the OS hasn't reused it for some other purpose.
std::uint16_t FindFreePort() noexcept {
  uv_loop_t loop{};
  horus_internal::UvAssert(uv_loop_init(&loop));

  // Bind TCP server on port 0.
  uv_tcp_t tcp_handle{};
  horus_internal::UvAssert(uv_tcp_init(&loop, &tcp_handle));
  sockaddr_in addr{};
  horus_internal::UvAssert(uv_ip4_addr("127.0.0.1", 0, &addr));
  horus_internal::UvAssert(
      uv_tcp_bind(&tcp_handle, horus_internal::UnsafePointerCast<sockaddr>(&addr), 0));
  horus_internal::UvAssert(uv_run(&loop, UV_RUN_DEFAULT));

  // Get address of bound TCP server.
  std::int32_t addr_len{sizeof(addr)};
  horus_internal::UvAssert(uv_tcp_getsockname(
      &tcp_handle, horus_internal::UnsafePointerCast<sockaddr>(&addr), &addr_len));
  horus_internal::UvAssert(uv_run(&loop, UV_RUN_DEFAULT));

  // Get port assigned to server.
  std::uint16_t const port{addr.sin_port};

  // Shutdown server and loop.
  uv_close(&horus_internal::UvToHandle(tcp_handle), nullptr);
  horus_internal::UvAssert(uv_run(&loop, UV_RUN_DEFAULT));
  horus_internal::UvAssert(uv_loop_close(&loop));

  return port;
}

}  // namespace

WebSocketServer::WebSocketServer(MessageHandler&& handler) : message_handler_{std::move(handler)} {
  std::uint8_t tries{0};
  constexpr std::uint8_t kMaxTries{100};
  for (;;) {
    // `ix::WebSocketServer` doesn't allow us to get the port of the server when we use a free port
    // (0). Instead, we use libuv to get a free port and start the server with it.
    // If we're unlucky, though, the port will be taken by the time the server starts, so we do this
    // in a loop until we succeed.
    port_ = FindFreePort();
    server_ = std::make_unique<ix::WebSocketServer>(port_, "127.0.0.1");
    server_->setOnConnectionCallback(
        [this](const std::weak_ptr<ix::WebSocket>& websocket,
               const std::shared_ptr<ix::ConnectionState> /* connection_state */) {
          websocket.lock()->setOnMessageCallback(
              [this, endpoint{std::make_shared<BasicWebSocketEndpoint>(websocket)}](
                  const std::unique_ptr<ix::WebSocketMessage>& message) {
                message_handler_(endpoint, *message);
              });
        });
    const auto result = server_->listen();
    if (result.first) {
      break;  // Success!
    }
    ++tries;
    if (tries == kMaxTries) {
      throw std::runtime_error{
          StrCat("could not find free port for test WS server: ", result.second)};
    }
  }
  server_->start();
}

WebSocketServer::MessageHandler HandleMessagesWith(horus_internal::RpcBaseHandler& handler) {
  return [&handler](const std::shared_ptr<BasicWebSocketEndpoint>& endpoint,
                    const ix::WebSocketMessage& ws_message) {
    if (ws_message.type != ix::WebSocketMessageType::Message) {
      return;
    }
    PbReader reader{PbView{PbBuffer::Borrowed(ws_message.str)}};
    pb::RpcMessage rpc_message{reader};
    const bool is_two_way{rpc_message.request_id() != kOneWayRpcRequestId};

    const RpcContext context{endpoint};
    pb::RpcMessage result_message{TestOnlyExecute(handler.Handle(context, std::move(rpc_message)))};
    if (is_two_way) {
      endpoint->Send(std::move(result_message), RetryServerClientDefault());
    }
  };
}

}  // namespace horus
