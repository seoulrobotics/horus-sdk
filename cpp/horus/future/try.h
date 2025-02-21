/// @file
///
/// The `Try()` function.

#ifndef HORUS_FUTURE_TRY_H_
#define HORUS_FUTURE_TRY_H_

#include <cstddef>
#include <exception>
#include <tuple>
#include <type_traits>
#include <utility>

#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/future/operator.h"
#include "horus/future/poll.h"
#include "horus/internal/type_traits.h"
#include "horus/internal/void.h"
#include "horus/types/scope_guard.h"

namespace horus {

// MARK: Try()

/// Future returned by `Try()`.
template <class F, class... Handlers>
class TryFuture final : public Future<FutureResult<F>> {
 public:
  /// @copydoc Future::Result
  using typename TryFuture::Future::Result;

  /// Constructs a `TryFuture`.
  explicit TryFuture(F&& future, Handlers&&... handlers,
                     const FutureInfo& info = CurrentFutureInfo()) noexcept
      : future_{std::move(future)}, handlers_{std::move(handlers)...}, info_{info} {}

  /// Polls the future until its inner future completes, handling potential errors.
  PollResult<Result> UnsafePoll(PollContext& context) final;

 private:
  /// The inner future to poll.
  F future_;
  /// The handlers to apply to the future.
  std::tuple<Handlers...> handlers_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a future that attempts to drive `future` to completion, catching exceptions using the
/// given `handlers`.
///
/// - If a handler which takes no argument is given, it will be executed no matter
///   whether an exception was caught or not.
/// - Otherwise, it will be executed if an exception with a matching type is thrown.
///   In that case, and especially if you want to throw an error in this handler, make sure to
///   explicitely specify the handler's return type to the original return type of the `Try()`
///   future.
///
/// Example:
///
///   Try(ResolveWith<std::uint8_t>(0), [](std::runtime_error const& error) -> std::uint8_t {
///      throw MyCustomException(error.what());
///   }, []() {
///      Log("Execution finished");
///   });
template <class F, class... Handlers>
inline TryFuture<F, Handlers...> Try(F&& future, Handlers&&... handlers) noexcept {
  return TryFuture<F, Handlers...>{std::forward<F>(future), std::forward<Handlers>(handlers)...,
                                   UnknownFutureInfo()};
}

/// `Pipe()` operator which handles exceptions thrown by a future.
///
/// This can be considered as the piped version of `Try()` future.
/// Please also make sure to explicitely define the return type of your handlers, especially if you
/// want to throw an exception.
///
/// Example:
///
///   ResolveWith<std::uint8_t>(0) | Catch([](std::runtime_error const& error) -> std::uint8_t {
///      throw MyCustomException(error.what());
///   }, []() {
///      Log("Execution finished");
///   });
template <class... Handlers>
inline FutureOperator<TryFuture, void, Handlers...> Catch(Handlers&&... handlers) noexcept {
  return FutureOperator<TryFuture, void, Handlers...>{std::forward<Handlers>(handlers)...,
                                                      UnknownFutureInfo()};
}

// MARK: Function definitions

namespace horus_internal {

/// Detector used in `IsFinallyHandler`.
template <class Handler>
using IsFinallyHandlerDetector = decltype(std::declval<Handler&>()());

/// Defines `value` to be true if `Handler` is a "finally" handler (i.e. it takes no arguments).
template <class Handler>
using IsFinallyHandler = IsDetected<IsFinallyHandlerDetector, Handler>;

/// Detector used in `IsCatchAllHandler`.
template <class Handler>
using IsCatchAllHandlerDetector = decltype(std::declval<Handler&>()(std::exception_ptr{}));

/// Defines `value` to be true if `Handler` is a "catch-all" handler (i.e. it takes a
/// `std::exception_ptr`).
template <class Handler>
using IsCatchAllHandler = IsDetected<IsCatchAllHandlerDetector, Handler>;

/// Defines `type` to be the type of the handled exception.
template <class F>
struct HandledType : HandledType<decltype(&F::operator())> {};
template <class R, class E>
struct HandledType<R(E)> : std::decay<E> {};
template <class R, class E>
struct HandledType<R (*)(E)> : std::decay<E> {};
template <class R, class This, class E>
struct HandledType<R (This::*)(E)> : std::decay<E> {};
template <class R, class This, class E>
struct HandledType<R (This::*)(E) const> : std::decay<E> {};

/// Evaluates to the type of the handled exception.
template <class F>
using HandledTypeT = typename HandledType<F>::type;

/// Base recursive case of `TryPoll()`.
template <class Future>
inline PollResult<FutureResult<Future>> TryPoll(Future& future, PollContext& context) {
  return future.UnsafePoll(context);
}

/// Returns `future.UnsafePoll(context)` while handling all exceptions specified by the given
/// `handler, rest...`.
template <class Future, class Handler, class... Rest,
          std::enable_if_t<!IsFinallyHandler<Handler>::value &&
                           !IsCatchAllHandler<Handler>::value>* = nullptr>
inline PollResult<FutureResult<Future>> TryPoll(Future& future, PollContext& context,
                                                Handler& handler, Rest&... rest) {
  static_assert(std::is_base_of<std::exception, HandledTypeT<Handler>>::value, "");
  try {
    return TryPoll(future, context, rest...);
  } catch (const HandledTypeT<Handler>& e) {
    return SwitchVoid<FutureResult<Future>>(
        [&e, &handler](const auto& forward) -> PollResult<FutureResult<Future>> {
          handler(forward(e));
          return ReadyResult<FutureResult<Future>>();
        },
        [&e, &handler](const auto& forward) -> PollResult<FutureResult<Future>> {
          return ReadyResult<FutureResult<Future>>(handler(forward(e)));
        });
  }
}

/// `TryPoll()` with the handler being a "catch-all" handler.
template <class Future, class Handler, class... Rest,
          std::enable_if_t<IsCatchAllHandler<Handler>::value>* = nullptr>
inline PollResult<FutureResult<Future>> TryPoll(Future& future, PollContext& context,
                                                Handler& handler, Rest&... rest) {
  try {
    return TryPoll(future, context, rest...);
  } catch (...) {
    return SwitchVoid<FutureResult<Future>>(
        [&handler](const auto& forward) -> PollResult<FutureResult<Future>> {
          handler(forward(std::current_exception()));
          return ReadyResult<FutureResult<Future>>();
        },
        [&handler](const auto& forward) -> PollResult<FutureResult<Future>> {
          return ReadyResult<FutureResult<Future>>(handler(forward(std::current_exception())));
        });
  }
}

/// `TryPoll()` with the handler being a "finally" handler.
template <class Future, class Handler, class... Rest,
          std::enable_if_t<IsFinallyHandler<Handler>::value>* = nullptr>
inline PollResult<FutureResult<Future>> TryPoll(Future& future, PollContext& context,
                                                Handler& handler, Rest&... rest) {
  auto finally_handler = Defer([&handler]() noexcept { handler(); });
  PollResult<FutureResult<Future>> result{TryPoll(future, context, rest...)};
  if (result.template Is<Pending>()) {
    finally_handler.Disengage();
  }
  return result;
}

/// Calls `TryPoll(handlers...)` with `handlers` in reverse order.
template <class Future, class... Handlers, std::size_t... Indices>
inline PollResult<FutureResult<Future>> TryPollTuple(
    Future& future, PollContext& context, std::tuple<Handlers...>& handlers,
    std::index_sequence<Indices...> /* indices */) {
  return TryPoll(future, context, std::get<sizeof...(Indices) - Indices - 1>(handlers)...);
}

}  // namespace horus_internal

template <class F, class... Handlers>
auto TryFuture<F, Handlers...>::UnsafePoll(PollContext& context) -> PollResult<Result> {
  const PollContext::Trace trace{context, "Try", info_};
  return horus_internal::TryPollTuple(future_, context, handlers_,
                                      std::make_index_sequence<sizeof...(Handlers)>{});
}

}  // namespace horus

#endif  // HORUS_FUTURE_TRY_H_
