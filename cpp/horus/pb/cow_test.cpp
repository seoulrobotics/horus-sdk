#include "horus/pb/cow.h"

#include <gtest/gtest.h>

#include <cstdint>

namespace horus {
namespace {

TEST(Cow, Basic) {
  std::uint32_t value{1};

  Cow<std::uint32_t> const cow1{value};
  EXPECT_EQ(cow1.Ref(), 1);
  EXPECT_EQ(&cow1.Ref(), &value);

  Cow<std::uint32_t> cow2{cow1};
  EXPECT_EQ(cow2.Ref(), cow1.Ref());
  EXPECT_EQ(&cow2.Ref(), &cow1.Ref());

  value = 2;
  EXPECT_EQ(cow1.Ref(), 2);
  EXPECT_EQ(cow2.Ref(), 2);

  cow2.Mutable() = 3;
  EXPECT_EQ(value, 2);
  EXPECT_EQ(cow1.Ref(), 2);
  EXPECT_EQ(cow2.Ref(), 3);
}

}  // namespace
}  // namespace horus
