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
  SimEngine sim{};

  MockHandler h1{};
  MockHandler h2{};
  
  sim.subscribe(&h1);
  sim.subscribe(&h2, 1);
  sim.subscribe(&h2, 2);

  // Each subscribed handlers should receive only one call to initialize
  EXPECT_CALL(h1, initialize(Ref(sim))).Times(1);
  EXPECT_CALL(h2, initialize(Ref(sim))).Times(1);

  ASSERT_NO_THROW(sim.initialize());
  ASSERT_EQ(SimEngineState::Running, sim.state());

  EXPECT_CALL(h1, initialize).Times(0);
  EXPECT_CALL(h2, initialize).Times(0);
  ASSERT_THROW(sim.initialize(), std::runtime_error);
}

TEST(testSimEngine, finalize)
{
  SimEngine sim{};

  MockHandler h1{};
  MockHandler h2{};
  
  sim.subscribe(&h1);
  sim.subscribe(&h2, 1);
  sim.subscribe(&h2, 2);

  EXPECT_CALL(h1, initialize(Ref(sim))).Times(1);
  EXPECT_CALL(h2, initialize(Ref(sim))).Times(1);
  sim.initialize();

  // Each subscribed handlers should receive only one call to finalize
  EXPECT_CALL(h1, finalize(Ref(sim))).Times(1);
  EXPECT_CALL(h2, finalize(Ref(sim))).Times(1);

  ASSERT_NO_THROW(sim.finalize());
  ASSERT_EQ(SimEngineState::Finalized, sim.state());

  EXPECT_CALL(h1, finalize).Times(0);
  EXPECT_CALL(h2, finalize).Times(0);
  ASSERT_THROW(sim.finalize(), std::runtime_error);
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

TEST(testSimEngine, subscriptions)
{
  SimEngine sim{};

  MockHandler h1{};
  MockHandler h2{};
  MockHandler h3{};

  decltype(sim.getAllHandlers()) handlers{};

  ASSERT_NO_THROW(handlers = sim.getAllHandlers());
  EXPECT_EQ(0, handlers.size());

  // Subscribe h1 to all events
  ASSERT_NO_THROW(sim.subscribe(&h1));
  ASSERT_NO_THROW(handlers = sim.getAllHandlers());
  EXPECT_EQ(1, handlers.size());
  ASSERT_TRUE(handlers.count(&h1) > 0);

  // Subscribe h1 again
  ASSERT_NO_THROW(sim.subscribe(&h1));
  ASSERT_NO_THROW(handlers = sim.getAllHandlers());
  EXPECT_EQ(1, handlers.size());
  ASSERT_TRUE(handlers.count(&h1) > 0);

  // Subscribe h2 to events 1 and 2
  ASSERT_NO_THROW(sim.subscribe(&h2, 1));
  ASSERT_NO_THROW(handlers = sim.getAllHandlers());
  EXPECT_EQ(2, handlers.size());
  ASSERT_TRUE(handlers.count(&h1) > 0);
  ASSERT_TRUE(handlers.count(&h2) > 0);

  ASSERT_NO_THROW(sim.subscribe(&h2, 2));
  ASSERT_NO_THROW(handlers = sim.getAllHandlers());
  EXPECT_EQ(2, handlers.size());
  ASSERT_TRUE(handlers.count(&h1) > 0);
  ASSERT_TRUE(handlers.count(&h2) > 0);

  // Subscribe h2 to event 2 again
  ASSERT_NO_THROW(sim.subscribe(&h2, 2));
  ASSERT_NO_THROW(handlers = sim.getAllHandlers());
  EXPECT_EQ(2, handlers.size());
  ASSERT_TRUE(handlers.count(&h1) > 0);
  ASSERT_TRUE(handlers.count(&h2) > 0);

  // Subscribe h3 to all events
  ASSERT_NO_THROW(sim.subscribe(&h3));
  ASSERT_NO_THROW(handlers = sim.getAllHandlers());
  EXPECT_EQ(3, handlers.size());
  ASSERT_TRUE(handlers.count(&h1) > 0);
  ASSERT_TRUE(handlers.count(&h2) > 0);
  ASSERT_TRUE(handlers.count(&h3) > 0);

  // Resubscribe h3 to event 3
  ASSERT_NO_THROW(sim.subscribe(&h3, 3));
  ASSERT_NO_THROW(handlers = sim.getAllHandlers());
  EXPECT_EQ(3, handlers.size());
  ASSERT_TRUE(handlers.count(&h1) > 0);
  ASSERT_TRUE(handlers.count(&h2) > 0);
  ASSERT_TRUE(handlers.count(&h3) > 0);

  // Resubscribe h3 to all events
  ASSERT_NO_THROW(sim.subscribe(&h3));
  ASSERT_NO_THROW(handlers = sim.getAllHandlers());
  EXPECT_EQ(3, handlers.size());
  ASSERT_TRUE(handlers.count(&h1) > 0);
  ASSERT_TRUE(handlers.count(&h2) > 0);
  ASSERT_TRUE(handlers.count(&h3) > 0);
}

TEST(testSimEngine, step)
{
  FAIL() << "Not implemented";
}
