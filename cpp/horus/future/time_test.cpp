#include "horus/future/time.h"

#include <gtest/gtest.h>

#include <chrono>

#include "horus/future/race.h"
#include "horus/future/resolved.h"
#include "horus/future/run.h"
#include "horus/testing/timing.h"

namespace horus {
namespace {

constexpr std::chrono::milliseconds kTimeToWait{10};

TEST(Time, CompleteIn) {
  ExpectExecutionTimeNear(kTimeToWait, []() { RunFuture(CompleteIn(kTimeToWait)); });
}

TEST(Time, CompleteAt) {
  ExpectExecutionTimeNear(kTimeToWait, []() {
    RunFuture(CompleteAt(std::chrono::high_resolution_clock::now() + kTimeToWait));
  });
}

TEST(Time, CompleteInCancelled) {
  ExpectExecutionTimeLt(kTimeToWait,
                        []() { RunFuture(Race(CompleteIn(kTimeToWait), ResolvedFuture<void>{})); });
}

}  // namespace
}  // namespace horus
