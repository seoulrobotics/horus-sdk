/// @file
///
/// The `FromPoll()` function.

#ifndef HORUS_FUTURE_FROM_POLL_H_
#define HORUS_FUTURE_FROM_POLL_H_

#include <type_traits>
#include <utility>

#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/future/poll.h"

namespace horus {
namespace horus_internal {

/// See `PollFunctionResult`.
template <class T>
struct PollFunctionResultImpl;
template <class T>
struct PollFunctionResultImpl<PollResult<T>> {
  using Type = T;
};
/// Yields `U` given `T = PollResult<U>`.
template <class T>
using PollFunctionResult = typename PollFunctionResultImpl<T>::Type;

}  // namespace horus_internal

// MARK: FromPoll()

/// Future returned by `FromPoll()`.
template <class F>
class FromPollFuture final
    : public Future<horus_internal::PollFunctionResult<decltype(std::declval<F&>()(
          std::declval<PollContext&>()))>> {
  static_assert(std::is_nothrow_move_constructible<F>::value, "");

 public:
  /// @copydoc Future::Result
  using typename FromPollFuture::Future::Result;

  /// Constructs a future which dispatches `UnsafePoll()` calls to `invocable()`.
  explicit FromPollFuture(F&& invocable, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : invocable_{std::move(invocable)}, info_{info} {}

  /// Polls the future using the provided function `F`.
  PollResult<Result> UnsafePoll(PollContext& context) final {
    const PollContext::Trace trace{context, "FromPoll", info_};
    return invocable_(context);
  }

 private:
  /// The function which implements `UnsafePoll()`.
  F invocable_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a future whose `UnsafePoll()` function is dispatched to `poll_invocable()`.
template <class F>
inline FromPollFuture<F> FromPoll(F&& poll_invocable,
                                  const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return FromPollFuture<F>{std::forward<F>(poll_invocable), info};
}

}  // namespace horus

#endif  // HORUS_FUTURE_FROM_POLL_H_
