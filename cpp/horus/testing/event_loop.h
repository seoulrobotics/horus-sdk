/// @file
///
/// Testing helpers for the `EventLoop`.

#ifndef HORUS_TESTING_EVENT_LOOP_H_
#define HORUS_TESTING_EVENT_LOOP_H_

#include <chrono>
#include <cstdlib>
#include <utility>

#include "horus/event_loop/event_loop.h"
#include "horus/future/cancel.h"
#include "horus/future/future.h"

namespace horus {

/// Maximum time to wait for completion in `TestOnlyExecute()`.
static constexpr std::chrono::milliseconds kMaxTestTime{400};

/// Executes the given future to completion.
template <class F>
FutureResult<F> TestOnlyExecute(F&& future) noexcept(false) {
  horus_internal::EventLoop event_loop;
  if (std::getenv("HORUS_SDK_DEBUGGING") != nullptr) {  // NOLINT(*-mt-unsafe)
    // Don't add a timeout to tests when we're debugging as it will prevent us from stepping
    // through code.
    return event_loop.RunFuture(std::forward<F>(future));
  }
  return event_loop.RunFuture(CancelIn(kMaxTestTime, std::forward<F>(future)));
}

template <class Test>
class EventLoopTest : public Test {
 public:
  /// Returns a reference to the event loop.
  constexpr horus_internal::EventLoop& Loop() noexcept { return event_loop_; }

 private:
  /// The test event loop.
  horus_internal::EventLoop event_loop_;
};

}  // namespace horus

#endif  // HORUS_TESTING_EVENT_LOOP_H_
