#include "gtest/gtest.h"
#include "core/Event.h"
#include "core/EventQueue.h"

using namespace des;

TEST(testEventQueue, ctor)
{
  ASSERT_NO_THROW(EventQueue{});

  EventQueue q{};

  EXPECT_EQ(0, q.size());
  EXPECT_TRUE(q.empty());

  ASSERT_THROW(q.peekNext(), std::runtime_error);
  EXPECT_EQ(0, q.size());
  EXPECT_TRUE(q.empty());

  ASSERT_THROW(q.getNext(), std::runtime_error);
  EXPECT_EQ(0, q.size());
  EXPECT_TRUE(q.empty());
}

TEST(testEventQueue, queue)
{
  EventQueue q{};

  Event e1{1, 10};
  Event e2{2, 20};
  Event e3{3, 30};

  Event res{0, 0};

  EXPECT_EQ(0, q.size());
  EXPECT_TRUE(q.empty());

  // Insert (copy) and remove one event
  ASSERT_NO_THROW(q.insert(e1));
  EXPECT_EQ(1, q.size());
  EXPECT_FALSE(q.empty());

  // Check event
  res = Event{0, 0};
  ASSERT_NO_THROW(res = q.peekNext());
  EXPECT_EQ(1, q.size());
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(1, res.time());
  EXPECT_EQ(10, res.type());

  res = Event{0, 0};
  ASSERT_NO_THROW(res = q.getNext());
  EXPECT_EQ(0, q.size());
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(1, res.time());
  EXPECT_EQ(10, res.type());

  ASSERT_THROW(q.peekNext(), std::runtime_error);
  ASSERT_THROW(q.getNext(), std::runtime_error);

  // Insert (move) and remove one event
  ASSERT_NO_THROW(q.insert(Event{1, 10}));
  EXPECT_EQ(1, q.size());
  EXPECT_FALSE(q.empty());

  // Check event
  res = Event{0, 0};
  ASSERT_NO_THROW(res = q.peekNext());
  EXPECT_EQ(1, q.size());
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(1, res.time());
  EXPECT_EQ(10, res.type());

  res = Event{0, 0};
  ASSERT_NO_THROW(res = q.getNext());
  EXPECT_EQ(0, q.size());
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(1, res.time());
  EXPECT_EQ(10, res.type());

  ASSERT_THROW(q.peekNext(), std::runtime_error);
  ASSERT_THROW(q.getNext(), std::runtime_error);

  // Insert two events (copy then move)
  ASSERT_NO_THROW(q.insert(e2));
  EXPECT_EQ(1, q.size());
  EXPECT_FALSE(q.empty());

  ASSERT_NO_THROW(q.insert(Event{3, 30}));
  EXPECT_EQ(2, q.size());
  EXPECT_FALSE(q.empty());

  // Check first event
  res = Event{0, 0};
  ASSERT_NO_THROW(res = q.peekNext());
  EXPECT_EQ(2, q.size());
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(2, res.time());
  EXPECT_EQ(20, res.type());

  res = Event{0, 0};
  ASSERT_NO_THROW(res = q.getNext());
  EXPECT_EQ(1, q.size());
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(2, res.time());
  EXPECT_EQ(20, res.type());

  // Check second event
  res = Event{0, 0};
  ASSERT_NO_THROW(res = q.peekNext());
  EXPECT_EQ(1, q.size());
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(3, res.time());
  EXPECT_EQ(30, res.type());

  res = Event{0, 0};
  ASSERT_NO_THROW(res = q.getNext());
  EXPECT_EQ(0, q.size());
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(3, res.time());
  EXPECT_EQ(30, res.type());

  ASSERT_THROW(q.peekNext(), std::runtime_error);
  ASSERT_THROW(q.getNext(), std::runtime_error);

  // Insert two events (move then copy)
  ASSERT_NO_THROW(q.insert(Event{2, 20}));
  EXPECT_EQ(1, q.size());
  EXPECT_FALSE(q.empty());

  ASSERT_NO_THROW(q.insert(e3));
  EXPECT_EQ(2, q.size());
  EXPECT_FALSE(q.empty());

  // Check first event
  res = Event{0, 0};
  ASSERT_NO_THROW(res = q.peekNext());
  EXPECT_EQ(2, q.size());
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(2, res.time());
  EXPECT_EQ(20, res.type());

  res = Event{0, 0};
  ASSERT_NO_THROW(res = q.getNext());
  EXPECT_EQ(1, q.size());
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(2, res.time());
  EXPECT_EQ(20, res.type());

  // Check second event
  res = Event{0, 0};
  ASSERT_NO_THROW(res = q.peekNext());
  EXPECT_EQ(1, q.size());
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(3, res.time());
  EXPECT_EQ(30, res.type());

  res = Event{0, 0};
  ASSERT_NO_THROW(res = q.getNext());
  EXPECT_EQ(0, q.size());
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(3, res.time());
  EXPECT_EQ(30, res.type());

  ASSERT_THROW(q.peekNext(), std::runtime_error);
  ASSERT_THROW(q.getNext(), std::runtime_error);

  // Insert and remove many events
  ASSERT_EQ(0, q.size());
  for(int i = 1; i < 10; ++i)
  {
    size_t size = q.size();
    ASSERT_NO_THROW(q.insert(Event{(SimTime)i, (EventType)(i * 10)}));
    EXPECT_EQ(size + 1, q.size());
    EXPECT_FALSE(q.empty());
  }

  SimTime t = 0;
  EventTag g = 0;
  while(!q.empty())
  {
    ASSERT_NO_THROW(res = q.getNext());
    ASSERT_TRUE(res.time() > t);
    ASSERT_TRUE(res.type() > g);

    t = res.time();
    g = res.type();
  }
}

TEST(testEventQueue, orderEvent)
{
  EventQueue q{};

  // Insert events in an unordered fashion
  q.insert(Event{30, 3});
  q.insert(Event{10, 1});
  q.insert(Event{50, 5});
  q.insert(Event{20, 2});
  q.insert(Event{40, 4});

  // Events should be returned in ascending time order
  Event e1 = q.getNext();
  EXPECT_EQ(10, e1.time());
  EXPECT_EQ(1, e1.type());

  Event e2 = q.getNext();
  EXPECT_EQ(20, e2.time());
  EXPECT_EQ(2, e2.type());

  Event e3 = q.getNext();
  EXPECT_EQ(30, e3.time());
  EXPECT_EQ(3, e3.type());

  Event e4 = q.getNext();
  EXPECT_EQ(40, e4.time());
  EXPECT_EQ(4, e4.type());

  Event e5 = q.getNext();
  EXPECT_EQ(50, e5.time());
  EXPECT_EQ(5, e5.type());
}
