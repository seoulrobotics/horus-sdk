/// @file
///
/// The `Sdk` class.

#ifndef HORUS_SDK_H_
#define HORUS_SDK_H_

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <exception>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <utility>

#include "horus/functional/move_only_function.h"
#include "horus/future/any.h"
#include "horus/future/channel.h"
#include "horus/future/future.h"
#include "horus/future/map_to.h"
#include "horus/future/work.h"
#include "horus/internal/attributes.h"
#include "horus/rpc/endpoint.h"
#include "horus/rpc/services.h"
#include "horus/sdk/logs.h"
#include "horus/sdk/objects.h"
#include "horus/sdk/point_clouds.h"
#include "horus/sdk/profiling.h"
#include "horus/sdk/sensor.h"
#include "horus/types/in_place.h"
#include "horus/types/one_of.h"
#include "horus/types/string_view.h"

namespace horus {

/// Entry-point of the Horus SDK, which allows communication to all services.
///
/// Progress and background errors will be reported with `Log()`, which by default prints logs on
/// `stderr`. This behavior can be modified by constructing a `ScopedLogger` (from
/// `horus/strings/logging.h`) to redirect calls in a scope.
class Sdk final {
 public:
  /// A map from service to URL where it can be resolved.
  struct ServiceResolutionMap {
    /// An entry in the map.
    struct Entry {
      /// The host name of the service.
      StringView host;
      /// The port at which the service listens.
      std::uint16_t port;

      /// Constructs an entry pointing to a `<given_host>:<given_port>` pair. `given_host` will
      /// *not* be copied.
      constexpr Entry(StringView given_host HORUS_SDK_ATTRIBUTE_LIFETIME_BOUND,
                      std::uint16_t given_port) noexcept
          : host{given_host}, port{given_port} {}

      /// Constructs an entry pointing to `127.0.0.1:<local_port>`.
      constexpr explicit Entry(std::uint16_t local_port) noexcept
          : Entry{"127.0.0.1", local_port} {}

      /// Constructs an entry pointing to the default configuration of a service.
      constexpr explicit Entry(const RpcServices::ServiceInfo& service_info) noexcept
          : Entry{service_info.default_ip, service_info.default_port} {}
    };

    /// How to resolve the notification service.
    Entry notification{RpcServices::NotificationService()};

    /// How to resolve the detection service.
    Entry detection{RpcServices::DetectionService()};

    /// How to resolve the point aggregator.
    Entry point_aggregator{RpcServices::PointAggregatorService()};
  };

  /// A future returned by the `Sdk`.
  ///
  /// Most operations in the SDK communicate with external services, and therefore execute
  /// asynchronously. `Future`s allow clients to either `Wait()` until the future completes,
  /// blocking the calling thread (but _not_ the SDK) until their completion, or to schedule for
  /// some function to be called when the future completes (or fails).
  template <class T>
  class Future;

  /// A subscription to some stream of events.
  class Subscription;

  /// Constructs the SDK.
  ///
  /// The SDK will spawn a thread to communicate with Horus services.
  ///
  /// @throws std::bad_alloc If the event-loop cannot be allocated.
  /// @throws std::system_error If the event-loop thread cannot be spawned.
  explicit Sdk(const ServiceResolutionMap& services) noexcept(false);

  /// Same as `Sdk()`, connecting to services hosted on localhost on their default ports.
  Sdk() noexcept(false) : Sdk{ServiceResolutionMap{}} {}

  /// The SDK cannot be copied.
  Sdk(const Sdk&) = delete;
  /// The SDK cannot be copied.
  Sdk& operator=(const Sdk&) = delete;

  /// Move-constructs the SDK.
  Sdk(Sdk&&) noexcept = default;
  /// Move-assigns the SDK.
  Sdk& operator=(Sdk&&) noexcept = default;

  /// Destroys the SDK, waiting until the event loop is done.
  ~Sdk() noexcept;

  /// Subscribes to receive user logs sent by Horus services.
  Future<Subscription> SubscribeToLogs(sdk::LogSubscriptionRequest&& request);

  /// Subscribes to receive profiling information.
  Future<Subscription> SubscribeToProfiling(sdk::ProfilingSubscriptionRequest&& request);

  /// Subscribes to receive object information.
  Future<Subscription> SubscribeToObjects(sdk::ObjectSubscriptionRequest&& request);

  /// Subscribes to receive occupancy grid information.
  Future<Subscription> SubscribeToOccupancyGrid(sdk::OccupancyGridSubscriptionRequest&& request);

  /// Subscribes to receive point clouds.
  Future<Subscription> SubscribeToPointClouds(sdk::PointCloudSubscriptionRequest&& request);

  /// Subscribes to receive sensor information.
  Future<Subscription> SubscribeToSensorInfo(sdk::SensorInfoSubscriptionRequest&& request);

 private:
  /// A task to perform on the event loop.
  using Task = MoveOnlyFunction<AnyFuture<void>()>;

  /// State held by a `Future` and shared with the `Sdk`.
  template <class T>
  struct FutureState;

  /// Schedules for the user `callback` to run on the thread pool with the given argument.
  template <class Arg>
  void InvokeUserCallback(MoveOnlyFunction<void(Arg)>&& callback, std::decay_t<Arg> arg) {
    task_channel_.GetSender().SendOrWait(
        [callback1{std::move(callback)}, arg1{std::move(arg)}]() mutable -> auto {
          return MapToVoid(DeferWork([callback2{std::move(callback1)}, arg2{std::move(arg1)}](
                                         std::atomic<bool>& cancelled) mutable -> bool {
            if (!cancelled.load()) {
              callback2(std::move(arg2));
            }
            return true;  // Because `DeferWork()` requires a result.
          }));
        });
  }

  /// Schedules for the user `callback` to run on the thread pool with the given argument.
  template <class Arg>
  ChannelSendFuture<Task> InvokeUserCallbackWithinEventLoop(MoveOnlyFunction<void(Arg)>&& callback,
                                                            std::decay_t<Arg> arg) {
    return task_channel_.GetSender().Send(
        [callback1{std::move(callback)}, arg1{std::move(arg)}]() mutable -> auto {
          return MapToVoid(DeferWork([callback2{std::move(callback1)}, arg2{std::move(arg1)}](
                                         std::atomic<bool>& cancelled) mutable -> bool {
            if (!cancelled.load()) {
              callback2(std::move(arg2));
            }
            return true;  // Because `DeferWork()` requires a result.
          }));
        });
  }

  /// Constructs and returns a `Future<T>` which completes when `F` does.
  ///
  /// @throws std::bad_alloc If the `Future` cannot be allocated.
  template <class F>
  Future<FutureResult<F>> CreateFuture(F&& future) noexcept(false);

  /// Constructs and returns a `Future` which completes with a `Subscription` to `target`.
  /// `Subscriber` must be an RPC subscriber service handler, and `subscribe()` will be called on
  /// connection to subscribe to the `target`.
  ///
  /// @throws std::bad_alloc If the `Future` or `Subscription` cannot be allocated.
  template <class Client, class Subscriber, class SubscriptionRequest>
  Future<Subscription> CreateSubscription(const ServiceResolutionMap::Entry& target,
                                          Subscriber&& subscriber,
                                          SubscriptionRequest&& request) noexcept(false);

  /// Function executing in the `event_loop_thread_`.
  static void RunEventLoop(ChannelReceiver<Task>&& task_receiver) noexcept;

  /// The map used to resolve service hosts and ports.
  ServiceResolutionMap service_map_;
  /// A channel used to send tasks to perform on the event loop.
  Channel<Task> task_channel_;
  /// The thread which runs the event loop.
  std::thread event_loop_thread_;
};

template <class T>
struct Sdk::FutureState {
  /// Constructs the shared state.
  FutureState(Sdk& sdk_ref, AnyFuture<void>&& future) noexcept
      : sdk{sdk_ref}, value{InPlaceIndex<0>, std::move(future)} {}

  /// A reference to the SDK.
  Sdk& sdk;
  /// Mutex used to guard the shared state.
  std::mutex mutex;
  /// Condition variable notified on completion.
  std::condition_variable completion_cv;
  /// The resulting value; 0 if pending, 1 if resolved. Only set if `on_completion` is empty or set
  /// by `Wait()`. We never actually use `AnyFuture<void>`. It is only stored to make sure we only
  /// cancel it if the `State` itself is destroyed.
  OneOf<AnyFuture<void>, T, std::exception_ptr> value;
  /// A (possibly empty) function to call on completion. This should make sure to call user
  /// callbacks in a work thread.
  MoveOnlyFunction<void(FutureState&)> on_completion{nullptr};
};

template <class T>
class Sdk::Future final {
  /// @see FutureState
  using State = FutureState<T>;

 public:
  /// (Internal use only) Constructs the future.
  explicit Future(std::shared_ptr<State>&& state) noexcept : state_{std::move(state)} {}

  /// Waits until the future resolves, then returns its result. If the future encountered an
  /// exception, `Wait()` will rethrow it.
  ///
  /// `Wait()` does *not* busy-wait; calling `Wait()` will yield the current thread. `Wait()` also
  /// does *not* block the SDK. That is, given two `Future`s, calling `Wait()` on one will not block
  /// progress on the other one.
  T Wait();

  /// Waits until the future resolves, then returns its result. If the future does not resolve
  /// within `duration`, returns `void`. If the future encountered an exception, `WaitFor()` will
  /// rethrow it.
  OneOf<T, void> WaitFor(std::chrono::milliseconds duration) {
    return TimedWait(&std::condition_variable::wait_for, duration);
  }

  /// Waits until the future resolves, then returns its result. If the future does not resolve
  /// before `deadline`, returns `void`. If the future encountered an exception, `WaitUntil()` will
  /// rethrow it.
  template <class Clock>
  OneOf<T, void> WaitUntil(std::chrono::time_point<Clock> deadline) {
    return TimedWait(&std::condition_variable::wait_until, deadline);
  }

  /// Wires the future to call `on_completion()` on completion and `on_error()` on error. If it
  /// already completed, the relevant callback will be called immediately in the calling thread.
  /// Otherwise, it will be called on a work thread at some point in the future.
  ///
  /// If this function is called multiple times, the previous callbacks will be discarded and
  /// replaced with the new ones.
  ///
  /// If `Wait()` is called after this function, both functions will be called with an rvalue
  /// reference to the same object, and must ensure that they do not perform invalid operations on
  /// it. The order in which these functions are called is indeterminate.
  template <class F, class E>
  void OnCompletion(F&& on_completion, E&& on_error);

 private:
  /// Predicate given to `std::condition_variable::wait()`.
  struct Predicate {
    /// The (locked) state to operate on.
    State& state;

    /// Returns whether the wait can end.
    constexpr bool operator()() const noexcept { return !state.value.template Is<0>(); }
  };

  /// Implementation of `WaitFor()` and `WaitUntil()`.
  template <class Arg>
  OneOf<T, void> TimedWait(
      bool (std::condition_variable::*wait)(std::unique_lock<std::mutex>&, const Arg&,
                                            typename Sdk::Future<T>::Predicate),
      Arg arg);

  /// The (non-null) completion state shared with the real future.
  std::shared_ptr<State> state_;
};

/// @see Sdk::Future
template <class T>
using SdkFuture = Sdk::Future<T>;

template <class T>
T Sdk::Future<T>::Wait() {
  State& state{*state_};
  std::unique_lock<std::mutex> lock{state.mutex};
  if (state.value.template Is<0>()) {
    state.completion_cv.wait(lock, Predicate{state});
  }
  if (state.value.template Is<2>()) {
    std::rethrow_exception(state.value.template As<2>());
  }
  return std::move(state.value.template As<1>());
}

template <class T>
template <class Arg>
OneOf<T, void> Sdk::Future<T>::TimedWait(
    bool (std::condition_variable::*wait)(std::unique_lock<std::mutex>&, const Arg&,
                                          typename Sdk::Future<T>::Predicate),
    Arg arg) {
  State& state{*state_};
  std::unique_lock<std::mutex> lock{state.mutex};  // NOLINT(*-const-correctness): mutable ref below
  if (!(state.completion_cv.*wait)(lock, arg, Predicate{state})) {
    // Did not complete within deadline.
    return OneOf<T, void>{InPlaceIndex<1>};
  }
  if (state.value.template Is<2>()) {
    std::rethrow_exception(state.value.template As<2>());
  }
  return OneOf<T, void>{InPlaceIndex<0>, std::move(state.value.template As<1>())};
}

template <class T>
template <class F, class E>
void Sdk::Future<T>::OnCompletion(F&& on_completion, E&& on_error) {
  State& state{*state_};
  const std::unique_lock<std::mutex> lock{state.mutex};
  if (state.value.template Is<1>()) {
    // Already completed.
    std::forward<F>(on_completion)(std::move(state.value.template As<1>()));
  } else if (state.value.template Is<2>()) {
    // Already encountered error.
    std::forward<E>(on_error)(state.value.template As<2>());
  } else {
    state.on_completion = [complete{MoveOnlyFunction<void(T&&)>{std::forward<F>(on_completion)}},
                           reject{MoveOnlyFunction<void(const std::exception_ptr&)>{
                               std::forward<E>(on_error)}}](State& inner_state) mutable {
      if (inner_state.value.template Is<1>()) {
        inner_state.sdk.InvokeUserCallback(std::move(complete),
                                           std::move(inner_state.value.template As<1>()));
      } else {
        inner_state.sdk.InvokeUserCallback(std::move(reject), inner_state.value.template As<2>());
      }
    };
  }
}

/// A subscription to a stream of events made from the `Sdk`.
class Sdk::Subscription final {
  /// The type of the `subscribe` function.
  using Subscribe = MoveOnlyFunction<AnyFuture<void>(const std::shared_ptr<RpcEndpoint>&)>;

 public:
  /// The status of the subscription.
  enum class Status : std::uint8_t {
    /// Connecting to the service.
    kConnecting = 1,
    /// Connected to the service.
    kConnected = 2,
    /// Connected to the service and subscribed to future events.
    kSubscribed = 3,
  };

  /// (Internal use only) Constructs a subscription.
  explicit Subscription(Sdk& sdk, std::shared_ptr<RpcEndpoint>&& subscriber,
                        Subscribe&& subscribe) noexcept(false);

  /// Non-copyable.
  Subscription(const Subscription&) = delete;
  /// Non-copyable.
  Subscription& operator=(const Subscription&) = delete;
  /// Move constructor.
  Subscription(Subscription&&) noexcept = default;
  /// Move assignment operator.
  Subscription& operator=(Subscription&&) noexcept = default;

  /// Destroys the subscription, unsubscribing from future changes.
  ~Subscription() noexcept {
    if (state_ != nullptr) {
      // `state_` is null after a move.
      state_->Shutdown();
    }
  }

  /// The current status of the subscription. This may be slightly outdated since the status may
  /// be changed in another thread while this function is called (although the function itself is
  /// thread-safe).
  Status CurrentStatus() const noexcept { return state_->CurrentStatus(); }

 private:
  /// The state held by the subscription and stored in a `std::shared_ptr` (since it is immovable).
  class State final : public std::enable_shared_from_this<State> {
   public:
    /// Constructs the state.
    State(Sdk& sdk, std::shared_ptr<RpcEndpoint>&& subscriber, Subscribe&& subscribe) noexcept;

    /// Cannot copy/move.
    State(const State&) = delete;
    /// Cannot copy/move.
    State& operator=(const State&) = delete;
    /// Cannot copy/move.
    State(State&&) = delete;
    /// Cannot copy/move.
    State& operator=(State&&) = delete;

    /// Destructor.
    ~State() noexcept = default;

    /// Handles a subscription lifecycle event.
    void HandleEvent(RpcEndpoint::LifecycleEvent&& event) noexcept;

    /// @see Subscription::CurrentStatus()
    Status CurrentStatus() const noexcept { return status_.load(std::memory_order_relaxed); }

    /// Starts destroying the state. Called by `~Subscription()`.
    void Shutdown() noexcept;

   private:
    /// A reference to the SDK.
    Sdk& sdk_;

    /// The endpoint which subscribes to the event stream; when destroyed, this will shut down the
    /// connection and therefore unsubscribe.
    std::shared_ptr<RpcEndpoint> subscriber_;

    /// The function to call when we are connected and able to subscribe.
    Subscribe subscribe_;

    /// @see Subscription::CurrentStatus()
    std::atomic<Status> status_{Status::kConnecting};
  };

  /// The (non-null) state of the subscription. We use a `std::shared_ptr` since we pass a reference
  /// to the subscription to event handlers whose lifetimes isn't bound to the `Subscription`.
  std::shared_ptr<State> state_;
};

/// @see Sdk::Subscription
using SdkSubscription = Sdk::Subscription;

}  // namespace horus

#endif  // HORUS_SDK_H_
