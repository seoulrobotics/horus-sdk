#include "horus/future/time.h"

#include <uv.h>

#include <cassert>
#include <chrono>
#include <cstdint>

#include "horus/event_loop/event_loop.h"
#include "horus/event_loop/handle.h"
#include "horus/event_loop/uv.h"
#include "horus/future/poll.h"

namespace horus {
namespace {

/// Clamps a (possibly negative) `std::chrono::milliseconds` timeout to a positive ms timeout.
constexpr std::uint64_t ClampMsTimeout(std::chrono::milliseconds timeout) noexcept {
  return timeout.count() <= 0 ? 0UL : static_cast<std::uint64_t>(timeout.count());
}

}  // namespace

PollResult<void> CompleteInFuture::UnsafePoll(PollContext& context) {
  const PollContext::Trace trace{context, "CompleteIn", info_};

  std::uint64_t const now{uv_now(horus_internal::EventLoopToUv(&context.Loop()))};
  if (!deadline_.Is<std::uint64_t>()) {
    // Not yet polled.
    std::uint64_t const ms_timeout{ClampMsTimeout(deadline_.As<std::chrono::milliseconds>())};
    std::uint64_t const deadline{now + ms_timeout};

    deadline_.Emplace<std::uint64_t>(deadline);

    const bool engaged{handle_.Engage(this, &CompleteInFuture::handle_, context, &uv_timer_init,
                                      [this, ms_timeout](uv_timer_t& timer) noexcept {
                                        horus_internal::UvThrowIfError(uv_timer_start(
                                            &timer, handle_.CreateCloseAndWakeUpCallback(),
                                            ms_timeout,
                                            /*repeat=*/0));
                                      })};
    assert(engaged);  // Otherwise `deadline_` would be a different type.
    static_cast<void>(engaged);
  }
  if (now >= deadline_.As<std::uint64_t>() && !handle_.IsEngaged()) {
    return ReadyResult<void>();
  }
  return PendingResult<void>();
}

}  // namespace horus
