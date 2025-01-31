#include "horus/future/from_continuation.h"

#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <thread>
#include <utility>

#include "horus/testing/event_loop.h"

namespace horus {
namespace {

/// A wrapper around a `std::thread` which automatically `join()`s it on destruction.
class JoinThread final {
 public:
  JoinThread() noexcept = default;

  JoinThread(const JoinThread&) = delete;
  JoinThread& operator=(const JoinThread&) = delete;
  JoinThread(JoinThread&&) = delete;
  JoinThread& operator=(JoinThread&&) = delete;

  ~JoinThread() noexcept {
    if (thread_.joinable()) {
      thread_.join();
    }
  }

  template <class... Args>
  void Start(Args&&... args) {
    if (thread_.joinable()) {
      thread_.join();
    }
    thread_ = std::thread{std::forward<Args>(args)...};
  }

 private:
  std::thread thread_;
};

/// Spawns `background_work(continuation)` in another thread and returns a `Future<T>` which
/// completes when a message is received.
template <class T>
FromContinuationFuture<T> FromBackgroundContinuation(
    JoinThread& in_thread, std::function<void(Continuation<T>&&)> background_work) noexcept {
  auto future_and_continuation = FromContinuation<T>();

  in_thread.Start(std::move(background_work), std::move(future_and_continuation.second));

  return std::move(future_and_continuation.first);
}

/// Runs `immediate_work(continuation)` then returns a `Future<T>` which completes when a message is
/// received.
template <class T>
FromContinuationFuture<T> FromImmediateContinuation(
    std::function<void(Continuation<T>&)> immediate_work) {
  auto future_and_continuation = FromContinuation<T>();

  immediate_work(future_and_continuation.second);

  return std::move(future_and_continuation.first);
}

TEST(FromContinuation, ContinueWith) {
  JoinThread thread;

  std::int32_t const result{TestOnlyExecute(FromBackgroundContinuation<std::int32_t>(
      thread, [](Continuation<std::int32_t>&& continuation) {
        Continuation<std::int32_t> owned_continuation{std::move(continuation)};
        EXPECT_TRUE(owned_continuation.ContinueWith(1));
      }))};

  EXPECT_EQ(result, 1);
}

TEST(FromContinuation, ContinueWithDelayed) {
  JoinThread thread;

  std::int32_t const result{TestOnlyExecute(FromBackgroundContinuation<std::int32_t>(
      thread, [](Continuation<std::int32_t>&& continuation) {
        Continuation<std::int32_t> owned_continuation{std::move(continuation)};
        std::this_thread::sleep_for(std::chrono::milliseconds{10});
        EXPECT_TRUE(owned_continuation.ContinueWith(1));
      }))};

  EXPECT_EQ(result, 1);
}

TEST(FromContinuation, ContinueWithBeforePoll) {
  std::int32_t const result{TestOnlyExecute(
      FromImmediateContinuation<std::int32_t>([](Continuation<std::int32_t>& continuation) {
        EXPECT_TRUE(continuation.ContinueWith(1));
      }))};

  EXPECT_EQ(result, 1);
}

TEST(FromContinuation, FailWith) {
  JoinThread thread;

  EXPECT_THROW(TestOnlyExecute(FromBackgroundContinuation<std::int32_t>(
                   thread,
                   [](Continuation<std::int32_t>&& continuation) {
                     Continuation<std::int32_t> owned_continuation{std::move(continuation)};
                     EXPECT_TRUE(owned_continuation.FailWith(std::runtime_error{"oh noes"}));
                   })),
               std::runtime_error);
}

TEST(FromContinuation, FailWithDelayed) {
  JoinThread thread;

  EXPECT_THROW(TestOnlyExecute(FromBackgroundContinuation<std::int32_t>(
                   thread,
                   [](Continuation<std::int32_t>&& continuation) {
                     Continuation<std::int32_t> owned_continuation{std::move(continuation)};
                     std::this_thread::sleep_for(std::chrono::milliseconds{10});
                     EXPECT_TRUE(owned_continuation.FailWith(std::runtime_error{"oh noes"}));
                   })),
               std::runtime_error);
}

TEST(FromContinuation, FailWithBeforePoll) {
  EXPECT_THROW(TestOnlyExecute(FromImmediateContinuation<std::int32_t>(
                   [](Continuation<std::int32_t>& continuation) {
                     EXPECT_TRUE(continuation.FailWith(std::runtime_error{"oh noes"}));
                   })),
               std::runtime_error);
}

TEST(FromContinuation, SenderDestroyed) {
  JoinThread thread;

  EXPECT_THROW(TestOnlyExecute(FromBackgroundContinuation<std::int32_t>(
                   thread,
                   [](Continuation<std::int32_t>&& continuation) {
                     Continuation<std::int32_t> const owned_continuation{std::move(continuation)};
                     static_cast<void>(owned_continuation);
                   })),
               BrokenContinuationError);
}

TEST(FromContinuation, SenderDestroyedDelayed) {
  JoinThread thread;

  EXPECT_THROW(TestOnlyExecute(FromBackgroundContinuation<std::int32_t>(
                   thread,
                   [](Continuation<std::int32_t>&& continuation) {
                     Continuation<std::int32_t> const owned_continuation{std::move(continuation)};
                     std::this_thread::sleep_for(std::chrono::milliseconds{10});
                     static_cast<void>(owned_continuation);
                   })),
               BrokenContinuationError);
}

TEST(FromContinuation, SenderDestroyedBeforePoll) {
  EXPECT_THROW(TestOnlyExecute(FromImmediateContinuation<std::int32_t>(
                   [](Continuation<std::int32_t>& continuation) {
                     Continuation<std::int32_t> const owned_continuation{std::move(continuation)};
                     static_cast<void>(owned_continuation);
                   })),
               BrokenContinuationError);
}

TEST(FromContinuation, ReceiverDestroyed) {
  auto future_and_continuation = FromContinuation<std::int32_t>();
  static_cast<void>(FromContinuationFuture<std::int32_t>(std::move(future_and_continuation.first)));

  EXPECT_FALSE(future_and_continuation.second.ContinueWith(1));
}

}  // namespace
}  // namespace horus
