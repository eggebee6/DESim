#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "core/Event.h"
#include "core/EventHandler.h"
#include "core/SimEngine.h"

using namespace des;
using ::testing::Ref;

namespace _testSimEngine
{
  class MockHandler : public EventHandler
  {
  public:
    MockHandler() : EventHandler()
    {}

    ~MockHandler()
    {}

    MOCK_METHOD2(handleEvent, void(SimEngine& sim, const Event& evt));
    MOCK_METHOD1(initialize, void(SimEngine& sim));
    MOCK_METHOD1(finalize, void(SimEngine& sim));
  };
}
using namespace _testSimEngine;

TEST(testSimEngine, ctor)
{
  ASSERT_NO_THROW(SimEngine{});

  SimEngine sim{};

  ASSERT_FALSE(sim.hasNextEvent());
  ASSERT_EQ(0, sim.eventCount());
  ASSERT_EQ(0, sim.time());
  ASSERT_EQ(SimEngineState::Uninitialized, sim.state());

  auto handlers = sim.getAllHandlers();
  EXPECT_EQ(0, handlers.size());
}

TEST(testSimEngine, insert)
{
  SimEngine sim{};

  Event e1{1, 10};

  ASSERT_FALSE(sim.hasNextEvent());
  ASSERT_EQ(0, sim.eventCount());
  
  ASSERT_NO_THROW(sim.insertEvent(e1));
  ASSERT_TRUE(sim.hasNextEvent());
  ASSERT_EQ(1, sim.eventCount());

  ASSERT_NO_THROW(sim.insertEvent(Event{2, 20}));
  ASSERT_TRUE(sim.hasNextEvent());
  ASSERT_EQ(2, sim.eventCount());
}

TEST(testSimEngine, initialize)
{
  FAIL() << "Not implemented";
}

TEST(testSimEngine, finalize)
{
  FAIL() << "Not implemented";
}

TEST(testSimEngine, step_no_handlers)
{
  SimEngine sim{};

  Event e1{1, 10};
  Event e3{3, 30};

  Event res{0, 0};

  sim.insertEvent(Event{2, 20});
  sim.insertEvent(e1);
  sim.insertEvent(e3);

  // Check step before being initialized
  ASSERT_THROW(sim.step(), std::runtime_error);

  // Initialize simulation
  sim.initialize();
  ASSERT_TRUE(sim.hasNextEvent());
  ASSERT_EQ(3, sim.eventCount());
  ASSERT_EQ(0, sim.time());
  ASSERT_EQ(SimEngineState::Running, sim.state());

  // Check first event
  ASSERT_NO_THROW(res = sim.step());
  EXPECT_EQ(1, res.time());
  EXPECT_EQ(10, res.type());
  EXPECT_TRUE(sim.hasNextEvent());
  EXPECT_EQ(2, sim.eventCount());
  ASSERT_EQ(1, sim.time());

  // Check second event
  ASSERT_NO_THROW(res = sim.step());
  EXPECT_EQ(2, res.time());
  EXPECT_EQ(20, res.type());
  EXPECT_TRUE(sim.hasNextEvent());
  EXPECT_EQ(1, sim.eventCount());
  ASSERT_EQ(2, sim.time());

  // Check third event
  ASSERT_NO_THROW(res = sim.step());
  EXPECT_EQ(3, res.time());
  EXPECT_EQ(30, res.type());
  EXPECT_FALSE(sim.hasNextEvent());
  EXPECT_EQ(0, sim.eventCount());
  ASSERT_EQ(3, sim.time());

  // Step with no events
  ASSERT_THROW(res = sim.step(), std::runtime_error);
  EXPECT_FALSE(sim.hasNextEvent());
  EXPECT_EQ(0, sim.eventCount());
  ASSERT_EQ(3, sim.time());
  ASSERT_EQ(SimEngineState::Running, sim.state());

  // Check causality violation
  sim.insertEvent(Event{1, 40});
  ASSERT_THROW(res = sim.step(), CausalityException);
  EXPECT_FALSE(sim.hasNextEvent());
  EXPECT_EQ(0, sim.eventCount());
  ASSERT_EQ(3, sim.time());
  ASSERT_EQ(SimEngineState::Running, sim.state());
}

TEST(testSimEngine, step)
{
  FAIL() << "Not implemented";
}
