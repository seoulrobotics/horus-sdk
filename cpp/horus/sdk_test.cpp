#include "horus/sdk.h"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>

#include "horus/event_loop/event_loop.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/logs/logs_pb.h"
#include "horus/pb/logs/message_pb.h"
#include "horus/pb/notification_service/service_client.h"
#include "horus/pb/notification_service/service_handler.h"
#include "horus/pb/notification_service/service_pb.h"
#include "horus/pb/rpc_pb.h"
#include "horus/rpc/endpoint.h"
#include "horus/rpc/retry_policy.h"
#include "horus/testing/timing.h"
#include "horus/testing/ws_server.h"

namespace horus {
namespace {

constexpr std::chrono::milliseconds kWaitForNetwork{20};
constexpr std::chrono::milliseconds kWaitForBackgroundThread{10};

TEST(Sdk, SubscribeToLogs) {
  FLAKY_BLOCK() {
    std::mutex endpoint_mtx;
    std::weak_ptr<RpcEndpoint> weak_endpoint{};

    auto notification_service =
        pb::CreateFunctionalNotificationService()
            .SubscribeWith([&endpoint_mtx, &weak_endpoint](
                               const RpcContext& context,
                               const pb::DefaultSubscribeRequest&) -> pb::DefaultSubscribeResponse {
              const std::unique_lock<std::mutex> lock{endpoint_mtx};
              weak_endpoint = context.Endpoint();
              return {};
            })
            .UnsubscribeWith(
                [](const pb::DefaultUnsubscribeRequest&) -> pb::DefaultUnsubscribeResponse {
                  ADD_FAILURE() << "Unsubscribe() should never be explicitly called";
                  return {};
                });
    WebSocketServer server{HandleMessagesWith(notification_service)};
    horus_internal::EventLoop event_loop;

    const auto get_endpoint = [&endpoint_mtx, &weak_endpoint]() -> std::shared_ptr<RpcEndpoint> {
      const std::unique_lock<std::mutex> lock{endpoint_mtx};
      return weak_endpoint.lock();
    };

    Sdk sdk{Sdk::ServiceResolutionMap{/*notification=*/{"127.0.0.1", server.Port()}}};
    std::atomic<std::int32_t> log_count{0};
    FLAKY_EXPECT_EQ(get_endpoint(), nullptr);
    FLAKY_EXPECT_EQ(server.ClientCount(), 0);
    {
      const SdkSubscription subscription{
          sdk.SubscribeToLogs(
                 {/*on_log_message=*/[&log_count](const pb::LogMessage&) { ++log_count; }})
              .WaitFor(kWaitForNetwork)
              .As<SdkSubscription>()};

      // The subscription happens in the background, so we need to sleep to await it.
      std::this_thread::sleep_for(kWaitForBackgroundThread);
      FLAKY_EXPECT_NE(get_endpoint(), nullptr);
      FLAKY_EXPECT_EQ(server.ClientCount(), 1);

      FLAKY_EXPECT_EQ(log_count, 0);
      {
        std::shared_ptr<RpcEndpoint> endpoint{get_endpoint()};
        FLAKY_ASSERT_NE(endpoint, nullptr);

        const auto event = pb::LogMessageEvent{}.set_log_message(
            pb::LogMessage{}.set_data(pb::LogData{}.set_generic(
                pb::logs::Generic{}.set_message(CowBytes::Borrowed("hello")))));

        event_loop.RunFuture(
            pb::NotificationListenerServiceClient{std::move(endpoint)}.NotifyLogMessage(
                event, RetryIndefinitely(std::chrono::milliseconds{1})));
      }
      std::this_thread::sleep_for(kWaitForBackgroundThread);
      FLAKY_EXPECT_EQ(log_count, 1);
    }
    // Let `Sdk` event-loop destroy the subscription in the background.
    std::this_thread::sleep_for(kWaitForBackgroundThread);

    FLAKY_EXPECT_EQ(get_endpoint(), nullptr);
    FLAKY_EXPECT_EQ(server.ClientCount(), 0);
  }
}

}  // namespace
}  // namespace horus
