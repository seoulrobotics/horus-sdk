#include "horus/testing/event_loop.h"

#include <gtest/gtest.h>

#include <chrono>
#include <cstdlib>

#include "horus/future/cancel.h"
#include "horus/future/time.h"

namespace horus {
namespace {

TEST(EventLoopTesting, TestOnlyExecuteCancellation) {
  if (std::getenv("HORUS_SDK_DEBUGGING") != nullptr) {  // NOLINT(*-mt-unsafe)
    GTEST_SKIP() << "Automatic cancellations are disabled when debugging";
  }
  EXPECT_THROW(TestOnlyExecute(CompleteIn(kMaxTestTime + std::chrono::milliseconds{50})),
               CancellationError);
}

}  // namespace
}  // namespace horus
