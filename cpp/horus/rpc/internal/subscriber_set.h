/// @file
///
/// The `SubscriberSet` class.

#ifndef HORUS_RPC_INTERNAL_SUBSCRIBER_SET_H_
#define HORUS_RPC_INTERNAL_SUBSCRIBER_SET_H_

#include <chrono>
#include <cstddef>
#include <exception>
#include <utility>
#include <vector>

#include "horus/future/any.h"
#include "horus/future/cancel.h"
#include "horus/future/completed.h"
#include "horus/future/join.h"
#include "horus/future/try.h"
#include "horus/internal/vector.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/logs/logs_pb.h"
#include "horus/rpc/endpoint.h"
#include "horus/strings/logging.h"
#include "horus/types/in_place.h"
#include "horus/types/one_of.h"
#include "horus/types/owned.h"
#include "horus/types/span.h"
#include "horus/types/string_view.h"

namespace horus {
namespace horus_internal {

/// A set of RPC subscribers.
template <class SubscriberClient>
class SubscriberSet final {
 public:
  /// Constructs an empty set.
  SubscriberSet() noexcept = default;

  /// Invokes `invocable()` with a `Span<const SubscriberClient>`.
  ///
  /// @warning Handling subscriptions in such a way is no trivial endeavour; subscriptions should
  /// ideally execute in parallel, not block indefinitely, and errors should be checked. Prefer
  /// `NotifySubscribers()` over this function if possible.
  template <class F>
  auto WithSubscribers(const F& invocable) const noexcept(false)
      -> decltype(invocable(std::declval<Span<const SubscriberClient>>())) {
    if (subscribers_.template Is<void>()) {
      return invocable(Span<const SubscriberClient>{});
    }
    return invocable(Span<const SubscriberClient>{*subscribers_.template As<0>()});
  }

  /// Calls `invocable(subscriber)` for each `SubscriberClient` in the set, then awaits completion
  /// of all returned futures concurrently. Times out after the given duration (4s by default),
  /// emitting warnings.
  template <class F>
  AnyFuture<void> NotifySubscribers(const F& invocable,
                                    std::chrono::milliseconds timeout = std::chrono::seconds{
                                        4}) noexcept(false) {
    if (subscribers_.template Is<void>()) {
      return CompletedFuture<void>{};
    }
    OwnedSubscribers& owned_subscribers{subscribers_.template As<0>()};
    std::vector<AnyFuture<void>> tasks;
    tasks.reserve(owned_subscribers->size());
    for (const SubscriberClient& subscriber : owned_subscribers.Get()) {
      AnyFuture<void> task{
          invocable(subscriber) | WithTimeout(timeout) |
          Catch(
              [subscribers{owned_subscribers.Borrow()},
               endpoint{subscriber.Endpoint()}](const RpcEndpointDisconnectedError& /* error */) {
                // Remove disconnected subscriber.
                std::size_t const subscriber_index{FindSubscriber(*subscribers, *endpoint)};
                if (subscriber_index != subscribers->size()) {
                  SwapRemove(*subscribers, subscriber_index);
                }
              },
              [subscriber](const std::exception& exception) {
                Log("failed to notify subscriber ", subscriber.ServiceName(), " at ",
                    subscriber.Endpoint()->Uri(), ": ", exception.what());
              })};
      tasks.push_back(std::move(task));
    }
    return Join(std::move(tasks));
  }

  /// Adds the endpoint at the origin of an RPC request to the set.
  template <class Response>
  Response Add(const RpcContext& context);

  /// Removes the endpoint at the origin of an RPC request from the set.
  template <class Response>
  Response Remove(const RpcContext& context);

 private:
  /// An `Owned` vector of `SubscriberClient`s.
  using OwnedSubscribers = Owned<std::vector<SubscriberClient>, OwnedMode::kAutomaticOneThread>;

  /// Returns the index of the subscriber originating from the given endpoint or
  /// `subscribers_.size()` if no such subscriber exists.
  static std::size_t FindSubscriber(const std::vector<SubscriberClient>& subscribers,
                                    const RpcEndpoint& endpoint) noexcept;

  /// The vector of active subscribers. We don't need synchronization here as we assume all accesses
  /// will be made from the same thread. Because `Owned` may allocate, this field is initialized
  /// lazily on first mutation.
  OneOf<OwnedSubscribers, void> subscribers_{InPlaceType<void>};
};

// MARK: Function definitions

template <class SubscriberClient>
template <class Response>
Response SubscriberSet<SubscriberClient>::Add(const RpcContext& context) {
  if (subscribers_.template Is<void>()) {
    // Do not emplace directly since the `Owned` constructor can throw and `Emplace()`
    // requires a noexcept constructor.
    OwnedSubscribers owned_subscribers;
    static_cast<void>(subscribers_.template Emplace<0>(std::move(owned_subscribers)));
  }
  std::vector<SubscriberClient>& subscribers{*subscribers_.template As<0>()};
  if (FindSubscriber(subscribers, *context.Endpoint()) == subscribers.size()) {
    subscribers.emplace_back(context.Endpoint());
  }
  return {};
}

/// Returns a `pb::logs::RpcDisconnectionError` indicating that a specified subscriber could not be
/// found.
inline pb::logs::RpcDisconnectionError CreateMissingSubscriberResponse(
    const RpcContext& context, StringView service_name) noexcept(false) {
  return pb::logs::RpcDisconnectionError{}
      .set_target_service(CowBytes::Borrowed(service_name))
      .set_target_uri(CowBytes::OwnedCopy(context.Endpoint()->Uri()))
      .set_details(CowBytes::Borrowed("could not find corresponding subscriber"));
}

template <class SubscriberClient>
template <class Response>
Response SubscriberSet<SubscriberClient>::Remove(const RpcContext& context) {
  if (subscribers_.template Is<void>()) {
    return Response{}.set_disconnection_error(
        CreateMissingSubscriberResponse(context, SubscriberClient{nullptr}.ServiceName()));
  }
  std::vector<SubscriberClient>& subscribers{*subscribers_.template As<0>()};
  std::size_t const subscriber_index{FindSubscriber(subscribers, *context.Endpoint())};
  if (subscriber_index == subscribers.size()) {
    return Response{}.set_disconnection_error(
        CreateMissingSubscriberResponse(context, SubscriberClient{nullptr}.ServiceName()));
  }
  static_cast<void>(SwapRemove(subscribers, subscriber_index));
  return {};
}

template <class SubscriberClient>
// static
std::size_t SubscriberSet<SubscriberClient>::FindSubscriber(
    const std::vector<SubscriberClient>& subscribers, const RpcEndpoint& endpoint) noexcept {
  for (std::size_t i{0}; i < subscribers.size(); ++i) {
    if (subscribers[i].Endpoint().get() == &endpoint) {
      return i;
    }
  }
  return subscribers.size();
}

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_RPC_INTERNAL_SUBSCRIBER_SET_H_
