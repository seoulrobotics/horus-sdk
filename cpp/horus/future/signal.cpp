#include "horus/future/signal.h"

#include <uv.h>

#include <csignal>

#include "horus/event_loop/uv.h"
#include "horus/future/poll.h"

namespace horus {

PollResult<void> AwaitSignalFuture::UnsafePoll(PollContext& context) {
  const PollContext::Trace trace{context, "AwaitSignal", info_};

  if (signum_ == 0) {
    return ReadyResult<void>();
  }
  static_cast<void>(handle_.Engage(
      this, &AwaitSignalFuture::handle_, context, &uv_signal_init,
      [this](uv_signal_t& signal) noexcept {
        horus_internal::UvThrowIfError(uv_signal_start(
            &signal,
            handle_.CreateDoAndCloseAndWakeUpCallback<&AwaitSignalFuture::MarkSignaled,
                                                      decltype(SIGINT)>(),
            signum_));
      }));
  return PendingResult<void>();
}

}  // namespace horus
