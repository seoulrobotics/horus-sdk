#include "horus/future/channel.h"

#include <gtest/gtest.h>

#include <cstdint>

#include "horus/testing/event_loop.h"

namespace horus {
namespace {

TEST(Channel, Basic) {
  Channel<std::int32_t> channel{/*capacity=*/4};

  channel.GetSender().SendOrWait(1);
  channel.GetSender().SendOrWait(2);

  EXPECT_EQ(TestOnlyExecute(channel.GetReceiver().Receive()), 1);
  EXPECT_EQ(TestOnlyExecute(channel.GetReceiver().Receive()), 2);
}

}  // namespace
}  // namespace horus
