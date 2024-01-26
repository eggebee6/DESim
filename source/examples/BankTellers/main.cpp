#include <iostream>
#include <string>
#include <fstream>

#include "DESCommon.h"
#include "io/JsonEventWriter.h"

#include "Bank.h"

int main(int argc, char* argv[])
{
  des::SimEngine sim{};

  // Set simulation parameters
  BankParameters bankParams{};
  bankParams.numTellers = 4;
  bankParams.minCustomerInterarrivalTime = 0;
  bankParams.maxCustomerInterarrivalTime = 10;
  bankParams.minTransactionTime = 5;
  bankParams.maxTransactionTime = 30;

  const des::SimTime stopTime = 720;

  // Create bank object
  Bank bank{sim, bankParams};

  bank.setLoggerCallback([] (const des::Event& evt, const std::string& msg)
  {
    std::cout << "Time " << evt.time() << ": " << msg << std::endl;
  });

  // Initialize simulation
  sim.initialize();

  // Run simulation
  std::cout << "--- Simulation start ---" << std::endl;
  try
  {
    // Open file for writing events
    const std::string eventFileName = "bankEvents.json";
    std::ofstream file{eventFileName};

    if(!file.is_open())
    {
      throw std::runtime_error("Failed to open file");
    }

    // Initialize event writer
    des::JsonEventWriter eventWriter{file};
    std::cout << "Writing events to " << eventFileName << std::endl;

    // Step simulation while events are available and stop time has not been reached
    while((sim.hasNextEvent()) && (sim.time() <= stopTime))
    {
      des::Event evt = sim.step();
      eventWriter.write(evt);
    }

    file.close();
  }
  catch(const std::exception& ex)
  {
    std::cerr << "Simultaion exception: " << ex.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // Finalize
  sim.finalize();
  std::cout << "--- Simulation end ---" << std::endl;

  // Print results to console
  bank.logResults(std::cout);

  // Done
  std::exit(EXIT_SUCCESS);
}
