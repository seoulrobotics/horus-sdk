#include "horus/rpc/ws.h"

#include <gtest/gtest.h>
#include <horus/testing/timing.h>
#include <ixwebsocket/IXWebSocketMessage.h>

#include <atomic>
#include <memory>
#include <utility>

#include "horus/future/any.h"
#include "horus/future/attach.h"
#include "horus/future/from_continuation.h"
#include "horus/future/join.h"
#include "horus/future/map.h"
#include "horus/future/map_to.h"
#include "horus/future/resolved.h"
#include "horus/future/then.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/logs/logs_pb.h"
#include "horus/pb/logs/message_pb.h"
#include "horus/pb/notification_service/service_client.h"
#include "horus/pb/notification_service/service_handler.h"
#include "horus/pb/notification_service/service_pb.h"
#include "horus/pb/rpc_pb.h"
#include "horus/rpc/client_handler.h"
#include "horus/rpc/endpoint.h"
#include "horus/rpc/retry_policy.h"
#include "horus/testing/event_loop.h"
#include "horus/testing/ws_server.h"
#include "horus/types/string_view.h"

namespace horus {
namespace {

TEST(WebSockets, Connect) {
  FLAKY_BLOCK() {
    std::atomic<bool> client_seen{false};
    std::atomic<bool> server_seen{false};

    const WebSocketServer server{
        [&server_seen](const std::shared_ptr<BasicWebSocketEndpoint>&,
                       const ix::WebSocketMessage&) { server_seen = true; }};

    FLAKY_EXPECT_NO_THROW(
        TestOnlyExecute(ConnectedWebSocket("127.0.0.1", server.Port(), NoMessageHandler()) |
                        Map([&client_seen](const std::shared_ptr<RpcEndpoint>& /* endpoint */) {
                          client_seen = true;
                        })));

    FLAKY_EXPECT_TRUE(client_seen.load());
    FLAKY_EXPECT_TRUE(server_seen.load());
  }
}

TEST(WebSockets, Rpc) {
  // Server-side NotificationService handler
  // -----

  bool subscribed{false};

  auto notification_service =
      pb::CreateFunctionalNotificationService()
          .LogMessageWith([&subscribed](const RpcContext& context,
                                        const pb::LogMessageRequest& request) -> AnyFuture<void> {
            if (subscribed) {
              return pb::NotificationListenerServiceClient{context.Endpoint()}.NotifyLogMessage(
                  pb::LogMessageEvent{}.set_log_message(pb::LogMessage{request.log_message()}),
                  RetryServerClientDefault());
            }
            return ResolvedFuture<void>{};
          })
          .SubscribeWith(
              [&subscribed](const pb::DefaultSubscribeRequest&) -> pb::DefaultSubscribeResponse {
                subscribed = true;
                return {};
              })
          .UnsubscribeWith([&subscribed](const pb::DefaultUnsubscribeRequest&)
                               -> pb::DefaultUnsubscribeResponse {
            subscribed = false;
            return {};
          });
  const WebSocketServer server{HandleMessagesWith(notification_service)};

  // Client-side NotificationListenerService handler
  // -----

  auto future_and_continuation = FromContinuation<void>();
  bool received_log{false};

  MessageHandler message_handler{
      CreateClientHandler(pb::CreateFunctionalNotificationListenerService().NotifyLogMessageWith(
          [continuation{std::move(future_and_continuation.second)},
           &received_log](const pb::LogMessageEvent& event) mutable {
            EXPECT_EQ(event.log_message().data().generic().message().Str(), "hello");
            received_log = true;
            static_cast<void>(continuation.ContinueWith());
          }))};

  // Client-side NotificationService client
  // ----

  TestOnlyExecute(
      ConnectedWebSocket("127.0.0.1", server.Port(), std::move(message_handler)) |
      Then([message_received{std::move(future_and_continuation.first)}](
               std::shared_ptr<RpcEndpoint>&& endpoint) mutable -> auto {
        pb::NotificationServiceClient client{std::move(endpoint)};
        return client.Subscribe({}, RetryClientDefault()) |
               Then([client, future{std::move(message_received)}](
                        const pb::DefaultSubscribeResponse& response) mutable -> auto {
                 EXPECT_FALSE(response.has_connection_error());

                 return Join(
                     MapTo(client.LogMessage(
                               pb::LogMessageRequest{}.set_log_message(pb::LogMessage{}.set_data(
                                   pb::LogData{}.set_generic(pb::logs::Generic{}.set_message(
                                       CowBytes::Borrowed("hello"))))),
                               RetryClientDefault()),
                           0),
                     MapTo(std::move(future), 0));
               }) |
               MapToVoid() | Then([client]() mutable -> auto {
                 return Attach(client.Unsubscribe({}, RetryClientDefault()), client);
               });
      }));

  EXPECT_TRUE(received_log);
}

}  // namespace
}  // namespace horus
