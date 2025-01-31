/// @file
///
/// The `Loop()` function.

#ifndef HORUS_FUTURE_LOOP_H_
#define HORUS_FUTURE_LOOP_H_

#include <cassert>
#include <type_traits>
#include <utility>

#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/future/owner.h"
#include "horus/future/poll.h"
#include "horus/internal/void.h"
#include "horus/types/in_place.h"
#include "horus/types/one_of.h"

namespace horus {

/// Marker type used to indicate a `Loop()` should continue.
struct LoopContinue {};

/// The result of a `Loop()` body. If `LoopContinue`, the body will be called again. Otherwise,
/// the future completes with `T`.
template <class T>
using LoopResult = OneOf<T, LoopContinue>;

namespace horus_internal {

/// Defines `type = R` given `T = LoopResult<R>`.
template <class T>
struct LoopResultT;
template <class T>
struct LoopResultT<LoopResult<T>> {
  using type = T;
};

}  // namespace horus_internal

// MARK: Loop()

/// Future returned by `Loop()`.
template <class Body>
class LoopFuture final : public Future<typename horus_internal::LoopResultT<
                             FutureResult<decltype(std::declval<Body&>()())>>::type> {
 public:
  /// @copydoc Future::Result
  using typename LoopFuture::Future::Result;

  /// The type of the `Future` returned by `Body`.
  using BodyFuture = decltype(std::declval<Body&>()());

  /// Constructs the future.
  explicit LoopFuture(Body&& body, const FutureInfo& info = CurrentFutureInfo()) noexcept
      : body_{std::move(body)}, future_{InPlaceType<void>}, info_{info} {}

  /// Non-copyable.
  LoopFuture(const LoopFuture&) = delete;
  /// Non-copyable.
  LoopFuture& operator=(const LoopFuture&) = delete;

  /// Move-constructor; this may not be called after polling the future for the first time.
  LoopFuture(LoopFuture&& other) noexcept
      : body_{std::move(other.body_)}, future_{InPlaceType<void>}, info_{other.info_} {
    static_assert(std::is_nothrow_move_constructible<Body>::value, "");
    assert(other.future_.template Is<void>());
  }

  /// Non-move-assignable.
  LoopFuture& operator=(LoopFuture&&) = delete;

  /// Destructor.
  ~LoopFuture() noexcept final = default;

  /// Polls the future until the given body returns a non-`Pending`, non-`LoopContinue` result.
  PollResult<Result> UnsafePoll(PollContext& context) final;

 private:
  /// Alias for `FutureOwner<BodyFuture>`.
  using BodyFutureOwner = horus_internal::FutureOwner<BodyFuture>;

  /// The body of the loop to execute at the beginning of each iteration.
  Body body_;
  /// The last future returned by `body_`. This is `void` until the first call to `UnsafePoll()`,
  /// where it will be given its first value.
  OneOf<BodyFutureOwner, void> future_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a future which will execute `body()` continuously until it returns something other than
/// `LoopContinue`.
template <class Body>
inline LoopFuture<Body> Loop(Body&& body, const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return LoopFuture<Body>{std::forward<Body>(body), info};
}

// MARK: Function definitions

template <class Body>
auto LoopFuture<Body>::UnsafePoll(PollContext& context) -> PollResult<Result> {
  const PollContext::Trace trace{context, "Loop", info_};

  BodyFutureOwner* future{future_.template TryAs<BodyFutureOwner>()};
  if (future == nullptr) {
    future = &future_.template Emplace<BodyFutureOwner>(body_());
  }
  while (true) {
    if (future->DidStartDestruction()) {
      if (!future->DidCompleteDestruction()) {
        // Future is being destroyed, wait until it's done.
        return PendingResult<Result>();
      }
      // Future was fully destroyed; we can keep going with a new one.
      future = &future_.template Emplace<BodyFutureOwner>(body_());
    }

    PollResult<LoopResult<Result>> poll_result{future->PollFuture(context)};
    LoopResult<Result>* const loop_result{poll_result.template TryAs<LoopResult<Result>>()};
    if (loop_result == nullptr) {
      return PendingResult<Result>();
    }

    // Future is done, we can start destroying it.
    static_cast<void>(future->PollDestroy(context.Waker()));

    if (loop_result->template Is<LoopContinue>()) {
      continue;
    }
    // Break with value. After this we won't override `BodyFutureOwner` again so we don't need to
    // worry about destroying it here.
    return horus_internal::SwitchVoid<Result>(
        [](const auto& forward) -> PollResult<Result> {
          return ReadyResult<decltype(forward())>();
        },
        [loop_result](const auto& forward) -> PollResult<Result> {
          return ReadyResult<Result>(forward(std::move(*loop_result).template As<Result>()));
        });
  }
}

}  // namespace horus

#endif  // HORUS_FUTURE_LOOP_H_
