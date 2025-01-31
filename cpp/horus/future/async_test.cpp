#include "horus/future/async.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>

#include "horus/future/any.h"
#include "horus/future/join.h"
#include "horus/future/map.h"
#include "horus/future/time.h"
#include "horus/testing/event_loop.h"
#include "horus/testing/timing.h"

namespace horus {
namespace {

using ::testing::Eq;
using ::testing::FieldsAre;

constexpr std::chrono::milliseconds k5ms{5};

AnyFuture<std::int32_t> ValueIn5ms(std::int32_t value) {
  return Map(CompleteIn(k5ms), [value]() -> std::int32_t { return value; });
}

TEST(Async, Basic) {
  ExpectExecutionTimeNear(k5ms * 2, []() {
    EXPECT_EQ(TestOnlyExecute(Async([](const Awaiter& await) {
                return await(ValueIn5ms(1)) + await(ValueIn5ms(2));
              })),
              3);
  });
}

TEST(Async, DoesNotBlockEventLoop) {
  ExpectExecutionTimeNear(k5ms * 2, []() {
    EXPECT_THAT(TestOnlyExecute(Join(ValueIn5ms(0), Async([](const Awaiter& await) {
                                       return await(ValueIn5ms(1)) + await(ValueIn5ms(2));
                                     }))),
                FieldsAre(Eq(0), Eq(3)));
  });
}

}  // namespace
}  // namespace horus
