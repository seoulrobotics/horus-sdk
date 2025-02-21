/// @file
///
/// The `Attach()` function.

#ifndef HORUS_FUTURE_ATTACH_H_
#define HORUS_FUTURE_ATTACH_H_

#include <utility>

#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/future/operator.h"
#include "horus/future/poll.h"

namespace horus {

// MARK: Attach()

/// Future returned by `Attach()`.
template <class F, class S>
class AttachFuture final : public Future<FutureResult<F>> {
 public:
  /// Constructs the future.
  explicit AttachFuture(F&& future, S&& state,
                        const FutureInfo& info = CurrentFutureInfo()) noexcept
      : future_{std::move(future)}, state_{std::move(state)}, info_{info} {}

  /// Polls the future until the inner future completes.
  PollResult<FutureResult<F>> UnsafePoll(PollContext& context) final {
    return AttachFuture::PollFuture(future_, context);
  }

 private:
  /// The underlying future.
  F future_;
  /// The state attached to the future.
  S state_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a future which wraps `future` and stores `state`, ensuring that it lives as long as
/// `future`.
template <class F, class S>
inline AttachFuture<F, S> Attach(F&& future, S state) noexcept {
  return AttachFuture<F, S>{std::forward<F>(future), std::move(state)};
}

/// `Pipe()` operator which keeps some state `S` alive as long as the future it is attached to.
template <class S>
constexpr FutureOperator<AttachFuture, void, S> Attach(
    S state, FutureInfo future_info = UnknownFutureInfo()) noexcept {
  return FutureOperator<AttachFuture, void, S>{std::forward<S>(state), future_info};
}

}  // namespace horus

#endif  // HORUS_FUTURE_ATTACH_H_
