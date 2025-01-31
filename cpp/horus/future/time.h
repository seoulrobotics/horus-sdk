/// @file
///
/// Futures that complete after some delay.

#ifndef HORUS_FUTURE_TIME_H_
#define HORUS_FUTURE_TIME_H_

#include <chrono>
#include <cstdint>

#include "horus/event_loop/handle.h"
#include "horus/future/future.h"
#include "horus/future/info.h"
#include "horus/types/in_place.h"
#include "horus/types/one_of.h"

namespace horus {

// MARK: CompleteIn()

/// Future returned by `CompleteIn()`.
class CompleteInFuture final : public Future<void> {
 public:
  /// Constructs the future.
  template <class Rep, class Period>
  explicit CompleteInFuture(std::chrono::duration<Rep, Period> timeout,
                            const FutureInfo& info = CurrentFutureInfo()) noexcept
      : deadline_{InPlaceType<std::chrono::milliseconds>,
                  std::chrono::duration_cast<std::chrono::milliseconds>(timeout)},
        info_{info} {}

  /// Polls the future until the given timeout has passed.
  ///
  /// @throws std::bad_alloc
  PollResult<void> UnsafePoll(PollContext& context) final;

 private:
  /// The deadline requested by the user.
  ///
  /// If we haven't been polled yet, this is a duration. When we get polled for the first time, this
  /// is converted to a libuv time point so that future polls use the same clock.
  OneOf<std::chrono::milliseconds, std::uint64_t> deadline_;

  /// The timer handle.
  horus_internal::Handle<uv_timer_t, CompleteInFuture> handle_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a future which will complete after the given `duration` has passed.
template <class Rep, class Period>
inline CompleteInFuture CompleteIn(std::chrono::duration<Rep, Period> duration,
                                   const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return CompleteInFuture{duration, info};
}

// MARK: CompleteAt()

/// Future returned by `CompleteAt()`.
template <class Clock>
class CompleteAtFuture final : public Future<void> {
 public:
  /// Constructs the future.
  explicit CompleteAtFuture(std::chrono::time_point<Clock> deadline,
                            const FutureInfo& info = CurrentFutureInfo()) noexcept
      : state_{InPlaceType<std::chrono::time_point<Clock>>, deadline}, info_{info} {}

  /// Polls the future until the given deadline is reached.
  ///
  /// @throws std::bad_alloc
  PollResult<void> UnsafePoll(PollContext& context) final {
    if (state_.template Is<std::chrono::time_point<Clock>>()) {
      state_.template Emplace<CompleteInFuture>(
          state_.template As<std::chrono::time_point<Clock>>() - Clock::now(), info_);
    }
    return PollFuture(state_.template As<CompleteInFuture>(), context);
  }

 private:
  /// The state of the future. If this is a time point, we haven't been polled yet.
  OneOf<std::chrono::time_point<Clock>, CompleteInFuture> state_;
  /// Future information.
  FutureInfo info_;
};

/// Returns a future which will complete when the given `deadline` is reached.
template <class Clock>
inline CompleteAtFuture<Clock> CompleteAt(std::chrono::time_point<Clock> deadline,
                                          const FutureInfo& info = CurrentFutureInfo()) noexcept {
  return CompleteAtFuture<Clock>{deadline, info};
}

}  // namespace horus

#endif  // HORUS_FUTURE_TIME_H_
