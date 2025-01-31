#ifndef HORUS_PB_DETECTION_SERVICE_DETECTION_SERVICE_HANDLER_H_
#define HORUS_PB_DETECTION_SERVICE_DETECTION_SERVICE_HANDLER_H_

#include <cstdint>

#include "horus/future/any.h"
#include "horus/internal/attributes.h"
#include "horus/pb/detection_service/detection_pb.h"
#include "horus/pb/detection_service/detection_service_client.h"
#include "horus/pb/detection_service/detection_service_pb.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/pb/rpc_pb.h"
#include "horus/rpc/base_handler.h"
#include "horus/rpc/endpoint.h"
#include "horus/rpc/internal/subscriber_set.h"
#include "horus/types/string_view.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

namespace horus {
namespace sdk {
namespace pb {

/// Service for receiving pre-processed foreground points and sending detection results.
class DetectionServiceHandler : public horus_internal::RpcBaseHandler {
 public:
  /// The request type of `Subscribe()`.
  using SubscribeRequestType = pb::DefaultSubscribeRequest;
  /// The response type of `Subscribe()`.
  using SubscribeResponseType = pb::DefaultSubscribeResponse;
  /// The request type of `Unsubscribe()`.
  using UnsubscribeRequestType = pb::DefaultUnsubscribeRequest;
  /// The response type of `Unsubscribe()`.
  using UnsubscribeResponseType = pb::DefaultUnsubscribeResponse;

  /// Default constructor.
  DetectionServiceHandler() noexcept = default;

  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 7; }
  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  StringView ServiceFullName() const noexcept final { return "horus.pb.DetectionService"; }

  /// @copydoc horus_internal::RpcBaseHandler::Handle()
  AnyFuture<pb::RpcMessage> Handle(const RpcContext& context,
                                   pb::RpcMessage&& request) noexcept(false) final;

 protected:

  /// Subscribes to receive future detection results as they are made available.
  virtual AnyFuture<pb::DefaultSubscribeResponse> Subscribe(const RpcContext& context,
                                                            pb::DefaultSubscribeRequest&& request) noexcept(false);

  /// Unsubscribe following a call to `Subscribe()`.
  virtual AnyFuture<pb::DefaultUnsubscribeResponse> Unsubscribe(const RpcContext& context,
                                                                pb::DefaultUnsubscribeRequest&& request) noexcept(false);

  /// Returns a reference to the `SubscriberSet` of `DetectionSubscriberService`s.
  constexpr horus_internal::SubscriberSet<DetectionSubscriberServiceClient>& DetectionSubscriberServiceSubscribers() noexcept {
    return detection_subscriber_service_subscribers_;
  }
  /// Returns a reference to the `SubscriberSet` of `DetectionSubscriberService`s.
  constexpr const horus_internal::SubscriberSet<DetectionSubscriberServiceClient>& DetectionSubscriberServiceSubscribers() const noexcept {
    return detection_subscriber_service_subscribers_;
  }

 private:
  /// `DetectionSubscriberService` subscribers.
  horus_internal::SubscriberSet<DetectionSubscriberServiceClient> detection_subscriber_service_subscribers_;
};

/// A `DetectionServiceHandler` which forwards received messages to function objects.
template <class SharedState, class OnSubscribe, class OnUnsubscribe>
class FunctionalDetectionService final : public DetectionServiceHandler {
 public:
  /// Constructs a `FunctionalDetectionService` which forwards received messages to function objects.
  FunctionalDetectionService(SharedState shared_state, OnSubscribe&& on_subscribe, OnUnsubscribe&& on_unsubscribe) noexcept
      : shared_state_{std::forward<SharedState>(shared_state)}
      , on_subscribe_{std::move(on_subscribe)}
      , on_unsubscribe_{std::move(on_unsubscribe)} {}

  /// Returns a `FunctionalDetectionService` which handles `Subscribe()` with `on_subscribe()`.
  template <class F>
  FunctionalDetectionService<SharedState, F, OnUnsubscribe>
  SubscribeWith(F&& on_subscribe) && noexcept {
    return FunctionalDetectionService<SharedState, F, OnUnsubscribe>{
        std::move(shared_state_), std::forward<F>(on_subscribe), std::move(on_unsubscribe_)};
  }

  /// Returns a `FunctionalDetectionService` which handles `Unsubscribe()` with `on_unsubscribe()`.
  template <class F>
  FunctionalDetectionService<SharedState, OnSubscribe, F>
  UnsubscribeWith(F&& on_unsubscribe) && noexcept {
    return FunctionalDetectionService<SharedState, OnSubscribe, F>{
        std::move(shared_state_), std::move(on_subscribe_), std::forward<F>(on_unsubscribe)};
  }

 protected:
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
  /// Handler of `Subscribe`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnSubscribe on_subscribe_;
  /// Handler of `Unsubscribe`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnUnsubscribe on_unsubscribe_;
};

/// Returns a `FunctionalDetectionService` with the given shared state.
template <class SharedState>
FunctionalDetectionService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>
CreateFunctionalDetectionService(SharedState&& shared_state) noexcept {
  return FunctionalDetectionService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>{
      std::forward<SharedState>(shared_state), &horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>, &horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>};
}

/// Returns a `FunctionalDetectionService` with no shared state.
inline FunctionalDetectionService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>
CreateFunctionalDetectionService() noexcept {
  return FunctionalDetectionService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>{
      nullptr, &horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>, &horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>};
}

/// Service exposed by clients of the `DetectionService` that wish to receive updates when new detection results are processed.
class DetectionSubscriberServiceHandler : public horus_internal::RpcBaseHandler {
 public:
  /// The request type of `BroadcastDetection()`.
  using BroadcastDetectionRequestType = pb::DetectionEvent;
  /// The response type of `BroadcastDetection()`.
  using BroadcastDetectionResponseType = void;

  /// Default constructor.
  DetectionSubscriberServiceHandler() noexcept = default;

  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 12; }
  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  StringView ServiceFullName() const noexcept final { return "horus.pb.DetectionSubscriberService"; }

  /// @copydoc horus_internal::RpcBaseHandler::Handle()
  AnyFuture<pb::RpcMessage> Handle(const RpcContext& context,
                                   pb::RpcMessage&& request) noexcept(false) final;

 protected:

  /// Notify new detection results.
  virtual AnyFuture<void> BroadcastDetection(const RpcContext& context,
                                             pb::DetectionEvent&& request) noexcept(false) = 0;
};

/// A `DetectionSubscriberServiceHandler` which forwards received messages to function objects.
template <class SharedState, class OnBroadcastDetection>
class FunctionalDetectionSubscriberService final : public DetectionSubscriberServiceHandler {
 public:
  /// Constructs a `FunctionalDetectionSubscriberService` which forwards received messages to function objects.
  FunctionalDetectionSubscriberService(SharedState shared_state, OnBroadcastDetection&& on_broadcast_detection) noexcept
      : shared_state_{std::forward<SharedState>(shared_state)}
      , on_broadcast_detection_{std::move(on_broadcast_detection)} {}

  /// Returns a `FunctionalDetectionSubscriberService` which handles `BroadcastDetection()` with `on_broadcast_detection()`.
  template <class F>
  FunctionalDetectionSubscriberService<SharedState, F>
  BroadcastDetectionWith(F&& on_broadcast_detection) && noexcept {
    return FunctionalDetectionSubscriberService<SharedState, F>{
        std::move(shared_state_), std::forward<F>(on_broadcast_detection)};
  }

 protected:
  /// Forwards `BroadcastDetection()` to `OnBroadcastDetection`.
  AnyFuture<void> BroadcastDetection(const RpcContext& context,
                                     pb::DetectionEvent&& request) noexcept(false) final {
    return horus_internal::ForwardToFunctionalHandler<void>(shared_state_, on_broadcast_detection_, context, std::move(request));
  }
 private:
  /// State shared between all callbacks.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS SharedState shared_state_;
  /// Handler of `BroadcastDetection`.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS OnBroadcastDetection on_broadcast_detection_;
};

/// Returns a `FunctionalDetectionSubscriberService` with the given shared state.
template <class SharedState>
FunctionalDetectionSubscriberService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::DetectionEvent, void>)>
CreateFunctionalDetectionSubscriberService(SharedState&& shared_state) noexcept {
  return FunctionalDetectionSubscriberService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::DetectionEvent, void>)>{
      std::forward<SharedState>(shared_state), &horus_internal::IgnoreRpc<pb::DetectionEvent, void>};
}

/// Returns a `FunctionalDetectionSubscriberService` with no shared state.
inline FunctionalDetectionSubscriberService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::DetectionEvent, void>)>
CreateFunctionalDetectionSubscriberService() noexcept {
  return FunctionalDetectionSubscriberService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::DetectionEvent, void>)>{
      nullptr, &horus_internal::IgnoreRpc<pb::DetectionEvent, void>};
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus

#endif  // HORUS_PB_DETECTION_SERVICE_DETECTION_SERVICE_HANDLER_H_
