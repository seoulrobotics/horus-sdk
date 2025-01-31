#include "horus/future/then.h"

#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>

#include "horus/future/any.h"
#include "horus/future/map.h"
#include "horus/future/resolved.h"
#include "horus/future/time.h"
#include "horus/testing/event_loop.h"

namespace horus {
namespace {

ResolvedFuture<std::int32_t> ImmediateOne() { return ResolveWith(1); }

AnyFuture<std::int32_t> DelayedOne() {
  return Map(CompleteIn(std::chrono::milliseconds{1}), []() noexcept -> std::int32_t { return 1; });
}

ResolvedFuture<std::int32_t> MapImmediatePlusOne(std::int32_t value) {
  return ResolveWith(value + 1);
}

AnyFuture<std::int32_t> MapDelayedPlusOne(std::int32_t value) {
  return Map(CompleteIn(std::chrono::milliseconds{1}),
             [value]() noexcept -> std::int32_t { return value + 1; });
}

TEST(Then, ImmediateImmediate) {
  EXPECT_EQ(TestOnlyExecute(Then(ImmediateOne(), &MapImmediatePlusOne)), 2);
}

TEST(Then, ImmediateDelayed) {
  EXPECT_EQ(TestOnlyExecute(Then(ImmediateOne(), &MapDelayedPlusOne)), 2);
}

TEST(Then, DelayedImmediate) {
  EXPECT_EQ(TestOnlyExecute(Then(DelayedOne(), &MapImmediatePlusOne)), 2);
}

TEST(Then, DelayedDelayed) {
  EXPECT_EQ(TestOnlyExecute(Then(DelayedOne(), &MapDelayedPlusOne)), 2);
}

}  // namespace
}  // namespace horus
