#include <gtest/gtest.h>

#include "horus/pb/cow_bytes.h"
#include "horus/pb/testing/messages_pb.h"

namespace horus {
namespace {

TEST(Message, U32) {
  pb::TestMessage message;
  EXPECT_FALSE(message.has_u32());
  EXPECT_EQ(message.u32(), 0);
  EXPECT_TRUE(message.IsEmpty());

  message.set_u32(1);

  EXPECT_FALSE(message.IsEmpty());
  EXPECT_EQ(message.u32(), 1);
}

TEST(Message, Oneof) {
  pb::TestMessage message;
  EXPECT_EQ(message.test_oneof_case(), pb::TestMessage::TestOneofOneof::kNotSet);
  EXPECT_TRUE(message.IsEmpty());

  message.mutable_oneof_string() = CowBytes::Borrowed("foo");

  EXPECT_FALSE(message.IsEmpty());
  EXPECT_EQ(message.test_oneof_case(), pb::TestMessage::TestOneofOneof::kOneofString);
  EXPECT_EQ(message.oneof_string().Str(), "foo");

  EXPECT_TRUE(message.has_oneof_string());
  EXPECT_FALSE(message.has_oneof_bytes());

  message.mutable_oneof_bytes() = CowBytes::Borrowed("bar");

  EXPECT_FALSE(message.has_oneof_string());
  EXPECT_TRUE(message.has_oneof_bytes());
  EXPECT_EQ(message.test_oneof_case(), pb::TestMessage::TestOneofOneof::kOneofBytes);
  EXPECT_EQ(message.oneof_bytes().Str(), "bar");
  EXPECT_EQ(message.oneof_string().Str(), "");
}

}  // namespace
}  // namespace horus
