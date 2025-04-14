/// @file
///
/// Functions used to execute futures concurrently until one of them completes.

#ifndef HORUS_FUTURE_RACE_H_
#define HORUS_FUTURE_RACE_H_

#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "horus/attributes.h"
#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/internal/void.h"
#include "horus/types/in_place.h"
#include "horus/types/one_of.h"
#include "horus/types/span.h"

namespace horus {

// MARK: Race(Futures...)

/// Future returned by `Race(Futures&&...)`.
template <class... Futures>
class RaceFuture final : public Future<OneOf<FutureResult<Futures>...>> {
 public:
  /// @copydoc Future::Result
  using typename RaceFuture::Future::Result;

  /// Constructs the future.
  explicit RaceFuture(Futures&&... futures, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : futures_{InPlaceType<std::tuple<Futures...>>, std::move(futures)...}, info_{info} {}

  /// Polls the future until any of its inner futures has completed.
  PollResult<Result> UnsafePoll(PollContext& context) final {
    const PollContext::Trace trace{context, "Race", info_};
    return PollFutures<0>(context);
  }

 private:
  /// Base recursive case of `PollFutures()`.
  template <std::size_t I, std::enable_if_t<(I == sizeof...(Futures))>* = nullptr>
  PollResult<Result> PollFutures(PollContext& /* context */) {
    return PendingResult<Result>();
  }

  /// Polls the future at index `I`, falling back to the future at index `I + 1`.
  template <std::size_t I, std::enable_if_t<(I < sizeof...(Futures))>* = nullptr>
  PollResult<Result> PollFutures(PollContext& context);

  /// The futures to poll until completion of one of them. If `void`, we already finished.
  OneOf<std::tuple<Futures...>, void> futures_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a future which will complete when any of the given futures completes.
template <class... Futures, std::tuple<FutureResult<Futures>...>* = nullptr>
inline RaceFuture<Futures...> Race(Futures&&... futures) noexcept {
  return RaceFuture<Futures...>{std::forward<Futures>(futures)..., UnknownFutureInfo()};
}

// MARK: Race(std::array<F, N>)

/// Future returned by `Race(std::array<F, N>&&)`.
template <class F, std::size_t N>
class RaceArrayFuture final : public Future<FutureResult<F>> {
 public:
  /// @copydoc Future::Result
  using typename RaceArrayFuture::Future::Result;

  /// Constructs the future.
  explicit RaceArrayFuture(std::array<F, N>&& futures,
                           const FutureInfo& info = CurrentFutureInfo()) noexcept
      : futures_{InPlaceType<std::array<F, N>>, std::move(futures)}, info_{info} {}

  /// Polls the future until any of its inner futures has completed.
  PollResult<Result> UnsafePoll(PollContext& context) final;

 private:
  /// The futures to poll until completion of one of them. If `void`, we already finished.
  OneOf<std::array<F, N>, void> futures_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a future which will complete when any of the given futures completes.
template <class F, std::size_t N>
inline RaceArrayFuture<F, N> Race(std::array<F, N>&& futures,
                                  const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return RaceArrayFuture<F, N>{std::move(futures), info};
}

// MARK: Race(Vector)

/// Future returned by `Race(Vector&&)`.
template <class Vector>
class RaceVectorFuture final : public Future<FutureResult<typename Vector::value_type>> {
 public:
  /// @copydoc Future::Result
  using typename RaceVectorFuture::Future::Result;

  /// Constructs the future.
  explicit RaceVectorFuture(Vector&& futures, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : futures_{std::move(futures)}, info_{info} {}

  /// Polls the future until any of its inner futures has completed.
  PollResult<Result> UnsafePoll(PollContext& context) final;

 private:
  /// The futures to poll until completion of one of them. Will be emptied on completion of one the
  /// futures.
  Vector futures_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a future which will complete when any of the given futures completes.
template <class F, class A>
inline RaceVectorFuture<std::vector<F, A>> Race(
    std::vector<F, A>&& futures, const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return RaceVectorFuture<std::vector<F, A>>{std::move(futures), info};
}

// MARK: Race(Span<F>)

/// Future returned by `Race(Span<F>)`.
template <class F>
using RaceSpanFuture = RaceVectorFuture<Span<F>>;

/// Returns a future which will complete when any of the given futures completes.
template <class F>
inline RaceSpanFuture<F> Race(Span<F> futures HORUS_LIFETIME_BOUND,
                              const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return RaceSpanFuture<F>{futures, info};
}

// MARK: Function definitions

template <class... Futures>
template <std::size_t I, std::enable_if_t<(I < sizeof...(Futures))>*>
auto RaceFuture<Futures...>::PollFutures(PollContext& context) -> PollResult<Result> {
  using FutureResult = FutureResult<std::tuple_element_t<I, std::tuple<Futures...>>>;

  const PollContext::Trace trace{context, "Race", info_};

  std::tuple<Futures...>* const futures{futures_.template TryAs<std::tuple<Futures...>>()};
  if (futures == nullptr) {
    throw PolledAfterCompletionError{};
  }
  PollResult<FutureResult> maybe_result{RaceFuture::PollFuture(std::get<I>(*futures), context)};
  FutureResult* result{maybe_result.template TryAs<FutureResult>()};
  if (result == nullptr) {
    return PollFutures<I + 1>(context);
  }
  return ReadyResult<Result>(horus_internal::SwitchVoid<FutureResult>(
      /*if_void=*/[](auto const& /*forward*/) noexcept
      -> Result { return Result{InPlaceType<FutureResult>}; },
      /*if_not_void=*/
      [result](auto forward) noexcept -> Result {
        return Result{InPlaceType<FutureResult>, std::move(*forward(result))};
      }));
}

template <class F, std::size_t N>
auto RaceArrayFuture<F, N>::UnsafePoll(PollContext& context) -> PollResult<Result> {
  const PollContext::Trace trace{context, "Join", info_};

  std::array<F, N>* const futures{futures_.template TryAs<std::array<F, N>>()};
  if (futures == nullptr) {
    throw PolledAfterCompletionError{};
  }
  for (F& future : *futures) {
    PollResult<Result> maybe_result{RaceArrayFuture::PollFuture(future, context)};
    Result* result{maybe_result.template TryAs<Result>()};
    if (result != nullptr) {
      return horus_internal::SwitchVoid<Result>(
          /*if_void=*/[](auto forward) noexcept
          -> PollResult<Result> { return PollResult<Result>{InPlaceType<decltype(forward())>}; },
          /*if_not_void=*/
          [result](auto forward) noexcept -> PollResult<Result> {
            return ReadyResult<Result>(std::move(*forward(result)));
          });
    }
  }
  return PendingResult<Result>();
}

template <class Vector>
auto RaceVectorFuture<Vector>::UnsafePoll(PollContext& context) -> PollResult<Result> {
  const PollContext::Trace trace{context, "Join", info_};

  for (typename Vector::value_type& future : futures_) {
    PollResult<Result> maybe_result{RaceVectorFuture::PollFuture(future, context)};
    Result* result{maybe_result.template TryAs<Result>()};
    if (result != nullptr) {
      futures_ = {};  // Cancel remaining futures.
      return ReadyResult<Result>(std::move(*result));
    }
  }
  return PendingResult<Result>();
}

}  // namespace horus

#endif  // HORUS_FUTURE_RACE_H_
