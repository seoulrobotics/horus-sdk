/// @file
///
/// The `Then()` function.

#ifndef HORUS_FUTURE_THEN_H_
#define HORUS_FUTURE_THEN_H_

#include <utility>

#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/future/map.h"
#include "horus/future/poll.h"
#include "horus/types/one_of.h"

namespace horus {

// MARK: Then()

/// Future returned by `Then()`.
template <class F, class Map>
class ThenFuture final : public Future<FutureResult<horus_internal::MappedType<Map, F>>> {
 public:
  /// @copydoc Future::Result
  using typename ThenFuture::Future::Result;

  /// The future returned by invoking `Map`.
  using MappedFuture = horus_internal::MappedType<Map, F>;

  /// Constructs a `ThenFuture`.
  ThenFuture(F&& future, Map&& map, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : future_{std::move(future)}, then_{std::move(map)}, info_{info} {}

  /// Polls the future until the inner future completes, then returns its mapped result.
  PollResult<Result> UnsafePoll(PollContext& context) final;

 private:
  /// The future to execute first. This cannot be in a `OneOf` as futures cannot be (easily)
  /// destroyed.
  F future_;
  /// Either the mapping function (if `before_future_` hasn't completed yet) or the mapped future.
  OneOf<Map, MappedFuture> then_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a future which completes with the result of `future` after mapping it with `map()`.
template <class Future, class Map>
inline ThenFuture<Future, Map> Then(Future&& future, Map&& map,
                                    const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return ThenFuture<Future, Map>{std::forward<Future>(future), std::forward<Map>(map), info};
}

/// `Pipe()` operator which maps a `Future` to another `Future`.
template <class Map>
constexpr FutureOperator<ThenFuture, void, Map> Then(
    Map&& map, const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return FutureOperator<ThenFuture, void, Map>{std::forward<Map>(map), info};
}

// MARK: Function definitions

template <class F, class Map>
auto ThenFuture<F, Map>::UnsafePoll(PollContext& context) -> PollResult<Result> {
  const PollContext::Trace trace{context, "Then", info_};

  MappedFuture* mapped_future{then_.template TryAs<MappedFuture>()};
  if (mapped_future == nullptr) {
    using T = FutureResult<F>;

    PollResult<T> poll_result{ThenFuture::PollFuture(future_, context)};
    T* result{poll_result.template TryAs<T>()};
    if (result == nullptr) {
      return PendingResult<Result>();
    }
    MappedFuture mapped{horus_internal::DoMap(then_.template As<Map>(), result)};
    mapped_future = &then_.template Emplace<MappedFuture>(std::move(mapped));
  }
  return ThenFuture::PollFuture(*mapped_future, context);
}

}  // namespace horus

#endif  // HORUS_FUTURE_THEN_H_
