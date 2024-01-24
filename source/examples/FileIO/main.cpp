#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>

#include "DESCommon.h"
#include "io/BinaryEventReader.h"
#include "io/BinaryEventWriter.h"

void writeEvents(des::EventWriter& writer, const std::vector<des::Event>& events);
std::vector<des::Event> readEvents(des::EventReader& reader);

int main(int argc, char* argv[])
{
  // Initialize file names
  const std::string binaryFileName = "./events.dat";

  // Fill a vector with several events
  std::vector<des::Event> eventList;
  eventList.push_back(des::Event{1, 10, 100});
  eventList.push_back(des::Event{2, 20, 200});
  eventList.push_back(des::Event{3, 30, 300});

  // Write file
  try
  {
    // Create file stream and event writer
    std::ofstream file{binaryFileName, std::ios::binary};
    des::BinaryEventWriter writer{file};

    std::cout << "Writing events to " << binaryFileName << std::endl;

    // Write events from vector
    writeEvents(writer, eventList);
    file.close();

    std::cout << "Done" << std::endl;
  }
  catch(const std::exception& ex)
  {
    std::cerr << "Failed to write events: " << ex.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  // Read file
  try
  {
    // Create file stream and event reader
    std::ifstream file{binaryFileName, std::ios::binary};
    des::BinaryEventReader reader{file};

    std::cout << "Reading events from " << binaryFileName << std::endl;

    // Read vector of events
    auto eventsFromFile = readEvents(reader);
    file.close();

    std::cout << "Read " << eventsFromFile.size() << " events" << std::endl;
    
    // Print events to console
    for(const auto& evt : eventsFromFile)
    {
      std::cout << "Event time " << evt.time();
      std::cout << ", type " << evt.type();
      std::cout << ", tag " << evt.tag() << std::endl;
    }
  }
  catch(const std::exception& ex)
  {
    std::cerr << "Failed to read events: " << ex.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::exit(EXIT_SUCCESS);
}

void writeEvents(des::EventWriter& writer, const std::vector<des::Event>& events)
{
  // Write all events from vector
  for(const auto& evt : events)
  {
    writer.write(evt);
  }
}

std::vector<des::Event> readEvents(des::EventReader& reader)
{
  std::vector<des::Event> events;
  auto& stream = reader.stream();

  // Read events into vector until end of file is reached
  while(!stream.eof())
  {
    events.push_back(reader.read());

    // Peek for EOF
    stream.peek();
  }

  return events;
}
