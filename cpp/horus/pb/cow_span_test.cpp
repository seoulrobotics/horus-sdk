#include "horus/pb/cow_span.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <vector>

namespace horus {
namespace {

using ::testing::ElementsAre;
using ::testing::Eq;

TEST(CowSpan, Empty) {
  const CowSpan<std::uint32_t> span;
  EXPECT_TRUE(span.Span().empty());
}

TEST(CowSpan, Basic) {
  std::array<std::uint32_t, 3> array{1, 2, 3};
  const CowSpan<std::uint32_t> span1{CowSpan<std::uint32_t>::Borrowed(array)};
  EXPECT_THAT(span1.Span(), ElementsAre(Eq(1), Eq(2), Eq(3)));

  CowSpan<std::uint32_t> span2{span1};
  EXPECT_THAT(span2.Span(), ElementsAre(Eq(1), Eq(2), Eq(3)));

  array[0] = 0;
  EXPECT_THAT(span1.Span(), ElementsAre(Eq(0), Eq(2), Eq(3)));
  EXPECT_THAT(span2.Span(), ElementsAre(Eq(0), Eq(2), Eq(3)));

  span2.Vector()[0] = 4;
  EXPECT_THAT(array, ElementsAre(Eq(0), Eq(2), Eq(3)));
  EXPECT_THAT(span1.Span(), ElementsAre(Eq(0), Eq(2), Eq(3)));
  EXPECT_THAT(span2.Span(), ElementsAre(Eq(4), Eq(2), Eq(3)));
}

}  // namespace
}  // namespace horus
