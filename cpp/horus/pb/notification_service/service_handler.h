#ifndef HORUS_PB_NOTIFICATION_SERVICE_SERVICE_HANDLER_H_
#define HORUS_PB_NOTIFICATION_SERVICE_SERVICE_HANDLER_H_

#include <cstdint>

#include "horus/future/any.h"
#include "horus/internal/attributes.h"
#include "horus/pb/notification_service/service_client.h"
#include "horus/pb/notification_service/service_pb.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/pb/rpc_pb.h"
#include "horus/rpc/base_handler.h"
#include "horus/rpc/endpoint.h"
#include "horus/rpc/internal/subscriber_set.h"
#include "horus/strings/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

namespace horus {
namespace sdk {
namespace pb {

/// Service which receives notifications from services, storing and forwarding them.
class NotificationServiceHandler : public horus_internal::RpcBaseHandler {
 public:
  /// The request type of `LogMessage()`.
  using LogMessageRequestType = pb::LogMessageRequest;
  /// The response type of `LogMessage()`.
  using LogMessageResponseType = void;
  /// The request type of `Subscribe()`.
  using SubscribeRequestType = pb::DefaultSubscribeRequest;
  /// The response type of `Subscribe()`.
  using SubscribeResponseType = pb::DefaultSubscribeResponse;
  /// The request type of `Unsubscribe()`.
  using UnsubscribeRequestType = pb::DefaultUnsubscribeRequest;
  /// The response type of `Unsubscribe()`.
  using UnsubscribeResponseType = pb::DefaultUnsubscribeResponse;

  /// Default constructor.
  NotificationServiceHandler() noexcept = default;

  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 2; }
  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  StringView ServiceFullName() const noexcept final { return "horus.pb.NotificationService"; }

  /// @copydoc horus_internal::RpcBaseHandler::Handle()
  AnyFuture<pb::RpcMessage> Handle(const RpcContext& context,
                                   pb::RpcMessage&& request) noexcept(false) final;

 protected:

  /// Logs a message, storing it and broadcasting it to subscribers.
  virtual AnyFuture<void> LogMessage(const RpcContext& context,
                                     pb::LogMessageRequest&& request) noexcept(false) = 0;

  /// Subscribes to new log and profiling events.
  virtual AnyFuture<pb::DefaultSubscribeResponse> Subscribe(const RpcContext& context,
                                                            pb::DefaultSubscribeRequest&& request) noexcept(false);

  /// Unsubscribe following a call to `Subscribe()`.
  virtual AnyFuture<pb::DefaultUnsubscribeResponse> Unsubscribe(const RpcContext& context,
                                                                pb::DefaultUnsubscribeRequest&& request) noexcept(false);

  /// Returns a reference to the `SubscriberSet` of `NotificationListenerService`s.
  constexpr horus_internal::SubscriberSet<NotificationListenerServiceClient>& NotificationListenerServiceSubscribers() noexcept {
    return notification_listener_service_subscribers_;
  }
  /// Returns a reference to the `SubscriberSet` of `NotificationListenerService`s.
  constexpr const horus_internal::SubscriberSet<NotificationListenerServiceClient>& NotificationListenerServiceSubscribers() const noexcept {
    return notification_listener_service_subscribers_;
  }

 private:
  /// `NotificationListenerService` subscribers.
  horus_internal::SubscriberSet<NotificationListenerServiceClient> notification_listener_service_subscribers_;
};

/// A `NotificationServiceHandler` which forwards received messages to function objects.
template <class SharedState, class OnLogMessage, class OnSubscribe, class OnUnsubscribe>
class FunctionalNotificationService final : public NotificationServiceHandler {
 public:
  /// Constructs a `FunctionalNotificationService` which forwards received messages to function objects.
  FunctionalNotificationService(SharedState shared_state, OnLogMessage&& on_log_message, OnSubscribe&& on_subscribe, OnUnsubscribe&& on_unsubscribe) noexcept
      : shared_state_{std::forward<SharedState>(shared_state)}
      , on_log_message_{std::move(on_log_message)}
      , on_subscribe_{std::move(on_subscribe)}
      , on_unsubscribe_{std::move(on_unsubscribe)} {}

  /// Returns a `FunctionalNotificationService` which handles `LogMessage()` with `on_log_message()`.
  template <class F>
  FunctionalNotificationService<SharedState, F, OnSubscribe, OnUnsubscribe>
  LogMessageWith(F&& on_log_message) && noexcept {
    return FunctionalNotificationService<SharedState, F, OnSubscribe, OnUnsubscribe>{
        std::move(shared_state_), std::forward<F>(on_log_message), std::move(on_subscribe_), std::move(on_unsubscribe_)};
  }

  /// Returns a `FunctionalNotificationService` which handles `Subscribe()` with `on_subscribe()`.
  template <class F>
  FunctionalNotificationService<SharedState, OnLogMessage, F, OnUnsubscribe>
  SubscribeWith(F&& on_subscribe) && noexcept {
    return FunctionalNotificationService<SharedState, OnLogMessage, F, OnUnsubscribe>{
        std::move(shared_state_), std::move(on_log_message_), std::forward<F>(on_subscribe), std::move(on_unsubscribe_)};
  }

  /// Returns a `FunctionalNotificationService` which handles `Unsubscribe()` with `on_unsubscribe()`.
  template <class F>
  FunctionalNotificationService<SharedState, OnLogMessage, OnSubscribe, F>
  UnsubscribeWith(F&& on_unsubscribe) && noexcept {
    return FunctionalNotificationService<SharedState, OnLogMessage, OnSubscribe, F>{
        std::move(shared_state_), std::move(on_log_message_), std::move(on_subscribe_), std::forward<F>(on_unsubscribe)};
  }

 protected:
  /// Forwards `LogMessage()` to `OnLogMessage`.
  AnyFuture<void> LogMessage(const RpcContext& context,
                             pb::LogMessageRequest&& request) noexcept(false) final {
    return horus_internal::ForwardToFunctionalHandler<void>(shared_state_, on_log_message_, context, std::move(request));
  }
  /// Forwards `Subscribe()` to `OnSubscribe`.
  AnyFuture<pb::DefaultSubscribeResponse> Subscribe(const RpcContext& context,
                                                    pb::DefaultSubscribeRequest&& request) noexcept(false) final {
    return horus_internal::ForwardToFunctionalHandler<pb::DefaultSubscribeResponse>(shared_state_, on_subscribe_, context, std::move(request));
  }
  /// Forwards `Unsubscribe()` to `OnUnsubscribe`.
  AnyFuture<pb::DefaultUnsubscribeResponse> Unsubscribe(const RpcContext& context,
                                                        pb::DefaultUnsubscribeRequest&& request) noexcept(false) final {
    return horus_internal::ForwardToFunctionalHandler<pb::DefaultUnsubscribeResponse>(shared_state_, on_unsubscribe_, context, std::move(request));
  }
 private:
  /// State shared between all callbacks.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS SharedState shared_state_;
  /// Handler of `LogMessage`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnLogMessage on_log_message_;
  /// Handler of `Subscribe`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnSubscribe on_subscribe_;
  /// Handler of `Unsubscribe`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnUnsubscribe on_unsubscribe_;
};

/// Returns a `FunctionalNotificationService` with the given shared state.
template <class SharedState>
FunctionalNotificationService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::LogMessageRequest, void>), decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>
CreateFunctionalNotificationService(SharedState&& shared_state) noexcept {
  return FunctionalNotificationService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::LogMessageRequest, void>), decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>{
      std::forward<SharedState>(shared_state), &horus_internal::IgnoreRpc<pb::LogMessageRequest, void>, &horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>, &horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>};
}

/// Returns a `FunctionalNotificationService` with no shared state.
inline FunctionalNotificationService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::LogMessageRequest, void>), decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>
CreateFunctionalNotificationService() noexcept {
  return FunctionalNotificationService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::LogMessageRequest, void>), decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>{
      nullptr, &horus_internal::IgnoreRpc<pb::LogMessageRequest, void>, &horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>, &horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>};
}

/// Service which listens for notifications forwarded by the `NotificationService`.
class NotificationListenerServiceHandler : public horus_internal::RpcBaseHandler {
 public:
  /// The request type of `NotifyLogMessage()`.
  using NotifyLogMessageRequestType = pb::LogMessageEvent;
  /// The response type of `NotifyLogMessage()`.
  using NotifyLogMessageResponseType = void;
  /// The request type of `NotifyProfilingInfo()`.
  using NotifyProfilingInfoRequestType = pb::ProfilingInfoEvent;
  /// The response type of `NotifyProfilingInfo()`.
  using NotifyProfilingInfoResponseType = void;
  /// The request type of `NotifySensorInfo()`.
  using NotifySensorInfoRequestType = pb::SensorInfoEvent;
  /// The response type of `NotifySensorInfo()`.
  using NotifySensorInfoResponseType = void;

  /// Default constructor.
  NotificationListenerServiceHandler() noexcept = default;

  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 10; }
  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  StringView ServiceFullName() const noexcept final { return "horus.pb.NotificationListenerService"; }

  /// @copydoc horus_internal::RpcBaseHandler::Handle()
  AnyFuture<pb::RpcMessage> Handle(const RpcContext& context,
                                   pb::RpcMessage&& request) noexcept(false) final;

 protected:

  /// Received when a log message is received by the `NotificationService`.
  virtual AnyFuture<void> NotifyLogMessage(const RpcContext& context,
                                           pb::LogMessageEvent&& request) noexcept(false) = 0;

  /// Received when a profiling info is received by the `NotificationService`.
  virtual AnyFuture<void> NotifyProfilingInfo(const RpcContext& context,
                                              pb::ProfilingInfoEvent&& request) noexcept(false) = 0;

  /// Received when a sensor info is received by the `NotificationService`.
  virtual AnyFuture<void> NotifySensorInfo(const RpcContext& context,
                                           pb::SensorInfoEvent&& request) noexcept(false) = 0;
};

/// A `NotificationListenerServiceHandler` which forwards received messages to function objects.
template <class SharedState, class OnNotifyLogMessage, class OnNotifyProfilingInfo, class OnNotifySensorInfo>
class FunctionalNotificationListenerService final : public NotificationListenerServiceHandler {
 public:
  /// Constructs a `FunctionalNotificationListenerService` which forwards received messages to function objects.
  FunctionalNotificationListenerService(SharedState shared_state, OnNotifyLogMessage&& on_notify_log_message, OnNotifyProfilingInfo&& on_notify_profiling_info, OnNotifySensorInfo&& on_notify_sensor_info) noexcept
      : shared_state_{std::forward<SharedState>(shared_state)}
      , on_notify_log_message_{std::move(on_notify_log_message)}
      , on_notify_profiling_info_{std::move(on_notify_profiling_info)}
      , on_notify_sensor_info_{std::move(on_notify_sensor_info)} {}

  /// Returns a `FunctionalNotificationListenerService` which handles `NotifyLogMessage()` with `on_notify_log_message()`.
  template <class F>
  FunctionalNotificationListenerService<SharedState, F, OnNotifyProfilingInfo, OnNotifySensorInfo>
  NotifyLogMessageWith(F&& on_notify_log_message) && noexcept {
    return FunctionalNotificationListenerService<SharedState, F, OnNotifyProfilingInfo, OnNotifySensorInfo>{
        std::move(shared_state_), std::forward<F>(on_notify_log_message), std::move(on_notify_profiling_info_), std::move(on_notify_sensor_info_)};
  }

  /// Returns a `FunctionalNotificationListenerService` which handles `NotifyProfilingInfo()` with `on_notify_profiling_info()`.
  template <class F>
  FunctionalNotificationListenerService<SharedState, OnNotifyLogMessage, F, OnNotifySensorInfo>
  NotifyProfilingInfoWith(F&& on_notify_profiling_info) && noexcept {
    return FunctionalNotificationListenerService<SharedState, OnNotifyLogMessage, F, OnNotifySensorInfo>{
        std::move(shared_state_), std::move(on_notify_log_message_), std::forward<F>(on_notify_profiling_info), std::move(on_notify_sensor_info_)};
  }

  /// Returns a `FunctionalNotificationListenerService` which handles `NotifySensorInfo()` with `on_notify_sensor_info()`.
  template <class F>
  FunctionalNotificationListenerService<SharedState, OnNotifyLogMessage, OnNotifyProfilingInfo, F>
  NotifySensorInfoWith(F&& on_notify_sensor_info) && noexcept {
    return FunctionalNotificationListenerService<SharedState, OnNotifyLogMessage, OnNotifyProfilingInfo, F>{
        std::move(shared_state_), std::move(on_notify_log_message_), std::move(on_notify_profiling_info_), std::forward<F>(on_notify_sensor_info)};
  }

 protected:
  /// Forwards `NotifyLogMessage()` to `OnNotifyLogMessage`.
  AnyFuture<void> NotifyLogMessage(const RpcContext& context,
                                   pb::LogMessageEvent&& request) noexcept(false) final {
    return horus_internal::ForwardToFunctionalHandler<void>(shared_state_, on_notify_log_message_, context, std::move(request));
  }
  /// Forwards `NotifyProfilingInfo()` to `OnNotifyProfilingInfo`.
  AnyFuture<void> NotifyProfilingInfo(const RpcContext& context,
                                      pb::ProfilingInfoEvent&& request) noexcept(false) final {
    return horus_internal::ForwardToFunctionalHandler<void>(shared_state_, on_notify_profiling_info_, context, std::move(request));
  }
  /// Forwards `NotifySensorInfo()` to `OnNotifySensorInfo`.
  AnyFuture<void> NotifySensorInfo(const RpcContext& context,
                                   pb::SensorInfoEvent&& request) noexcept(false) final {
    return horus_internal::ForwardToFunctionalHandler<void>(shared_state_, on_notify_sensor_info_, context, std::move(request));
  }
 private:
  /// State shared between all callbacks.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS SharedState shared_state_;
  /// Handler of `NotifyLogMessage`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnNotifyLogMessage on_notify_log_message_;
  /// Handler of `NotifyProfilingInfo`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnNotifyProfilingInfo on_notify_profiling_info_;
  /// Handler of `NotifySensorInfo`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnNotifySensorInfo on_notify_sensor_info_;
};

/// Returns a `FunctionalNotificationListenerService` with the given shared state.
template <class SharedState>
FunctionalNotificationListenerService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::LogMessageEvent, void>), decltype(&horus_internal::IgnoreRpc<pb::ProfilingInfoEvent, void>), decltype(&horus_internal::IgnoreRpc<pb::SensorInfoEvent, void>)>
CreateFunctionalNotificationListenerService(SharedState&& shared_state) noexcept {
  return FunctionalNotificationListenerService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::LogMessageEvent, void>), decltype(&horus_internal::IgnoreRpc<pb::ProfilingInfoEvent, void>), decltype(&horus_internal::IgnoreRpc<pb::SensorInfoEvent, void>)>{
      std::forward<SharedState>(shared_state), &horus_internal::IgnoreRpc<pb::LogMessageEvent, void>, &horus_internal::IgnoreRpc<pb::ProfilingInfoEvent, void>, &horus_internal::IgnoreRpc<pb::SensorInfoEvent, void>};
}

/// Returns a `FunctionalNotificationListenerService` with no shared state.
inline FunctionalNotificationListenerService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::LogMessageEvent, void>), decltype(&horus_internal::IgnoreRpc<pb::ProfilingInfoEvent, void>), decltype(&horus_internal::IgnoreRpc<pb::SensorInfoEvent, void>)>
CreateFunctionalNotificationListenerService() noexcept {
  return FunctionalNotificationListenerService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::LogMessageEvent, void>), decltype(&horus_internal::IgnoreRpc<pb::ProfilingInfoEvent, void>), decltype(&horus_internal::IgnoreRpc<pb::SensorInfoEvent, void>)>{
      nullptr, &horus_internal::IgnoreRpc<pb::LogMessageEvent, void>, &horus_internal::IgnoreRpc<pb::ProfilingInfoEvent, void>, &horus_internal::IgnoreRpc<pb::SensorInfoEvent, void>};
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus

#endif  // HORUS_PB_NOTIFICATION_SERVICE_SERVICE_HANDLER_H_
