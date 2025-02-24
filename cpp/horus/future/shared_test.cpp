#include "horus/future/shared.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <stdexcept>
#include <utility>

#include "horus/future/any.h"
#include "horus/future/rejected.h"
#include "horus/future/resolved.h"
#include "horus/testing/event_loop.h"

namespace horus {
namespace {

ResolvedFuture<std::int32_t> Resolve() { return ResolveWith(1); }

RejectedFuture<std::int32_t> Reject() {
  return RejectedFuture<std::int32_t>{std::runtime_error{"mock"}};
}

TEST(Shared, Basic) {
  SharedFuture<AnyFuture<std::int32_t>> shared{Resolve()};
  EXPECT_EQ(TestOnlyExecute(std::move(shared)), 1);
}

TEST(Shared, Multipoll) {
  SharedFuture<AnyFuture<std::int32_t>> shared{Resolve()};
  SharedFuture<AnyFuture<std::int32_t>> shared_copy{shared};
  EXPECT_EQ(TestOnlyExecute(std::move(shared)), 1);
  EXPECT_EQ(TestOnlyExecute(std::move(shared_copy)), 1);
}

TEST(Shared, Multithrow) {
  SharedFuture<AnyFuture<std::int32_t>> shared{Reject()};
  SharedFuture<AnyFuture<std::int32_t>> shared_copy{shared};
  EXPECT_THROW(TestOnlyExecute(std::move(shared)), std::runtime_error);
  EXPECT_THROW(TestOnlyExecute(std::move(shared_copy)), std::runtime_error);
}

TEST(Shared, Share) {
  ResolvedFuture<std::int32_t> resolve{Resolve()};
  SharedFuture<AnyFuture<std::int32_t>> shared{Share(std::move(resolve))};
  EXPECT_EQ(TestOnlyExecute(std::move(shared)), 1);
}

}  // namespace
}  // namespace horus
