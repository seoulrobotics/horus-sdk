#include "horus/pb/cow_bytes.h"

#include <gtest/gtest.h>

#include <string>

namespace horus {
namespace {

TEST(CowBytes, Empty) {
  const CowBytes bytes;
  EXPECT_EQ(bytes.Str(), "");
}

TEST(CowBytes, Basic) {
  std::string source{"foo"};
  const CowBytes bytes1{CowBytes::Borrowed(source)};
  EXPECT_EQ(bytes1.Str(), "foo");

  CowBytes bytes2{bytes1};
  EXPECT_EQ(bytes2.Str(), "foo");

  source[0] = 'b';

  EXPECT_EQ(bytes1.Str(), "boo");
  EXPECT_EQ(bytes2.Str(), "boo");

  bytes2.String()[0] = 'm';

  EXPECT_EQ(source, "boo");
  EXPECT_EQ(bytes1.Str(), "boo");
  EXPECT_EQ(bytes2.Str(), "moo");
}

}  // namespace
}  // namespace horus
