/// @file
///
/// Testing helpers for measuring time.

#ifndef HORUS_TESTING_TIMING_H_
#define HORUS_TESTING_TIMING_H_

#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

namespace horus {

/// Returns the number of retries that should be done in a flaky timing test.
inline std::uint32_t TimingTestRetries() noexcept {
  static const std::uint32_t kTimingTestRetries{
      std::getenv("CI") == nullptr ? 1U : 3U};  // NOLINT(concurrency-mt-unsafe)
  return kTimingTestRetries;
}

// NOLINTBEGIN(*-macro-usage)

/// Starts a block in which `FLAKY` expectations can be made. The block will execute multiple times
/// if it fails in order to avoid flaky failures.
///
/// Example:
///
///  FLAKY_BLOCK() {
///    std::uint32_t const a{...};
///    FLAKY_EXPECT_GE(a, min);
///    FLAKY_EXPECT_LE(a, max);
///  }
#define FLAKY_BLOCK()                             \
  for (std::uint32_t flaky_tries{0}, flaky_ok{0}; \
       std::exchange(flaky_ok, 1) == 0 && flaky_tries <= TimingTestRetries(); ++flaky_tries)

/// Flaky expectation; if all `FLAKY` expectations in a `FLAKY_BLOCK` succeed, the block stops.
/// Otherwise, it will retry until some count is reached, after which expectations might truly fail.
#define FLAKY(CHECK, DO_EXPECT, ...)        \
  if (flaky_tries == TimingTestRetries()) { \
    DO_EXPECT(__VA_ARGS__);                 \
  } else if (!(CHECK)) {                    \
    flaky_ok = 0;                           \
    continue;                               \
  } else                                    \
    static_cast<void>(0)

#define FLAKY_EXPECT_EQ(LHS, RHS) FLAKY((LHS) == (RHS), EXPECT_EQ, LHS, RHS)
#define FLAKY_EXPECT_NE(LHS, RHS) FLAKY((LHS) != (RHS), EXPECT_NE, LHS, RHS)
#define FLAKY_EXPECT_LT(LHS, RHS) FLAKY((LHS) < (RHS), EXPECT_LT, LHS, RHS)
#define FLAKY_EXPECT_LE(LHS, RHS) FLAKY((LHS) <= (RHS), EXPECT_LE, LHS, RHS)
#define FLAKY_EXPECT_GT(LHS, RHS) FLAKY((LHS) > (RHS), EXPECT_GT, LHS, RHS)
#define FLAKY_EXPECT_GE(LHS, RHS) FLAKY((LHS) >= (RHS), EXPECT_GE, LHS, RHS)
#define FLAKY_EXPECT_TRUE(EXPR) FLAKY(EXPR, EXPECT_TRUE, EXPR)

#define FLAKY_ASSERT_EQ(LHS, RHS) FLAKY((LHS) == (RHS), ASSERT_EQ, LHS, RHS)
#define FLAKY_ASSERT_NE(LHS, RHS) FLAKY((LHS) != (RHS), ASSERT_NE, LHS, RHS)
#define FLAKY_ASSERT_LT(LHS, RHS) FLAKY((LHS) < (RHS), ASSERT_LT, LHS, RHS)
#define FLAKY_ASSERT_LE(LHS, RHS) FLAKY((LHS) <= (RHS), ASSERT_LE, LHS, RHS)
#define FLAKY_ASSERT_GT(LHS, RHS) FLAKY((LHS) > (RHS), ASSERT_GT, LHS, RHS)
#define FLAKY_ASSERT_GE(LHS, RHS) FLAKY((LHS) >= (RHS), ASSERT_GE, LHS, RHS)

#define FLAKY_EXPECT_NO_THROW(...)          \
  if (flaky_tries == TimingTestRetries()) { \
    EXPECT_NO_THROW(__VA_ARGS__);           \
  } else {                                  \
    try {                                   \
      __VA_ARGS__;                          \
    } catch (...) {                         \
      flaky_ok = 0;                         \
      continue;                             \
    }                                       \
  }

// NOLINTEND(*-macro-usage)

/// Runs `invocable()` then `EXPECT`s its execution time to be between `min_time_incl` and
/// `max_time_incl`.
///
/// In case of failure, this will run again to avoid flaky tests.
template <class F>
void ExpectExecutionTimeBetween(std::chrono::milliseconds min_time_incl,
                                std::chrono::milliseconds max_time_incl, const F& invocable) {
  using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
                                   std::chrono::high_resolution_clock, std::chrono::steady_clock>;

  FLAKY_BLOCK() {
    Clock::time_point const start_time{Clock::now()};
    invocable();
    std::chrono::milliseconds const elapsed_ms{
        std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start_time)};

    // Use `.count()` to have GoogleTest print the durations in ms.
    FLAKY_EXPECT_GE(elapsed_ms.count(), min_time_incl.count());
    FLAKY_EXPECT_LE(elapsed_ms.count(), max_time_incl.count());
  }
}

/// Same as `ExpectExecutionTimeBetween()`, but the min and max times are
/// `[time-tolerance,time+tolerance]`.
template <class F>
void ExpectExecutionTimeNear(std::chrono::milliseconds time, const F& invocable) {
  static const std::chrono::milliseconds kTolerance{
      std::getenv("CI") == nullptr ? 1L : 2L};  // NOLINT(concurrency-mt-unsafe)

  ExpectExecutionTimeBetween(time - kTolerance, time + kTolerance, invocable);
}

/// Same as `ExpectExecutionTimeBetween()`, but with no expectation of a minimum execution time.
template <class F>
void ExpectExecutionTimeLe(std::chrono::milliseconds max_time_incl, const F& invocable) {
  ExpectExecutionTimeBetween(std::chrono::milliseconds::zero(), max_time_incl, invocable);
}

/// Same as `ExpectExecutionTimeBetween()`, but with no expectation of a minimum execution time.
template <class F>
void ExpectExecutionTimeLt(std::chrono::milliseconds max_time_incl, const F& invocable) {
  ASSERT_GT(max_time_incl.count(), 0);
  ExpectExecutionTimeLe(max_time_incl - std::chrono::milliseconds{1}, invocable);
}

}  // namespace horus

#endif  // HORUS_TESTING_TIMING_H_
