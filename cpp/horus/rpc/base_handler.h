/// @file
///
/// The `RpcBaseHandler` class.

#ifndef HORUS_RPC_BASE_HANDLER_H_
#define HORUS_RPC_BASE_HANDLER_H_

#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "horus/future/any.h"
#include "horus/future/future.h"
#include "horus/future/map.h"
#include "horus/future/resolved.h"
#include "horus/internal/type_traits.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/rpc/message_pb.h"
#include "horus/pb/serialize.h"
#include "horus/rpc/endpoint.h"
#include "horus/types/string_view.h"

namespace horus {
namespace horus_internal {

/// An error thrown by RPC handlers when asked to handle an `RpcMessage` whose `MethodId()` is
/// unknown.
class UnknownRpcMethodError final : public std::runtime_error {
 public:
  /// Constructs the error with a default error message.
  UnknownRpcMethodError() : std::runtime_error{"unknown rpc method"} {}
};

/// Base class for generated RPC handlers.
class RpcBaseHandler {
 public:
  /// Default constructor.
  RpcBaseHandler() noexcept = default;

  /// Non-copyable.
  RpcBaseHandler(const RpcBaseHandler&) = delete;
  /// Non-copyable.
  RpcBaseHandler& operator=(const RpcBaseHandler&) = delete;

  /// Virtual destructor for inheritance.
  virtual ~RpcBaseHandler() = default;

  /// The internal identifier of the service used to route RPC messages to this class.
  virtual std::uint16_t ServiceId() const noexcept = 0;

  /// The full name of the service, e.g. "horus.pb.FooService".
  virtual StringView ServiceFullName() const noexcept = 0;

  /// The short name of the service, e.g. "FooService".
  StringView ServiceName() const noexcept;

  /// Handles an RPC `request`.
  ///
  /// @throws std::exception Any exception encountered while handling the request.
  /// @throws UnknownRpcMethodError If the `request` specifies an unknown method.
  virtual AnyFuture<sdk::pb::RpcMessage> Handle(const RpcContext& context,
                                                sdk::pb::RpcMessage&& request) noexcept(false) = 0;

 protected:
  /// Movable.
  RpcBaseHandler(RpcBaseHandler&&) = default;
  /// Movable.
  RpcBaseHandler& operator=(RpcBaseHandler&&) = default;

  /// Calls `handle` with the request stored in `request_message`.
  template <class Self, class Request>
  AnyFuture<sdk::pb::RpcMessage> DoDispatch(
      AnyFuture<void> (Self::*handle)(const RpcContext&, Request&&), const RpcContext& context,
      const sdk::pb::RpcMessage& request_message) noexcept(false) {
    PbReader reader{request_message.message_bytes().View()};
    return Map((static_cast<Self*>(this)->*handle)(context, Request{reader}),
               []() noexcept -> sdk::pb::RpcMessage { return {}; });
  }

  /// Calls `handle` with the request stored in `request_message` and returns a response message.
  template <class Self, class Request, class Response>
  AnyFuture<sdk::pb::RpcMessage> DoDispatch(
      AnyFuture<Response> (Self::*handle)(const RpcContext&, Request&&), const RpcContext& context,
      const sdk::pb::RpcMessage& request_message) noexcept(false) {
    PbReader reader{request_message.message_bytes().View()};
    sdk::pb::RpcMessage response_message{sdk::pb::RpcMessage{}
                                             .set_version(sdk::pb::RpcMessage::Version::kOne)
                                             .set_request_id(request_message.request_id())};
    return Map((static_cast<Self*>(this)->*handle)(context, Request{reader}),
               [inner_response_message{std::move(response_message)}](
                   const Response& response) mutable -> sdk::pb::RpcMessage {
                 inner_response_message.set_message_bytes(CowBytes{response.SerializeToBuffer()});
                 return std::move(inner_response_message);
               });
  }
};

/// Invokes `invocable(args...)`, returning its result as a `AnyFuture<R>`.
template <class R, class F, class... Args,
          std::enable_if_t<std::is_void<InvokeResultT<F, Args...>>::value>* = nullptr>
inline AnyFuture<R> InvokeToFuture(F&& invocable, Args&&... args) {
  std::forward<F>(invocable)(std::forward<Args>(args)...);
  return ResolvedFuture<void>{};
}
template <class R, class F, class... Args,
          std::enable_if_t<!std::is_void<InvokeResultT<F, Args...>>::value &&
                           !IsFuture<InvokeResultT<F, Args...>>::value>* = nullptr>
inline AnyFuture<R> InvokeToFuture(F&& invocable, Args&&... args) {
  return ResolveWith(std::forward<F>(invocable)(std::forward<Args>(args)...));
}
template <class R, class F, class... Args,
          std::enable_if_t<IsFuture<InvokeResultT<F, Args...>>::value>* = nullptr>
inline AnyFuture<R> InvokeToFuture(F&& invocable, Args&&... args) {
  return std::forward<F>(invocable)(std::forward<Args>(args)...);
}

/// Forwards a method call of a functional `RpcBaseHandler` to a function object `handler`.
template <class Response, class Request, class SharedState, class Handler,
          std::enable_if_t<
              IsInvocable<Handler&, SharedState&, const RpcContext&, Request&&>::value>* = nullptr>
inline AnyFuture<Response> ForwardToFunctionalHandler(SharedState& state, Handler& handler,
                                                      const RpcContext& context,
                                                      Request&& request) {
  return InvokeToFuture<Response>(handler, state, context, std::forward<Request>(request));
}
template <class Response, class Request, class SharedState, class Handler,
          std::enable_if_t<IsInvocable<Handler&, SharedState&, Request&&>::value>* = nullptr>
inline AnyFuture<Response> ForwardToFunctionalHandler(SharedState& state, Handler& handler,
                                                      const RpcContext& context,
                                                      Request&& request) {
  static_cast<void>(context);
  return InvokeToFuture<Response>(handler, state, std::forward<Request>(request));
}
template <class Response, class Request, class SharedState, class Handler,
          std::enable_if_t<IsInvocable<Handler&, const RpcContext&, Request&&>::value>* = nullptr>
inline AnyFuture<Response> ForwardToFunctionalHandler(SharedState& state, Handler& handler,
                                                      const RpcContext& context,
                                                      Request&& request) {
  static_cast<void>(state);
  return InvokeToFuture<Response>(handler, context, std::forward<Request>(request));
}
template <class Response, class Request, class SharedState, class Handler,
          std::enable_if_t<IsInvocable<Handler&, Request&&>::value>* = nullptr>
inline AnyFuture<Response> ForwardToFunctionalHandler(SharedState& state, Handler& handler,
                                                      const RpcContext& context,
                                                      Request&& request) {
  static_cast<void>(state);
  static_cast<void>(context);
  return InvokeToFuture<Response>(handler, std::forward<Request>(request));
}

/// Defines `Return()`, which returns the default value of `T`. Specialized for `void` to return
/// nothing.
template <class T>
class ReturnDefault final {
 public:
  static constexpr T Return() noexcept(std::is_nothrow_default_constructible<T>::value) {
    return {};
  }
};
template <>
class ReturnDefault<void> final {
 public:
  static constexpr void Return() noexcept {}
};

/// Ignores the given value.
///
/// Used to pass callbacks to `CreateFunctional___Service()` when such callbacks are irrelevant to a
/// subscriber.
template <class Request, class Response>
constexpr Response IgnoreRpc(const horus_internal::TypeIdentityT<Request>& request) noexcept {
  static_cast<void>(request);
  return horus_internal::ReturnDefault<Response>::Return();
}

}  // namespace horus_internal
}  // namespace horus

#endif  // HORUS_RPC_BASE_HANDLER_H_
