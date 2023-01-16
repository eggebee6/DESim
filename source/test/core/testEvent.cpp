#include "gtest/gtest.h"
#include "core/Event.h"

using namespace des;

TEST(testEvent, ctor)
{
  Event e1{12, 34};
  Event e2{56, 78};
  Event e3{12, 78, 90};

  EXPECT_EQ(12, e1.time());
  EXPECT_EQ(34, e1.type());
  EXPECT_EQ(0, e1.tag());

  EXPECT_EQ(56, e2.time());
  EXPECT_EQ(78, e2.type());
  EXPECT_EQ(0, e2.tag());

  EXPECT_EQ(12, e3.time());
  EXPECT_EQ(78, e3.type());
  EXPECT_EQ(90, e3.tag());

  Event e5 = e1;
  EXPECT_EQ(12, e5.time());
  EXPECT_EQ(34, e5.type());
  EXPECT_EQ(0, e5.tag());

  Event e6 = std::move(Event{56, 78});
  EXPECT_EQ(56, e2.time());
  EXPECT_EQ(78, e2.type());
  EXPECT_EQ(0, e2.tag());
}
