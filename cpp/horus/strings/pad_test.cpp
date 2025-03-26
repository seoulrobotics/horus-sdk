#include "horus/strings/pad.h"

#include <gtest/gtest.h>

#include "horus/strings/str_cat.h"

namespace horus {
namespace {

TEST(StrCat, Pad) {
  EXPECT_EQ(StrCat("hello ", PadLeftBy(4, 42)), "hello   42");
  EXPECT_EQ(StrCat("hello ", PadLeftBy(4, 42, '0')), "hello 0042");
  EXPECT_EQ(StrCat("hello ", PadLeftBy(4, 4200)), "hello 4200");
  EXPECT_EQ(StrCat("hello ", PadLeftBy(4, 42000)), "hello 42000");

  EXPECT_EQ(StrCat("hello ", PadRightBy(4, 42)), "hello 42  ");
  EXPECT_EQ(StrCat("hello ", PadRightBy(4, 42, '0')), "hello 4200");
  EXPECT_EQ(StrCat("hello ", PadRightBy(4, 4231)), "hello 4231");
  EXPECT_EQ(StrCat("hello ", PadRightBy(4, 42314)), "hello 42314");
}

}  // namespace
}  // namespace horus
