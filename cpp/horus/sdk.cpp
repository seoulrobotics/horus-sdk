#include "horus/sdk.h"

#include <atomic>
#include <cassert>
#include <chrono>
#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <utility>

#include "horus/event_loop/event_loop.h"
#include "horus/functional/move_only_function.h"
#include "horus/future/any.h"
#include "horus/future/attach.h"
#include "horus/future/channel.h"
#include "horus/future/future.h"
#include "horus/future/loop.h"
#include "horus/future/map.h"
#include "horus/future/map_to.h"
#include "horus/future/rejected.h"
#include "horus/future/resolved.h"
#include "horus/future/then.h"
#include "horus/future/try.h"
#include "horus/logs/format.h"  // IWYU pragma: keep
#include "horus/pb/cow.h"
#include "horus/pb/detection_service/detection_pb.h"
#include "horus/pb/detection_service/detection_service_client.h"
#include "horus/pb/detection_service/detection_service_handler.h"
#include "horus/pb/logs/message_pb.h"
#include "horus/pb/notification_service/service_client.h"
#include "horus/pb/notification_service/service_handler.h"
#include "horus/pb/notification_service/service_pb.h"
#include "horus/pb/point/point_message_pb.h"
#include "horus/pb/point_aggregator/point_aggregator_service_client.h"
#include "horus/pb/point_aggregator/point_aggregator_service_handler.h"
#include "horus/pb/preprocessing/messages_pb.h"
#include "horus/pb/profiling_pb.h"
#include "horus/pb/project_manager/service_client.h"
#include "horus/pb/project_manager/service_pb.h"
#include "horus/pb/rpc_pb.h"
#include "horus/pb/status_service/service_client.h"
#include "horus/pb/status_service/service_pb.h"
#include "horus/rpc/base_client.h"
#include "horus/rpc/client_handler.h"
#include "horus/rpc/endpoint.h"
#include "horus/rpc/retry_policy.h"
#include "horus/rpc/ws.h"
#include "horus/sdk/health.h"
#include "horus/sdk/logs.h"
#include "horus/sdk/objects.h"
#include "horus/sdk/point_clouds.h"
#include "horus/sdk/profiling.h"
#include "horus/sdk/sensor.h"
#include "horus/sdk/version.h"
#include "horus/strings/logging.h"
#include "horus/types/in_place.h"

namespace horus {

Sdk::Sdk(const ServiceResolutionMap& services) noexcept(false)
    : service_map_{services},
      task_channel_{/*capacity=*/64},
      event_loop_thread_{&Sdk::RunEventLoop, std::move(task_channel_.GetReceiver())} {}

Sdk::~Sdk() noexcept {
  try {
    // Send a null function to indicate we're shutting down.
    task_channel_.GetSender().SendOrWait(nullptr);
  } catch (const ChannelClosedError&) {
    // This isn't a big deal, but it's not supposed to happen so we only do something about it
    // when assertions are enabled.
    assert(false);
  }
  event_loop_thread_.join();
}

template <class F>
SdkFuture<FutureResult<F>> Sdk::CreateFuture(F&& future) noexcept(false) {
  using T = FutureResult<F>;
  using State = FutureState<T>;

  // Construct the state with a dummy future, since we need to store a weak pointer to the state
  // from within the real future.
  std::shared_ptr<State> state{
      std::make_shared<State>(*this, RejectedFuture<void>{std::bad_function_call{}})};
  AnyFuture<void> final_future{
      std::forward<F>(future) | Map([weak_state{std::weak_ptr<State>{state}}](T&& value) {
        const std::shared_ptr<State> locked_state{weak_state.lock()};
        if (locked_state == nullptr) {
          // We are being cancelled, but the future was resolved in the meantime.
          return;
        }
        const std::unique_lock<std::mutex> lock{locked_state->mutex};
        locked_state->value.template Emplace<1>(std::move(value));
        locked_state->completion_cv.notify_all();
        if (locked_state->on_completion != nullptr) {
          locked_state->on_completion(*locked_state);
        }
      }) |
      Catch([weak_state{std::weak_ptr<State>{state}}](const std::exception_ptr& exn) {
        const std::shared_ptr<State> locked_state{weak_state.lock()};
        if (locked_state == nullptr) {
          // We are being cancelled, but the future was rejected in the meantime.
          return;
        }
        const std::unique_lock<std::mutex> lock{locked_state->mutex};
        locked_state->value.template Emplace<2>(exn);
        locked_state->completion_cv.notify_all();
        if (locked_state->on_completion != nullptr) {
          locked_state->on_completion(*locked_state);
        }
      })};
  state->value.template Emplace<0>(std::move(final_future));
  task_channel_.GetSender().SendOrWait(
      [state]() -> AnyFuture<void> { return std::move(state->value.template As<0>()); });
  return SdkFuture<T>{std::move(state)};
}

namespace {

/// Moves `property()` out of `owner` and returns it.
///
/// This helps when we have an rvalue `owner` that Autosar forces us to move out of, but we only
/// care about one of its properties.
template <class Owner, class T>
T Extract(Owner&& owner, T& (Owner::*property)() &) {
  T result{std::move((owner.*property)())};
  static_cast<void>(Owner{std::forward<Owner>(owner)});
  return result;
}

}  // namespace

template <class Client, class Subscriber, class SubscriptionRequest>
SdkFuture<SdkSubscription> Sdk::CreateSubscription(const ServiceResolutionMap::Entry& target,
                                                   Subscriber&& subscriber,
                                                   SubscriptionRequest&& request) noexcept(false) {
  return CreateFuture(
      ConnectingWebSocket(target.host, target.port,
                          CreateClientHandler(std::forward<Subscriber>(subscriber))) |
      Map([this, request2{std::forward<SubscriptionRequest>(request)},
           target](std::shared_ptr<RpcEndpoint>&& endpoint) mutable -> Subscription {
        return Subscription{
            *this, std::move(endpoint),
            [request3{std::move(request2)},
             target](const std::shared_ptr<RpcEndpoint>& event_endpoint) -> AnyFuture<void> {
              // The client doesn't hold any state so we can just recreate it every time.
              static_assert(sizeof(Client) == sizeof(horus_internal::RpcBaseClient),
                            "subscriber client should not hold any state");
              Client client{event_endpoint};
              Log("connected to ", client.ServiceName(), " via ", target.host, ":", target.port);
              return MapToVoid(client.Subscribe(request3, RetryClientDefault()));
            }};
      }));
}

SdkFuture<SdkSubscription> Sdk::SubscribeToLogs(sdk::LogSubscriptionRequest&& request) {
  auto listener = pb::CreateFunctionalNotificationListenerService().NotifyLogMessageWith(
      [this, user_callback{std::move(request).on_log_message}](
          pb::LogMessageEvent&& event) -> ChannelSendFuture<Task> {
        MoveOnlyFunction<void(pb::LogMessage&&)> move_only_user_callback{
            std::function<void(pb::LogMessage&&)>{
                user_callback}};  // Copy `user_callback` into a `MoveOnlyFunction`.
        return InvokeUserCallbackWithinEventLoop(
            std::move(move_only_user_callback),
            Extract(std::move(event), &pb::LogMessageEvent::mutable_log_message));
      });
  return CreateSubscription<pb::NotificationServiceClient>(
      service_map_.notification, std::move(listener), pb::DefaultSubscribeRequest{});
}

SdkFuture<SdkSubscription> Sdk::SubscribeToProfiling(sdk::ProfilingSubscriptionRequest&& request) {
  auto listener = pb::CreateFunctionalNotificationListenerService().NotifyProfilingInfoWith(
      [this, user_callback{std::move(request).on_profiling_info}](
          pb::ProfilingInfoEvent&& event) -> ChannelSendFuture<Task> {
        MoveOnlyFunction<void(pb::ProfilingInfo&&)> move_only_user_callback{
            std::function<void(pb::ProfilingInfo&&)>{
                user_callback}};  // Copy `user_callback` into a `MoveOnlyFunction`.
        return InvokeUserCallbackWithinEventLoop(
            std::move(move_only_user_callback),
            Extract(std::move(event), &pb::ProfilingInfoEvent::mutable_profiling_info));
      });
  return CreateSubscription<pb::NotificationServiceClient>(
      service_map_.notification, std::move(listener), pb::DefaultSubscribeRequest{});
}

SdkFuture<SdkSubscription> Sdk::SubscribeToObjects(sdk::ObjectSubscriptionRequest&& request) {
  auto listener = pb::CreateFunctionalDetectionSubscriberService().BroadcastDetectionWith(
      [this, user_callback{std::move(request).on_detection_results}](
          pb::DetectionEvent&& event) -> ChannelSendFuture<Task> {
        MoveOnlyFunction<void(pb::DetectionEvent&&)> move_only_user_callback{
            std::function<void(pb::DetectionEvent&&)>{
                user_callback}};  // Copy `user_callback` into a `MoveOnlyFunction`.
        return InvokeUserCallbackWithinEventLoop(std::move(move_only_user_callback),
                                                 std::move(event));
      });
  return CreateSubscription<pb::DetectionServiceClient>(service_map_.detection, std::move(listener),
                                                        pb::DefaultSubscribeRequest{});
}

SdkFuture<SdkSubscription> Sdk::SubscribeToOccupancyGrid(
    sdk::OccupancyGridSubscriptionRequest&& request) {
  auto listener = pb::CreateFunctionalPointAggregatorSubscriberService().BroadcastOccupancyGridWith(
      [this, user_callback{std::move(request).on_occupancy_grid}](
          pb::OccupancyGridEvent&& event) -> ChannelSendFuture<Task> {
        MoveOnlyFunction<void(pb::OccupancyGridEvent&&)> move_only_user_callback{
            std::function<void(pb::OccupancyGridEvent&&)>{
                user_callback}};  // Copy `user_callback` into a `MoveOnlyFunction`.
        return InvokeUserCallbackWithinEventLoop(std::move(move_only_user_callback),
                                                 std::move(event));
      });
  return CreateSubscription<pb::PointAggregatorServiceClient>(
      service_map_.point_aggregator, std::move(listener), pb::DefaultSubscribeRequest{});
}

SdkFuture<SdkSubscription> Sdk::SubscribeToPointClouds(
    sdk::PointCloudSubscriptionRequest&& request) {
  auto listener =
      pb::CreateFunctionalPointAggregatorSubscriberService().BroadcastProcessedPointsWith(
          [this, user_callback{std::move(request).on_point_cloud}](
              pb::AggregatedPointEvents&& aggregated_events) -> ChannelSendFuture<Task> {
            MoveOnlyFunction<void(pb::AggregatedPointEvents&&)> move_only_callback{
                [callback{user_callback}](pb::AggregatedPointEvents&& events) {
                  for (const Cow<pb::ProcessedPointsEvent>& event : events.events()) {
                    // Shallow-copy point frame.
                    callback(pb::PointFrame{event.Ref().point_frame()});
                  }
                  static_cast<void>(pb::AggregatedPointEvents{std::move(events)});
                }};
            return InvokeUserCallbackWithinEventLoop(std::move(move_only_callback),
                                                     std::move(aggregated_events));
          });
  return CreateSubscription<pb::PointAggregatorServiceClient>(
      service_map_.point_aggregator, std::move(listener), pb::DefaultSubscribeRequest{});
}

SdkFuture<SdkSubscription> Sdk::SubscribeToSensorInfo(
    sdk::SensorInfoSubscriptionRequest&& request) {
  auto listener = pb::CreateFunctionalNotificationListenerService().NotifySensorInfoWith(
      [this, user_callback{std::move(request).on_sensor_info_event}](
          pb::SensorInfoEvent&& event) -> ChannelSendFuture<Task> {
        MoveOnlyFunction<void(pb::SensorInfoEvent&&)> move_only_user_callback{
            std::function<void(pb::SensorInfoEvent&&)>{
                user_callback}};  // Copy `user_callback` into a `MoveOnlyFunction`.
        return InvokeUserCallbackWithinEventLoop(std::move(move_only_user_callback),
                                                 std::move(event));
      });
  return CreateSubscription<pb::NotificationServiceClient>(
      service_map_.notification, std::move(listener), pb::DefaultSubscribeRequest{});
}

Sdk::Future<sdk::HealthStatus> Sdk::GetHealthStatus(sdk::GetHealthStatusRequest&& request) {
  // clang tidy triggers an error because request is trivially copyable
  static_cast<void>(
      sdk::GetHealthStatusRequest{std::move(request)});  // NOLINT (hicpp-move-const-arg)
  return CreateFuture(ConnectedWebSocket(service_map_.project_manager.host,
                                         service_map_.project_manager.port, CreateClientHandler()) |
                      Then([](std::shared_ptr<RpcEndpoint>&& endpoint)
                               -> AnyFuture<sdk::pb::GetHealthStatusResponse> {
                        constexpr std::chrono::milliseconds kTimeout{500};
                        sdk::pb::ProjectManagerServiceClient client{endpoint};
                        return client.GetHealthStatus(sdk::pb::GetHealthStatusRequest{},
                                                      RetryClientDefault().WithTimeout(kTimeout)) |
                               Attach(std::move(endpoint));
                      }) |
                      // clang-tidy wants us to move rpc_response, which would have no effect,
                      // since it is trivially copyable
                      // NOLINTNEXTLINE(*-not-moved)
                      Map([](sdk::pb::GetHealthStatusResponse&& rpc_response) -> sdk::HealthStatus {
                        return sdk::HealthStatus{rpc_response};
                      }));
}

Sdk::Future<sdk::Version> Sdk::GetVersion(sdk::GetVersionRequest&& request) {
  // clang tidy triggers an error because request is trivially copyable
  static_cast<void>(sdk::GetVersionRequest{std::move(request)});  // NOLINT (hicpp-move-const-arg)
  return CreateFuture(
      ConnectedWebSocket(service_map_.project_manager.host, service_map_.project_manager.port,
                         CreateClientHandler()) |
      Then([](std::shared_ptr<RpcEndpoint>&& endpoint) -> AnyFuture<pb::GetVersionResponse> {
        constexpr std::chrono::milliseconds kTimeout{500};
        sdk::pb::StatusServiceClient client{endpoint};
        return client.GetVersion(
                   sdk::pb::GetVersionRequest{},
                   RetryClientDefault().WithTimeout(std::chrono::milliseconds{kTimeout})) |
               Attach(std::move(endpoint));
      }) |
      // clang-tidy wants us to move rpc_response, which would have no effect,
      // since it is trivially copyable
      // NOLINTNEXTLINE(*-not-moved)
      Map([](sdk::pb::GetVersionResponse&& rpc_response) -> sdk::Version {
        return sdk::Version{rpc_response};
      }));
}

// static
void Sdk::RunEventLoop(ChannelReceiver<Task>&& task_receiver) noexcept {
  horus_internal::EventLoop event_loop;
  for (;;) {
    try {
      auto event_loop_future = Loop([tasks{std::move(task_receiver)}]() mutable -> auto {
        return tasks.Receive() | Then([](Task&& task_rvalue) -> AnyFuture<LoopResult<void>> {
                 Task task{std::move(task_rvalue)};
                 if (task == nullptr) {
                   // Empty task received? Shutting down.
                   return ResolveWith(LoopResult<void>{InPlaceType<void>});
                 }
                 return task() | MapTo(LoopResult<void>{InPlaceType<LoopContinue>}) |
                        Catch([](const std::exception& exn) -> LoopResult<void> {
                          Log("error handling SDK task: ", exn.what());
                          return LoopResult<void>{InPlaceType<LoopContinue>};
                        });
               });
      });
      event_loop.RunFuture(std::move(event_loop_future));
      return;
    } catch (const std::exception& e) {
      Log("event loop encountered error, restarting: ", e.what());
    }
  }
}

Sdk::Subscription::Subscription(Sdk& sdk, std::shared_ptr<RpcEndpoint>&& subscriber,
                                Subscribe&& subscribe) noexcept(false)
    : state_{std::make_shared<State>(sdk, std::move(subscriber), std::move(subscribe))} {}

Sdk::Subscription::State::State(Sdk& sdk, std::shared_ptr<RpcEndpoint>&& subscriber,
                                Subscribe&& subscribe) noexcept
    : sdk_{sdk}, subscriber_{std::move(subscriber)}, subscribe_{std::move(subscribe)} {
  assert(subscriber_ != nullptr);

  // This code is executing immediately after creating the WebSocket and lifecycle events are
  // dispatched on the event loop so we don't need to worry about possible race conditions, where
  // `ConnectedEvent` would have been received before calling `SetLifecycleEventCallback()`.
  subscriber_->SetLifecycleEventCallback<State, &State::HandleEvent>(*this);
}

void Sdk::Subscription::State::Shutdown() noexcept {
  try {
    sdk_.task_channel_.GetSender().SendOrWait([state{shared_from_this()}]() -> AnyFuture<void> {
      state->subscriber_->ClearLifecycleEventCallback();
      return ResolvedFuture<void>{};
    });
  } catch (const ChannelClosedError&) {  // NOLINT(*-empty-catch)
    // SDK is already being destroyed.
  }
}

void Sdk::Subscription::State::HandleEvent(RpcEndpoint::LifecycleEvent&& event) noexcept {
  std::shared_ptr<State> shared{nullptr};
  try {
    shared = shared_from_this();
  } catch (const std::bad_weak_ptr&) {
    static_cast<void>(RpcEndpoint::LifecycleEvent{std::move(event)});
    return;
  }
  switch (event.Tag()) {
    case RpcEndpoint::LifecycleEvent::kTagFor<RpcEndpoint::ConnectedEvent>: {
      status_.store(Status::kConnected, std::memory_order_relaxed);
      try {
        sdk_.task_channel_.GetSender().SendOrWait([state{std::move(shared)}]() -> AnyFuture<void> {
          return Map(
              state->subscribe_(state->subscriber_), [weak_state{std::weak_ptr<State>{state}}]() {
                const std::shared_ptr<State> locked_state{weak_state.lock()};
                if (locked_state != nullptr) {
                  locked_state->status_.store(Status::kSubscribed, std::memory_order_relaxed);
                }
              });
        });
      } catch (const ChannelClosedError&) {
        // This isn't a big deal, but it's not supposed to happen so we only do something about it
        // when assertions are enabled.
        assert(false);
      }
      break;
    }
    case RpcEndpoint::LifecycleEvent::kTagFor<RpcEndpoint::DisconnectedEvent>:
    case RpcEndpoint::LifecycleEvent::kTagFor<RpcEndpoint::ErrorEvent>: {
      // We always reconnect.
      status_.store(Status::kConnecting, std::memory_order_relaxed);
      break;
    }
    case RpcEndpoint::LifecycleEvent::kTagFor<RpcEndpoint::ShutdownEvent>:
      break;
    default: {
      assert(false);
      break;
    }
  }
  static_cast<void>(RpcEndpoint::LifecycleEvent{std::move(event)});
}

}  // namespace horus
