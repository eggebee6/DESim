#include <iostream>
#include "Ping.h"

int main(int argc, char* argv[])
{
  // Thing One responds to Ping with Pong
  Ping thingOne{"Thing One", [] (des::SimEngine& sim, const des::Event& evt)
  {
    std::cout << "Thing One received " << Ping::MessageName(evt.type()) << std::endl;
    if(evt.type() == Ping::PingType)
    {
      des::SimTime t = sim.time() + 1;
      std::cout << "Scheduling " << Ping::MessageName(Ping::PongType) << " for time " << t << std::endl;
      sim.insertEvent(des::Event{t, Ping::PongType});
    }
  }};

  // Thing Two responds to Pong with Ping
  Ping thingTwo{"Thing Two", [] (des::SimEngine& sim, const des::Event& evt)
  {
    std::cout << "Thing Two received " << Ping::MessageName(evt.type()) << std::endl;
    if(evt.type() == Ping::PongType)
    {
      des::SimTime t = sim.time() + 2;
      std::cout << "Scheduling " << Ping::MessageName(Ping::PingType) << " for time " << t << std::endl;
      sim.insertEvent(des::Event{t, Ping::PingType});
    }
  }};

  des::SimEngine sim{};

  try
  {
    // Subscribe Thing One to all events, Thing Two to Pong event only
    sim.subscribe(&thingOne);
    sim.subscribe(&thingTwo, Ping::PongType);
    
    // Set up simulation
    sim.initialize();

    const des::SimTime stopTime = 10;
    sim.insertEvent(des::Event{0, Ping::PingType});

    // Run simulation
    std::cout << "Running simulation..." << std::endl;
    std::cout << std::endl;
    while((sim.hasNextEvent()) && (sim.time() <= stopTime))
    {
      des::Event evt = sim.step();
      std::cout << "*** Processed event type " << evt.type() << " at time " << evt.time() << std::endl;
      std::cout << std::endl;
    }

    // Finalize
    sim.finalize();
  }
  catch(const des::CausalityException& ex)
  {
    std::cerr << "Event type " << ex.event().type() << " at time " << ex.event().type()
      << " violates causality, simulation time is " << sim.time() << std::endl;
  }
  catch(const std::exception& ex)
  {
    std::cerr << "ERROR: " << ex.what() << std::endl;
  }

  std::cout << "Done" << std::endl;
}
