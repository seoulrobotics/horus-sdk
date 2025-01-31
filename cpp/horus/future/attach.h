/// @file
///
/// The `Attach()` function.

#ifndef HORUS_FUTURE_ATTACH_H_
#define HORUS_FUTURE_ATTACH_H_

#include <utility>

#include "horus/future/future.h"
#include "horus/future/poll.h"

namespace horus {

// MARK: Attach()

/// Future returned by `Attach()`.
template <class F, class S>
class AttachFuture final : public Future<FutureResult<F>> {
 public:
  /// Constructs the future.
  explicit AttachFuture(F&& future, S&& state) noexcept
      : future_{std::move(future)}, state_{std::move(state)} {}

  /// Polls the future until the inner future completes.
  PollResult<FutureResult<F>> UnsafePoll(PollContext& context) final {
    return AttachFuture::PollFuture(future_, context);
  }

 private:
  /// The underlying future.
  F future_;
  /// The state attached to the future.
  S state_;
};

/// Returns a future which wraps `future` and stores `state`, ensuring that it lives as long as
/// `future`.
template <class F, class S>
inline AttachFuture<F, S> Attach(F&& future, S state) noexcept {
  return AttachFuture<F, S>{std::forward<F>(future), std::move(state)};
}

}  // namespace horus

#endif  // HORUS_FUTURE_ATTACH_H_
