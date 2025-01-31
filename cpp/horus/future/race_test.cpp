#include "horus/future/race.h"

#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>

#include "horus/future/any.h"
#include "horus/future/map.h"
#include "horus/future/resolved.h"
#include "horus/future/time.h"
#include "horus/testing/event_loop.h"
#include "horus/testing/timing.h"

namespace horus {
namespace {

using Clock = std::chrono::high_resolution_clock;

constexpr std::chrono::milliseconds kTolerance{1};
constexpr std::chrono::milliseconds k5ms{5};
constexpr std::chrono::milliseconds k10ms{10};

AnyFuture<std::int32_t> DelayedBy(std::chrono::milliseconds delay, std::int32_t value) {
  return Map(CompleteIn(delay), [value]() noexcept -> std::int32_t { return value; });
}

TEST(Race, TupleImmediateImmediate) {
  EXPECT_EQ(TestOnlyExecute(Race(ResolveWith(1), ResolveWith(2))).As<0>(), 1);
}

TEST(Race, TupleImmediateDelayed) {
  ExpectExecutionTimeLt(k5ms, []() {
    EXPECT_EQ(TestOnlyExecute(Race(ResolveWith(1), DelayedBy(k5ms, 2))).As<0>(), 1);
  });
  ExpectExecutionTimeLt(k5ms, []() {
    EXPECT_EQ(TestOnlyExecute(Race(DelayedBy(k5ms, 1), ResolveWith(2))).As<1>(), 2);
  });
}

TEST(Race, TupleDelayedDelayed) {
  ExpectExecutionTimeBetween(k5ms - kTolerance, k10ms - kTolerance, []() {
    EXPECT_EQ(TestOnlyExecute(Race(DelayedBy(k5ms, 1), DelayedBy(k10ms, 2))).As<0>(), 1);
  });
  ExpectExecutionTimeBetween(k5ms - kTolerance, k10ms - kTolerance, []() {
    EXPECT_EQ(TestOnlyExecute(Race(DelayedBy(k10ms, 1), DelayedBy(k5ms, 2))).As<1>(), 2);
  });
}

}  // namespace
}  // namespace horus
