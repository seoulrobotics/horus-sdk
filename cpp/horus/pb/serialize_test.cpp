#include "horus/pb/serialize.h"

#include <gtest/gtest.h>
#include <horus/strings/str_cat.h>
#include <unistd.h>  // IWYU pragma: keep

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

#include "horus/pb/buffer.h"
#include "horus/pb/cow_bytes.h"
#include "horus/pb/cow_repeated.h"
#include "horus/pb/cow_span.h"
#include "horus/pb/testing/messages_pb.h"
#include "horus/types/string_view.h"

namespace horus {
namespace {

// spell-checker: ignore textpb

/// Converts a textproto string to a binpb string.
std::string ToBinpb(StringView textpb) {
  if (textpb.contains('\'')) {
    throw std::logic_error{"textpb cannot contain '"};
  }
  std::string command{"cd '"};
  command.append(HORUS_SDK_ROOT_DIR);
  command.append("' && echo '");
  command.append(textpb);
  command.append(
      "' | protoc -I proto horus/pb/testing/messages.proto --encode=horus.pb.TestMessage");
  std::FILE* command_file{popen(command.c_str(), "r")};  // NOLINT(cert-env33-c, *-include-cleaner)
  std::string command_output;
  std::array<char, 256> buffer{};
  for (;;) {
    std::size_t const read{std::fread(buffer.data(), 1, buffer.size(), command_file)};
    command_output.append(buffer.data(), read);
    if (std::feof(command_file) == 0) {
      continue;
    }
    if (std::ferror(command_file) != 0) {
      static_cast<void>(pclose(command_file));  // NOLINT(*-include-cleaner)
      throw std::runtime_error{StrCat("could not read output of process `", command, "`")};
    }
    std::int32_t const status{WEXITSTATUS(pclose(command_file))};  // NOLINT(*-include-cleaner)
    if (status != 0) {
      throw std::runtime_error{StrCat("process exited with status ", status, ": `", command, "`")};
    }
    return command_output;
  }
}

TEST(ToBinpb, Example) {
  // clang-format off
  // echo 'u32: 42' | protoc -I proto horus/pb/testing/messages.proto --encode=horus.pb.TestMessage | xxd --include
  // clang-format on
  ASSERT_EQ(ToBinpb(R"pb(u32: 42)pb"), (std::string{0x28, 0x2a}));
}

/// Ensures that the serialized representation of `message` matches the serialized `textpb`, and
/// that parsing from that serialization succeeds, then calls `check()` on both `message` and its
/// deserialized version.
void TestSerialize(const pb::TestMessage& message, StringView textpb,
                   const std::function<void(const pb::TestMessage&)>& check) {
  std::string const encoded{message.SerializeToString()};
  std::string expected{ToBinpb(textpb)};

  EXPECT_EQ(encoded, expected);

  PbReader reader{PbBuffer{std::move(expected)}};
  pb::TestMessage const deserialized{reader};
  {
    SCOPED_TRACE("check given message");
    check(message);
  }
  {
    SCOPED_TRACE("check deserialized");
    check(deserialized);
  }
}

TEST(Serialize, TestMessage) {
  TestSerialize(pb::TestMessage{}.set_u32(2), R"pb(u32: 2)pb",
                [](const pb::TestMessage& test_message) { EXPECT_EQ(test_message.u32(), 2); });

  TestSerialize(
      pb::TestMessage{}.set_string(CowBytes::Borrowed("bar")), R"pb(string: "bar")pb",
      [](const pb::TestMessage& test_message) { EXPECT_EQ(test_message.string().Str(), "bar"); });
}

TEST(Serialize, AllFields) {
  TestSerialize(
      pb::TestMessage{}
          .set_float(1.F)
          .set_double(2.)
          .set_i32(-3)
          .set_i64(4L)
          .set_u32(5U)
          .set_u64(6UL)
          .set_bytes(CowBytes::Borrowed(StringView{"\x01\x02", 2}))
          .set_string(CowBytes::Borrowed("abc"))
          .set_bool(true)
          .set_submessage(pb::TestMessage::SubMessage{}.set_u32(42).set_i32(-42))
          .set_oneof_string(CowBytes::Borrowed("def"))
          .set_rep_float(CowSpan<float>{2.F, 3.F})
          .set_rep_double(CowSpan<double>{2., 3.})
          .set_rep_i32(CowRepeated<std::int32_t>{-2, 3})
          .set_rep_i64(CowRepeated<std::int64_t>{-2, 3})
          .set_rep_u32(CowRepeated<std::uint32_t>{2, 3})
          .set_rep_u64(CowRepeated<std::uint64_t>{2, 3})
          .set_rep_bytes(CowRepeated<CowBytes>{}
                             .Add(CowBytes::Borrowed(StringView{"\x01\x02", 2}))
                             .Add(CowBytes::Borrowed(StringView{"\x02\x03", 2})))
          .set_rep_string(
              CowRepeated<CowBytes>{}.Add(CowBytes::Borrowed("abc")).Add(CowBytes::Borrowed("def")))
          .set_rep_submessage(
              CowRepeated<pb::TestMessage::SubMessage>{}
                  .Add(pb::TestMessage::SubMessage{}.set_string(CowBytes::Borrowed("ghi")))
                  .Add())
          .set_rep_bool(CowRepeated<bool>{true, false}),
      R"pb(
        float: 1.
        double: 2.
        i32: -3
        i64: 4
        u32: 5
        u64: 6
        bytes: "\x01\x02"
        string: "abc"
        submessage { u32: 42 i32: -42 }
        bool: true

        oneof_string: "def"

        rep_float: 2.
        rep_float: 3.
        rep_double: 2.
        rep_double: 3.
        rep_i32: -2
        rep_i32: 3
        rep_i64: -2
        rep_i64: 3
        rep_u32: 2
        rep_u32: 3
        rep_u64: 2
        rep_u64: 3
        rep_bytes: "\x01\x02"
        rep_bytes: "\x02\x03"
        rep_string: "abc"
        rep_string: "def"
        rep_submessage { string: "ghi" }
        rep_submessage {}
        rep_bool: true
        rep_bool: false
      )pb",
      [](const pb::TestMessage& test_message) {
        EXPECT_EQ(test_message.float_(), 1.F);
        EXPECT_EQ(test_message.double_(), 2.F);
        EXPECT_EQ(test_message.i32(), -3);
        EXPECT_EQ(test_message.i64(), 4L);
        EXPECT_EQ(test_message.u32(), 5U);
        EXPECT_EQ(test_message.u64(), 6UL);
        EXPECT_EQ(test_message.bytes().Str(), CowBytes::Borrowed(StringView{"\x01\x02", 2}).Str());
        EXPECT_EQ(test_message.string().Str(), CowBytes::Borrowed("abc").Str());
        EXPECT_EQ(test_message.bool_(), true);
        EXPECT_EQ(test_message.submessage().u32(), 42);
        EXPECT_EQ(test_message.submessage().i32(), -42);

        EXPECT_TRUE(test_message.submessage().has_u32());
        EXPECT_FALSE(test_message.submessage().has_string());

        EXPECT_TRUE(test_message.has_oneof_string());
        EXPECT_FALSE(test_message.has_oneof_submessage());

        EXPECT_EQ(test_message.test_oneof_case(), pb::TestMessage::TestOneofOneof::kOneofString);
      });
}

}  // namespace
}  // namespace horus
