#ifndef HORUS_PB_DETECTION_MERGER_SERVICE_HANDLER_H_
#define HORUS_PB_DETECTION_MERGER_SERVICE_HANDLER_H_

#include <cstdint>

#include "horus/future/any.h"
#include "horus/internal/attributes.h"
#include "horus/pb/detection_merger/service_client.h"
#include "horus/pb/detection_merger/service_pb.h"
#include "horus/pb/detection_service/detection_pb.h"
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

/// Service for merging detection results from multiple sources.
class DetectionMergerServiceHandler : public horus_internal::RpcBaseHandler {
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
  DetectionMergerServiceHandler() noexcept = default;

  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 11; }
  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  StringView ServiceFullName() const noexcept final { return "horus.pb.DetectionMergerService"; }

  /// @copydoc horus_internal::RpcBaseHandler::Handle()
  AnyFuture<pb::RpcMessage> Handle(const RpcContext& context,
                                   pb::RpcMessage&& request) noexcept(false) final;

 protected:

  /// Subscribe to receive merged detection results.
  virtual AnyFuture<pb::DefaultSubscribeResponse> Subscribe(const RpcContext& context,
                                                            pb::DefaultSubscribeRequest&& request) noexcept(false);

  /// Unsubscribe from merged detection results.
  virtual AnyFuture<pb::DefaultUnsubscribeResponse> Unsubscribe(const RpcContext& context,
                                                                pb::DefaultUnsubscribeRequest&& request) noexcept(false);

  /// Returns a reference to the `SubscriberSet` of `DetectionMergerSubscriberService`s.
  constexpr horus_internal::SubscriberSet<DetectionMergerSubscriberServiceClient>& DetectionMergerSubscriberServiceSubscribers() noexcept {
    return detection_merger_subscriber_service_subscribers_;
  }
  /// Returns a reference to the `SubscriberSet` of `DetectionMergerSubscriberService`s.
  constexpr const horus_internal::SubscriberSet<DetectionMergerSubscriberServiceClient>& DetectionMergerSubscriberServiceSubscribers() const noexcept {
    return detection_merger_subscriber_service_subscribers_;
  }

 private:
  /// `DetectionMergerSubscriberService` subscribers.
  horus_internal::SubscriberSet<DetectionMergerSubscriberServiceClient> detection_merger_subscriber_service_subscribers_;
};

/// A `DetectionMergerServiceHandler` which forwards received messages to function objects.
template <class SharedState, class OnSubscribe, class OnUnsubscribe>
class FunctionalDetectionMergerService final : public DetectionMergerServiceHandler {
 public:
  /// Constructs a `FunctionalDetectionMergerService` which forwards received messages to function objects.
  FunctionalDetectionMergerService(SharedState shared_state, OnSubscribe&& on_subscribe, OnUnsubscribe&& on_unsubscribe) noexcept
      : shared_state_{std::forward<SharedState>(shared_state)}
      , on_subscribe_{std::move(on_subscribe)}
      , on_unsubscribe_{std::move(on_unsubscribe)} {}

  /// Returns a `FunctionalDetectionMergerService` which handles `Subscribe()` with `on_subscribe()`.
  template <class F>
  FunctionalDetectionMergerService<SharedState, F, OnUnsubscribe>
  SubscribeWith(F&& on_subscribe) && noexcept {
    return FunctionalDetectionMergerService<SharedState, F, OnUnsubscribe>{
        std::move(shared_state_), std::forward<F>(on_subscribe), std::move(on_unsubscribe_)};
  }

  /// Returns a `FunctionalDetectionMergerService` which handles `Unsubscribe()` with `on_unsubscribe()`.
  template <class F>
  FunctionalDetectionMergerService<SharedState, OnSubscribe, F>
  UnsubscribeWith(F&& on_unsubscribe) && noexcept {
    return FunctionalDetectionMergerService<SharedState, OnSubscribe, F>{
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

/// Returns a `FunctionalDetectionMergerService` with the given shared state.
template <class SharedState>
FunctionalDetectionMergerService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>
CreateFunctionalDetectionMergerService(SharedState&& shared_state) noexcept {
  return FunctionalDetectionMergerService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>{
      std::forward<SharedState>(shared_state), &horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>, &horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>};
}

/// Returns a `FunctionalDetectionMergerService` with no shared state.
inline FunctionalDetectionMergerService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>
CreateFunctionalDetectionMergerService() noexcept {
  return FunctionalDetectionMergerService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>), decltype(&horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>)>{
      nullptr, &horus_internal::IgnoreRpc<pb::DefaultSubscribeRequest, pb::DefaultSubscribeResponse>, &horus_internal::IgnoreRpc<pb::DefaultUnsubscribeRequest, pb::DefaultUnsubscribeResponse>};
}

/// Service for receiving merged detection results.
class DetectionMergerSubscriberServiceHandler : public horus_internal::RpcBaseHandler {
 public:
  /// The request type of `BroadcastDetection()`.
  using BroadcastDetectionRequestType = pb::DetectionEvent;
  /// The response type of `BroadcastDetection()`.
  using BroadcastDetectionResponseType = void;

  /// Default constructor.
  DetectionMergerSubscriberServiceHandler() noexcept = default;

  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  std::uint16_t ServiceId() const noexcept final { return 16; }
  /// @copydoc horus_internal::RpcBaseHandler::ServiceId()
  StringView ServiceFullName() const noexcept final { return "horus.pb.DetectionMergerSubscriberService"; }

  /// @copydoc horus_internal::RpcBaseHandler::Handle()
  AnyFuture<pb::RpcMessage> Handle(const RpcContext& context,
                                   pb::RpcMessage&& request) noexcept(false) final;

 protected:

  /// Receive merged detection results.
  virtual AnyFuture<void> BroadcastDetection(const RpcContext& context,
                                             pb::DetectionEvent&& request) noexcept(false) = 0;
};

/// A `DetectionMergerSubscriberServiceHandler` which forwards received messages to function objects.
template <class SharedState, class OnBroadcastDetection>
class FunctionalDetectionMergerSubscriberService final : public DetectionMergerSubscriberServiceHandler {
 public:
  /// Constructs a `FunctionalDetectionMergerSubscriberService` which forwards received messages to function objects.
  FunctionalDetectionMergerSubscriberService(SharedState shared_state, OnBroadcastDetection&& on_broadcast_detection) noexcept
      : shared_state_{std::forward<SharedState>(shared_state)}
      , on_broadcast_detection_{std::move(on_broadcast_detection)} {}

  /// Returns a `FunctionalDetectionMergerSubscriberService` which handles `BroadcastDetection()` with `on_broadcast_detection()`.
  template <class F>
  FunctionalDetectionMergerSubscriberService<SharedState, F>
  BroadcastDetectionWith(F&& on_broadcast_detection) && noexcept {
    return FunctionalDetectionMergerSubscriberService<SharedState, F>{
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

/// Returns a `FunctionalDetectionMergerSubscriberService` with the given shared state.
template <class SharedState>
FunctionalDetectionMergerSubscriberService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::DetectionEvent, void>)>
CreateFunctionalDetectionMergerSubscriberService(SharedState&& shared_state) noexcept {
  return FunctionalDetectionMergerSubscriberService<SharedState, decltype(&horus_internal::IgnoreRpc<pb::DetectionEvent, void>)>{
      std::forward<SharedState>(shared_state), &horus_internal::IgnoreRpc<pb::DetectionEvent, void>};
}

/// Returns a `FunctionalDetectionMergerSubscriberService` with no shared state.
inline FunctionalDetectionMergerSubscriberService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::DetectionEvent, void>)>
CreateFunctionalDetectionMergerSubscriberService() noexcept {
  return FunctionalDetectionMergerSubscriberService<decltype(nullptr), decltype(&horus_internal::IgnoreRpc<pb::DetectionEvent, void>)>{
      nullptr, &horus_internal::IgnoreRpc<pb::DetectionEvent, void>};
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus

#endif  // HORUS_PB_DETECTION_MERGER_SERVICE_HANDLER_H_
