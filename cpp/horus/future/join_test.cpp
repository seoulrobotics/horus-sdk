#include "horus/future/join.h"

#include <gtest/gtest.h>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "horus/future/any.h"
#include "horus/future/cancel.h"
#include "horus/future/map.h"
#include "horus/future/resolved.h"
#include "horus/future/time.h"
#include "horus/testing/event_loop.h"
#include "horus/testing/timing.h"
#include "horus/types/span.h"

namespace horus {
namespace {

constexpr std::chrono::milliseconds k5ms{5};
constexpr std::chrono::milliseconds k10ms{10};

TEST(Join, Tuple) {
  ExpectExecutionTimeNear(k10ms, []() {
    const auto result = TestOnlyExecute(Join(  //
        Map(CompleteIn(k5ms), []() -> bool { return true; }),
        Map(CompleteIn(k10ms),
            []() -> std::unique_ptr<std::int32_t> { return std::make_unique<std::int32_t>(2); }),
        ResolveWith(1)));

    EXPECT_EQ(std::get<0>(result), true);
    EXPECT_EQ(*std::get<1>(result), 2);
    EXPECT_EQ(std::get<2>(result), 1);
  });
}

TEST(Join, Vector) {
  constexpr std::size_t kTaskCount{25};
  constexpr std::size_t kChunkSize{10};

  std::vector<AnyFuture<std::size_t>> futures;
  futures.reserve(kTaskCount);
  for (std::size_t i{0}; i < kTaskCount; ++i) {
    futures.emplace_back(Map(CompleteIn(std::chrono::milliseconds{i / 2}),
                             [i]() noexcept -> std::size_t { return i; }));
  }
  const auto result = TestOnlyExecute(Join<kChunkSize>(std::move(futures)));

  for (std::size_t i{0}; i < kTaskCount; ++i) {
    EXPECT_EQ(result[i], i);
  }
}

TEST(Join, Span) {
  constexpr std::size_t kTaskCount{25};
  constexpr std::size_t kChunkSize{10};

  std::vector<AnyFuture<void>> futures;
  std::vector<std::size_t> results(kTaskCount);
  futures.reserve(kTaskCount);
  for (std::size_t i{0}; i < kTaskCount; ++i) {
    futures.emplace_back(Map(CompleteIn(std::chrono::milliseconds{i / 2}),
                             [&results, i]() noexcept { results[i] = i; }));
  }
  // Keeping references to the waker in `futures` is illegal so we make sure to clear it once all
  // inner tasks have resolved.
  TestOnlyExecute(
      Map(Join<kChunkSize>(Span<AnyFuture<void>>{futures}), [&]() { futures.clear(); }));

  for (std::size_t i{0}; i < kTaskCount; ++i) {
    EXPECT_EQ(results[i], i);
  }
}

TEST(Join, Cancelled) {
  ExpectExecutionTimeNear(k5ms, []() {
    auto with_dummy_result = [](auto&& future) -> auto {
      return Map(std::forward<decltype(future)>(future), []() -> bool { return true; });
    };

    EXPECT_THROW(TestOnlyExecute(
                     Join(with_dummy_result(CancelIn(k5ms)), with_dummy_result(CompleteIn(k10ms)))),
                 CancellationError);
  });
}

}  // namespace
}  // namespace horus
