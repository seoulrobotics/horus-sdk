/// @file
///
/// The `FutureOwner` class.

#ifndef HORUS_FUTURE_OWNER_H_
#define HORUS_FUTURE_OWNER_H_

#include <cassert>
#include <type_traits>
#include <utility>

#include "horus/event_loop/event_loop.h"
#include "horus/future/core_owner.h"
#include "horus/future/future.h"
#include "horus/future/poll.h"
#include "horus/types/in_place.h"

namespace horus {
namespace horus_internal {

/// Owns a `Future`, allowing it to be safely executed.
///
/// Futures contain handles to the event loop which must be destroyed when the future is destroyed.
/// However, these handles can only be destroyed _asynchronously_, which requires special care.
///
/// A `FutureOwner` owns a future, its descendants, and their handles. Destruction of all this owned
/// state can be requested with `PollDestroy()`, which is _not_ guaranteed to immediately succeed.
/// The `FutureOwner` shall not be destroyed until `PollDestroy()` returns a ready `PollResult`.
template <class F>
class FutureOwner final : public CoreFutureOwner {
 public:
  /// Constructs a `FutureOwner` which owns `future`.
  explicit FutureOwner(F&& future) noexcept : future_{std::move(future)} {
    static_assert(std::is_nothrow_move_constructible<F>::value, "");
  }

  /// Non-copyable.
  FutureOwner(const FutureOwner&) = delete;
  /// Non-copyable.
  FutureOwner& operator=(const FutureOwner&) = delete;

  /// Move constructor; this may not be called after initializing the owner.
  FutureOwner(FutureOwner&& other) noexcept : CoreFutureOwner{}, future_{std::move(other.future_)} {
    assert(!other.DidStartExecution());
  }

  /// Non-move-assignable.
  FutureOwner& operator=(FutureOwner&&) = delete;

  /// Destroys the `FutureOwner`. It is illegal for the `FutureOwner` to be destroyed before
  /// `PollDestroy()` returns a ready `PollResult`.
  ~FutureOwner() noexcept = default;

  /// Returns `<owned future>.UnsafePoll(<context>)`. This function may not be called after
  /// `PollDestroy()`.
  ///
  /// @throws std::exception Any exception thrown by `F::UnsafePoll()`.
  PollResult<FutureResult<F>> PollFuture(PollContext& context) & noexcept(false) {
    assert(!DidStartDestruction() && "PollFuture() called after PollDestroy()");
    InitializeChild(context);
    PollContext child_context{context.Loop(), *this, context.Waker()};
    return future_.UnsafePoll(child_context);
  }

 private:
  /// The underlying future.
  F future_;
};

}  // namespace horus_internal

/// A future which can be polled and destroyed upon completion.
///
/// `Future`s typically can only be manipulated by the `EventLoop` due to various invariants that
/// need to be maintained while they execute and before they are destroyed.
///
/// This class can also manage these invariants; it wraps a future `F` and ensures that it is driven
/// to completion. It will only complete once the inner future has both completed and been
/// destroyed.
template <class F>
class StandaloneFuture final : public Future<FutureResult<F>> {
 public:
  /// Constructs a `StandaloneFuture`.
  explicit StandaloneFuture(F&& future) noexcept : owner_{std::move(future)} {}

  /// Polls the inner future until it completes and is fully disposed of.
  PollResult<FutureResult<F>> UnsafePoll(PollContext& context) final;

 private:
  /// The owner which actually drives the future.
  horus_internal::FutureOwner<F> owner_;
  /// The result of the future set after the inner future completes.
  PollResult<FutureResult<F>> result_{InPlaceType<Pending>};
};

template <class F>
PollResult<FutureResult<F>> StandaloneFuture<F>::UnsafePoll(PollContext& context) {
  if (!owner_.DidStartDestruction()) {
    // Future hasn't completed yet, poll it.
    PollResult<FutureResult<F>> result{owner_.PollFuture(context)};
    if (result.template Is<Pending>()) {
      return result;
    }
    // Future just completed for the first time; start its destruction.
    if (owner_.PollDestroy(context.Waker())) {
      // We were able to destroy the future immediately, and can return the result now.
      return result;
    }
    // Future will be destroyed asynchronously.
    result_ = std::move(result);  // Store the result to be obtained later.
    return PendingResult<Pending>();
  }
  // Future has completed, but may not have been fully destroyed yet.
  if (!owner_.DidCompleteDestruction()) {
    return PendingResult<FutureResult<F>>();
  }
  if (result_.template Is<Pending>()) {
    // We set `result_` back to `Pending` after returning a `Ready` result to make sure we can
    // throw this exception.
    throw PolledAfterCompletionError{};
  }
  return std::exchange(result_, PendingResult<FutureResult<F>>());
}
}  // namespace horus

#endif  // HORUS_FUTURE_OWNER_H_
