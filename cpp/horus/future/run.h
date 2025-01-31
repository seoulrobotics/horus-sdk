/// @file
///
/// The `RunFuture()` function.

#ifndef HORUS_FUTURE_RUN_H_
#define HORUS_FUTURE_RUN_H_

#include "horus/event_loop/event_loop.h"
#include "horus/future/future.h"

namespace horus {

/// Executes the given `future` in an event loop running in the current thread, then returns its
/// result.
///
/// @throws std::exception Any exception thrown by `future.UnsafePoll()`.
/// @throws std::logic_error If the `future` hasn't completed when the event loop shuts down.
template <class F>
inline FutureResult<F> RunFuture(F&& future) {
  horus_internal::EventLoop event_loop;
  return event_loop.RunFuture(std::forward<F>(future));
}

}  // namespace horus

#endif  // HORUS_FUTURE_RUN_H_
