/// @file
///
/// The `MapTo()` and `MapToVoid()` functions.

#ifndef HORUS_FUTURE_MAP_TO_H_
#define HORUS_FUTURE_MAP_TO_H_

#include <utility>

#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/future/operator.h"
#include "horus/future/poll.h"
#include "horus/internal/attributes.h"

namespace horus {

// MARK: MapTo()

/// Future returned by `MapTo()`.
template <class F, class MapTo>
class MapToFuture final : public Future<MapTo> {
 public:
  /// Constructs a `MapToFuture`.
  MapToFuture(F&& future, MapTo&& value, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : future_{std::move(future)}, value_{std::move(value)}, info_{info} {}

  /// Polls the future until the inner future completes, then returns the given value.
  PollResult<MapTo> UnsafePoll(PollContext& context) final {
    const PollContext::Trace trace{context, "MapTo", info_};
    if (MapToFuture::PollFuture(future_, context).template Is<Pending>()) {
      return PendingResult<MapTo>();
    }
    return ReadyResult<MapTo>(std::move(value_));
  }

 private:
  /// The inner future whose result will be mapped.
  F future_;
  /// The value to return on completion.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS MapTo value_;
  /// The location where the future was created.
  FutureInfo info_;
};

/// Returns a future which completes with `value` after executing `future`.
template <class Future, class T>
inline MapToFuture<Future, T> MapTo(Future&& future, T&& value,
                                    const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return MapToFuture<Future, T>{std::forward<Future>(future), std::forward<T>(value), info};
}

/// `Pipe()` operator which maps the result of a `Future` to `value`.
template <class T>
constexpr FutureOperator<MapToFuture, void, T> MapTo(
    T&& value, const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return FutureOperator<MapToFuture, void, T>{std::forward<T>(value), info};
}

// MARK: MapToVoid()

template <class F>
class MapToVoidFuture final : public Future<void> {
 public:
  /// Constructs a `MapToVoidFuture`.
  explicit MapToVoidFuture(F&& future, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : future_{std::move(future)}, info_{info} {}

  /// Polls the future until the inner future completes, then returns nothing.
  PollResult<void> UnsafePoll(PollContext& context) final {
    const PollContext::Trace trace{context, "MapToVoid", info_};
    if (MapToVoidFuture::PollFuture(future_, context).template Is<Pending>()) {
      return PendingResult<void>();
    }
    return ReadyResult<void>();
  }

 private:
  /// The inner future whose result will be mapped.
  F future_;
  /// The location where the future was created.
  FutureInfo info_;
};

/// Returns a future which discards the result of `future`.
template <class Future>
inline MapToVoidFuture<Future> MapToVoid(Future&& future,
                                         const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return MapToVoidFuture<Future>{std::forward<Future>(future), info};
}

/// `Pipe()` operator which maps the result of a `Future`.
constexpr FutureOperator<MapToVoidFuture, void> MapToVoid(
    const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return FutureOperator<MapToVoidFuture, void>{info};
}

}  // namespace horus

#endif  // HORUS_FUTURE_MAP_TO_H_
