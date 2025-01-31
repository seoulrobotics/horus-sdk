#include "horus/strings/str_cat.h"

#include <gtest/gtest.h>

#include <cmath>
#include <cstdint>

#include "horus/strings/pad.h"

namespace horus {
namespace {

TEST(StrCat, Basic) { EXPECT_EQ(StrCat("hello ", 42), "hello 42"); }

TEST(StrCat, Float) {
  EXPECT_EQ(StrCat("hello ", 4.323, " ", INFINITY, " ", 0., " ", -0.F), "hello 4.323 inf 0 -0");
  EXPECT_EQ(StrCat("hello ", 100e100), "hello 1e+102");
}

TEST(StrCat, Pointer) {
  const std::int32_t* const null_ptr{nullptr};
  const std::int32_t* const ptr{
      // NOLINTNEXTLINE
      reinterpret_cast<const std::int32_t*>(static_cast<std::uintptr_t>(0x123456789ABUL))};
  EXPECT_EQ(StrCat("hello ", null_ptr), "hello null");
  EXPECT_EQ(StrCat("hello ", ptr), "hello 0x123456789ab");
}

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
