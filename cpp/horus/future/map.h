/// @file
///
/// The `Map()` function.

#ifndef HORUS_FUTURE_MAP_H_
#define HORUS_FUTURE_MAP_H_

#include <type_traits>
#include <utility>

#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/future/operator.h"
#include "horus/future/poll.h"
#include "horus/internal/attributes.h"

namespace horus {
namespace horus_internal {

/// Returns `map(std::move(*ptr))`.
template <class Map, class T>
inline auto DoMap(Map& map, T* ptr) -> decltype(map(std::declval<T&&>())) {
  return map(std::move(*ptr));
}

/// Returns `map()`.
template <class Map>
inline auto DoMap(Map& map, void* /* ptr */) -> decltype(map()) {
  return map();
}

/// The result of mapping the result of `Future` with `Map`.
template <class Map, class Future>
using MappedType = decltype(DoMap(std::declval<Map&>(), std::declval<FutureResult<Future>*>()));

}  // namespace horus_internal

// MARK: Map()

/// Future returned by `Map()`.
template <class F, class Map>
class MapFuture final : public Future<horus_internal::MappedType<Map, F>> {
 public:
  /// @copydoc Future::Result
  using typename MapFuture::Future::Result;

  /// Constructs a `MapFuture`.
  MapFuture(F&& future, Map&& map, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : future_{std::move(future)}, map_{std::move(map)}, info_{info} {}

  /// Polls the future until the inner future completes, then returns its mapped result.
  PollResult<Result> UnsafePoll(PollContext& context) final;

 private:
  /// The inner future whose result will be mapped.
  F future_;
  /// The mapping function.
  HORUS_SDK_ATTRIBUTE_NO_UNIQUE_ADDRESS Map map_;
  /// The location where the future was created.
  FutureInfo info_;
};

/// Returns a future which completes with the result of `future` after mapping it with `map()`.
template <class Future, class F>
inline MapFuture<Future, F> Map(Future&& future, F&& map,
                                const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return MapFuture<Future, F>{std::forward<Future>(future), std::forward<F>(map), info};
}

/// `Pipe()` operator which maps the result of a `Future`.
template <class F>
constexpr FutureOperator<MapFuture, void, F> Map(
    F&& map, const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return FutureOperator<MapFuture, void, F>{std::forward<F>(map), info};
}

// MARK: Function definitions

namespace horus_internal {

/// Returns `map(std::move(*ptr))`.
template <class Map, class T,
          std::enable_if_t<std::is_void<decltype(DoMap(std::declval<Map&>(),
                                                       std::declval<T*>()))>::value>* = nullptr>
inline PollResult<void> DoMapPollResult(Map& map, T* ptr) {
  DoMap(map, ptr);
  return ReadyResult<void>();
}

/// Returns `map()`.
template <class Map, class T,
          std::enable_if_t<!std::is_void<decltype(DoMap(std::declval<Map&>(),
                                                        std::declval<T*>()))>::value>* = nullptr>
inline PollResult<decltype(DoMap(std::declval<Map&>(), std::declval<T*>()))> DoMapPollResult(
    Map& map, T* ptr) {
  return ReadyResult<decltype(DoMap(std::declval<Map&>(), std::declval<T*>()))>(DoMap(map, ptr));
}

}  // namespace horus_internal

template <class F, class Map>
auto MapFuture<F, Map>::UnsafePoll(PollContext& context) -> PollResult<Result> {
  using T = FutureResult<F>;

  const PollContext::Trace trace{context, "Map", info_};

  PollResult<T> poll_result{MapFuture::PollFuture(future_, context)};
  T* result{poll_result.template TryAs<T>()};
  if (result == nullptr) {
    return Pending{};
  }
  return horus_internal::DoMapPollResult(map_, result);
}

}  // namespace horus

#endif  // HORUS_FUTURE_MAP_H_
