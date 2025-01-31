#include "horus/future/try.h"

#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>
#include <exception>
#include <stdexcept>

#include "horus/future/any.h"
#include "horus/future/map.h"
#include "horus/future/rejected.h"
#include "horus/future/resolved.h"
#include "horus/future/time.h"
#include "horus/testing/event_loop.h"

namespace horus {
namespace {

RejectedFuture<void> ImmediateRuntimeError() {
  return RejectedFuture<void>(std::runtime_error{"oh noes"});
}

AnyFuture<void> DelayedRuntimeError() {
  return Map(CompleteIn(std::chrono::milliseconds{1}),
             []() { throw std::runtime_error{"oh noes"}; });
}

TEST(Try, NoException) {
  EXPECT_EQ(
      TestOnlyExecute(Try(ResolveWith(1), [](const std::exception&) -> std::int32_t { return 2; })),
      1);

  EXPECT_EQ(TestOnlyExecute(Try(
                ResolveWith(1), [](const std::exception&) -> std::int32_t { return 2; },
                []() noexcept { EXPECT_EQ(std::current_exception(), nullptr); })),
            1);
}

TEST(Try, OneHandler) {
  {
    bool caught_exception{false};
    TestOnlyExecute(Try(ImmediateRuntimeError(),
                        [&caught_exception](const std::exception&) { caught_exception = true; }));
    EXPECT_TRUE(caught_exception);
  }

  {
    bool caught_exception{false};
    TestOnlyExecute(Try(DelayedRuntimeError(),
                        [&caught_exception](const std::exception&) { caught_exception = true; }));
    EXPECT_TRUE(caught_exception);
  }

  EXPECT_THROW(TestOnlyExecute(Try(DelayedRuntimeError(), [](const std::logic_error&) { FAIL(); })),
               std::runtime_error);
}

TEST(Try, TwoHandlers) {
  {
    bool caught_exception{false};
    TestOnlyExecute(Try(
        DelayedRuntimeError(),
        [&caught_exception](const std::runtime_error&) { caught_exception = true; },
        [](const std::exception&) { FAIL(); }));
    EXPECT_TRUE(caught_exception);
  }

  {
    bool caught_exception{false};
    TestOnlyExecute(Try(
        DelayedRuntimeError(),
        [&caught_exception](const std::exception&) { caught_exception = true; },
        [](const std::runtime_error&) { FAIL(); }));
    EXPECT_TRUE(caught_exception);
  }

  {
    bool caught_exception{false};
    TestOnlyExecute(Try(
        DelayedRuntimeError(), [](const std::logic_error&) { FAIL(); },
        [&caught_exception](const std::runtime_error&) { caught_exception = true; }));
    EXPECT_TRUE(caught_exception);
  }

  {
    bool caught_exception{false};
    TestOnlyExecute(
        DelayedRuntimeError() |
        Catch([&caught_exception](const std::runtime_error&) { caught_exception = true; },
              [](const std::logic_error&) { FAIL(); }));
    EXPECT_TRUE(caught_exception);
  }
}

TEST(Try, Finally) {
  EXPECT_EQ(TestOnlyExecute(
                Try(ResolveWith(1), []() noexcept { EXPECT_FALSE(std::uncaught_exception()); })),
            1);

  EXPECT_THROW(TestOnlyExecute(Try(DelayedRuntimeError(),
                                   []() noexcept { EXPECT_TRUE(std::uncaught_exception()); })),
               std::runtime_error);
}

}  // namespace
}  // namespace horus
