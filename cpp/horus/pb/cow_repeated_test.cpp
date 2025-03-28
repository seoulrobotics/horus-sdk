#include "horus/pb/cow_repeated.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <limits>
#include <utility>

#include "horus/pb/buffer.h"
#include "horus/pb/cow.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/serialize.h"
#include "horus/pb/testing/messages_pb.h"
#include "horus/pb/types.h"
#include "horus/strings/string_view.h"

namespace horus {
namespace {

using ::testing::ElementsAre;
using ::testing::Eq;

testing::Matcher<Cow<CowBytes>> CowEq(StringView string) {
  return testing::Property(&Cow<CowBytes>::Ref, testing::Property(&CowBytes::Str, Eq(string)));
}

TEST(CowRepeated, EmptyBytes) {
  const CowRepeated<CowBytes> rep;
  EXPECT_TRUE(rep.empty());
  EXPECT_EQ(rep.size(), 0);
}

TEST(CowRepeated, EmptyPacked) {
  const CowRepeated<std::int32_t> rep;
  EXPECT_TRUE(rep.empty());
  EXPECT_EQ(rep.size(), 0);
}

TEST(CowRepeated, BasicBytes) {
  constexpr PbTag kTag{1};
  constexpr PbTag kOtherTag{2};

  PbBuffer buffer;
  {
    PbWriter writer;
    writer.Writer().add_string(kTag, "foo");
    writer.Writer().add_int32(kOtherTag, 1);
    writer.Writer().add_string(kTag, "bar");
    buffer = std::move(writer).ToBuffer();
  }

  const CowRepeated<CowBytes> rep1{kTag, buffer.View()};
  EXPECT_EQ(rep1.size(), 2);
  EXPECT_THAT(rep1, ElementsAre(CowEq("foo"), CowEq("bar")));
}

TEST(CowRepeated, BasicPacked) {
  // Integers are encoded as varints, so we use increasingly large numbers below.
  constexpr std::int32_t kSmall{1};
  constexpr std::int32_t kMedium{
      1 + static_cast<std::int32_t>(std::numeric_limits<std::uint8_t>::max())};
  constexpr std::int32_t kLarge{
      1 + static_cast<std::int32_t>(std::numeric_limits<std::uint16_t>::max())};

  PbBuffer buffer;
  {
    const std::array<std::int32_t, 3> array{kSmall, kMedium, kLarge};
    PbWriter writer;
    writer.Writer().add_packed_int32(1, array.begin(), array.end());
    buffer = std::move(writer).ToBuffer();
  }
  // Skip tag and size.
  PbView view{buffer.View(2, buffer.Str().size() - 2)};

  const CowRepeated<std::uint32_t> rep1{std::move(view)};
  EXPECT_THAT(rep1, ElementsAre(Eq(kSmall), Eq(kMedium), Eq(kLarge)));

  EXPECT_FALSE(rep1.empty());
  EXPECT_EQ(rep1.size(), 3);

  CowRepeated<std::uint32_t> rep2{rep1};
  EXPECT_THAT(rep2, ElementsAre(Eq(kSmall), Eq(kMedium), Eq(kLarge)));

  rep2.Vector()[0] = 0;
  EXPECT_THAT(rep1, ElementsAre(Eq(kSmall), Eq(kMedium), Eq(kLarge)));
  EXPECT_THAT(rep2, ElementsAre(Eq(0), Eq(kMedium), Eq(kLarge)));

  EXPECT_FALSE(rep2.empty());
  EXPECT_EQ(rep2.size(), 3);
}

TEST(CowRepeated, BasicMessage) {
  constexpr PbTag kTag{1};

  PbBuffer buffer;
  {
    CowRepeated<pb::TestMessage> messages;
    static_cast<void>(messages.Add().set_u32(1).set_string(CowBytes::Borrowed("foo")));
    static_cast<void>(messages.Add().set_double(1.).set_oneof_submessage(
        pb::TestMessage::SubMessage{}.set_i32(1)));
    PbWriter writer;
    PbTraits<CowRepeated<pb::TestMessage>>::Serialize(writer, kTag, messages);
    buffer = std::move(writer).ToBuffer();
  }
  const StringView buffer_str{buffer.Str()};

  pb::TestMessage msg;
  {
    const CowRepeated<pb::TestMessage> rep1{kTag, buffer.View()};
    EXPECT_EQ(rep1.size(), 2);
    const pb::TestMessage msg1{(*rep1.begin()).Ref()};
    EXPECT_EQ(msg1.u32(), 1);
    EXPECT_EQ(msg1.string().Str(), "foo");
    EXPECT_GT(msg1.string().Str().data(), buffer_str.begin());
    EXPECT_LT(msg1.string().Str().data(), buffer_str.end());

    const pb::TestMessage msg2{(*(++rep1.begin())).Ref()};
    EXPECT_FALSE(msg2.has_u32());
    EXPECT_EQ(msg2.oneof_submessage().i32(), 1);

    msg = pb::TestMessage{msg1};
    buffer = {};
  }

  // Make sure that `msg` is still valid despite resetting `buffer`.
  EXPECT_EQ(msg.u32(), 1);
  EXPECT_EQ(msg.string().Str(), "foo");
  EXPECT_GT(msg.string().Str().data(), buffer_str.begin());
  EXPECT_LT(msg.string().Str().data(), buffer_str.end());
}

}  // namespace
}  // namespace horus
