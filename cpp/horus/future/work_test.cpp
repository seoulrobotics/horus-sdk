#include "horus/future/work.h"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <thread>
#include <tuple>

#include "horus/future/join.h"
#include "horus/future/map.h"
#include "horus/future/time.h"
#include "horus/testing/event_loop.h"
#include "horus/testing/timing.h"

namespace horus {
namespace {

TEST(Work, Basic) {
  std::uint32_t const answer{TestOnlyExecute(
      DeferWork([](std::atomic<bool>& /* cancelled */) -> std::uint32_t { return 42; }))};

  EXPECT_EQ(answer, 42);
}

TEST(Work, DoesNotBlock) {
  using Clock = std::chrono::high_resolution_clock;

  static constexpr std::chrono::milliseconds kTolerance{2};
  static constexpr std::chrono::milliseconds kWorkDuration{20};
  static constexpr std::chrono::milliseconds kWaitDuration{15};

  FLAKY_BLOCK() {
    const auto elapsed = [start_time{Clock::now()}]() -> std::chrono::milliseconds {
      return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start_time);
    };

    const auto wait_durations = TestOnlyExecute(
        Join(DeferWork([elapsed](std::atomic<bool>& /* cancelled */) -> std::chrono::milliseconds {
               std::this_thread::sleep_for(kWorkDuration);
               return elapsed();
             }),
             Map(CompleteIn(kWaitDuration),
                 [elapsed]() -> std::chrono::milliseconds { return elapsed(); })));
    std::chrono::milliseconds const work_elapsed_ms{std::get<0>(wait_durations)};
    std::chrono::milliseconds const wait_elapsed_ms{std::get<1>(wait_durations)};

    FLAKY_EXPECT_GE(work_elapsed_ms.count(), kWorkDuration.count() - kTolerance.count());
    FLAKY_EXPECT_LT(work_elapsed_ms.count(), kWorkDuration.count() + kTolerance.count());
    FLAKY_EXPECT_GE(wait_elapsed_ms.count(), kWaitDuration.count() - kTolerance.count());
    FLAKY_EXPECT_LE(wait_elapsed_ms.count(), kWaitDuration.count() + kTolerance.count());
  }
}

}  // namespace
}  // namespace horus
