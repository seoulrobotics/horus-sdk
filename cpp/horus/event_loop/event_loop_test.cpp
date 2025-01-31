#include "horus/event_loop/event_loop.h"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>
#include <utility>

#include "horus/future/from_continuation.h"
#include "horus/future/map.h"
#include "horus/future/map_to.h"
#include "horus/future/resolved.h"
#include "horus/future/time.h"
#include "horus/testing/timing.h"
#include "horus/types/in_place.h"
#include "horus/types/one_of.h"
#include "horus/types/scope_guard.h"

namespace horus {
namespace horus_internal {
namespace {

TEST(EventLoop, Run) {
  static constexpr std::chrono::milliseconds kWaitTime{5};

  EventLoop loop;

  ExpectExecutionTimeNear(kWaitTime, [&loop]() { loop.RunFuture(CompleteIn(kWaitTime)); });
}

TEST(EventLoop, RunResult) {
  static constexpr std::chrono::milliseconds kWaitTime{5};

  EventLoop loop;

  ExpectExecutionTimeNear(
      kWaitTime, [&loop]() { EXPECT_EQ(loop.RunFuture(CompleteIn(kWaitTime) | MapTo(1)), 1); });
}

TEST(EventLoop, SpawnThenRun) {
  static constexpr std::chrono::milliseconds kWaitTime{5};

  EventLoop loop;

  // `RunFuture()` only completes when spawned future completes.
  ExpectExecutionTimeLe(std::chrono::milliseconds{1},
                        [&loop]() { loop.SpawnFuture(CompleteIn(kWaitTime)); });

  ExpectExecutionTimeNear(kWaitTime, [&loop]() { loop.RunFuture(ResolvedFuture<void>{}); });

  // Both futures execute concurrently.
  ExpectExecutionTimeLe(std::chrono::milliseconds{1},
                        [&loop]() { loop.SpawnFuture(CompleteIn(kWaitTime)); });

  ExpectExecutionTimeNear(kWaitTime, [&loop]() { loop.RunFuture(CompleteIn(kWaitTime)); });
}

TEST(EventLoop, Await) {
  static constexpr std::chrono::milliseconds kWaitTime{5};

  EventLoop loop;
  auto future_and_continuation = FromContinuation<void>();

  std::thread loop_thread{[&loop, &future_and_continuation]() {
    loop.RunFuture(std::move(future_and_continuation.first));
  }};
  const auto cleanup = Defer([&loop_thread]() noexcept { loop_thread.join(); });

  ExpectExecutionTimeNear(kWaitTime, [&loop]() { loop.AwaitFuture(CompleteIn(kWaitTime)); });

  EXPECT_TRUE(future_and_continuation.second.ContinueWith());
}

TEST(EventLoop, Invoke) {
  OneOf<EventLoop::Invoker, void> maybe_invoker{InPlaceType<void>};

  std::atomic<bool> was_invoked{false};
  {
    EventLoop loop;
    EventLoop::Invoker& invoker{maybe_invoker.Emplace<EventLoop::Invoker>(loop)};

    auto future_and_continuation = FromContinuation<void>();

    std::atomic<std::thread::id> loop_thread_id;

    std::thread loop_thread{[&invoker, &loop, &loop_thread_id, &future_and_continuation]() {
      loop_thread_id = std::this_thread::get_id();
      loop.RunFuture(
          std::move(future_and_continuation.first) | Map([&invoker]() {
            // `TryInvoke()` also works from the event loop thread.
            bool was_invoked_on_loop{false};
            EXPECT_TRUE(invoker.TryInvoke([&was_invoked_on_loop](EventLoop& /* loop */) noexcept {
              was_invoked_on_loop = true;
            }));
            EXPECT_TRUE(was_invoked_on_loop);
          }));
    }};
    const auto cleanup = Defer([&loop_thread]() noexcept { loop_thread.join(); });

    ASSERT_TRUE(invoker.TryInvoke([&loop_thread_id, &was_invoked](EventLoop& /* loop */) noexcept {
      was_invoked = true;
      EXPECT_EQ(loop_thread_id, std::this_thread::get_id());
    }));

    EXPECT_TRUE(future_and_continuation.second.ContinueWith());
  }
  EXPECT_TRUE(was_invoked);

  // Event loop was destroyed so the invoker should fail now.
  std::atomic<bool> was_invoked2{false};

  EXPECT_FALSE(maybe_invoker.As<EventLoop::Invoker>().TryInvoke(
      [&was_invoked2](EventLoop& /* loop */) noexcept { was_invoked2 = true; }));

  EXPECT_FALSE(was_invoked2);
}

}  // namespace
}  // namespace horus_internal
}  // namespace horus
