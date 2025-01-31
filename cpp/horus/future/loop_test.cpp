#include "horus/future/loop.h"

#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>

#include "horus/future/any.h"
#include "horus/future/map.h"
#include "horus/future/resolved.h"
#include "horus/future/time.h"
#include "horus/testing/event_loop.h"
#include "horus/testing/timing.h"
#include "horus/types/in_place.h"

namespace horus {
namespace {

constexpr std::chrono::milliseconds k5ms{5};

TEST(Loop, Basic) {
  static constexpr std::uint32_t kIterations{3};

  ExpectExecutionTimeNear(k5ms * kIterations, []() {
    EXPECT_EQ(
        TestOnlyExecute(Loop([iteration{0U}]() mutable -> AnyFuture<LoopResult<std::uint32_t>> {
          if (iteration == kIterations) {
            return ResolveWith(LoopResult<std::uint32_t>{InPlaceType<std::uint32_t>, iteration});
          }
          return Map(CompleteIn(k5ms),
                     [iter{iteration++}]() -> LoopResult<std::uint32_t> { return LoopContinue{}; });
        })),
        kIterations);
  });
}

TEST(Loop, Void) {
  static constexpr std::uint32_t kIterations{3};

  ExpectExecutionTimeNear(k5ms * kIterations, []() {
    TestOnlyExecute(Loop([iteration{0U}]() mutable -> AnyFuture<LoopResult<void>> {
      if (iteration == kIterations) {
        return ResolveWith(LoopResult<void>{InPlaceType<void>});
      }
      return Map(CompleteIn(k5ms),
                 [iter{iteration++}]() -> LoopResult<void> { return LoopContinue{}; });
    }));
  });
}

}  // namespace
}  // namespace horus
