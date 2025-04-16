/// @file
///
/// The `Union()` function.

#ifndef HORUS_FUTURE_ONE_OF_H_
#define HORUS_FUTURE_ONE_OF_H_

#include <type_traits>
#include <utility>

#include "horus/future/future.h"
#include "horus/types/one_of.h"

namespace horus {

// MARK: OneFutureOf()

/// Future returned by `OneFutureOf()`.
template <class... Futures>
class OneFutureOfFuture final : public Future<std::common_type_t<FutureResult<Futures>...>> {
 public:
  /// @copydoc Future::Result
  using typename OneFutureOfFuture::Future::Result;

  /// Constructs a `OneOfFuture`.
  explicit OneFutureOfFuture(OneOf<Futures...>&& future) noexcept : future_{std::move(future)} {}

  /// Polls the future until the inner future completes.
  PollResult<Result> UnsafePoll(PollContext& context) final {
    // We don't trace the future as it trivially executes its inner future.
    return future_.Visit([&context](auto& future) -> PollResult<Result> {
      return OneFutureOfFuture::PollFuture(future, context);
    });
  }

 private:
  /// The underlying future to poll.
  OneOf<Futures...> future_;
};

/// Returns a future that completes when the future set in the `OneOf` `future` completes.
template <class... Futures, std::common_type_t<FutureResult<Futures>...>* = nullptr>
inline OneFutureOfFuture<Futures...> OneFutureOf(OneOf<Futures...>&& future) noexcept {
  return OneFutureOfFuture<Futures...>{std::move(future)};
}

}  // namespace horus

#endif  // HORUS_FUTURE_ONE_OF_H_
