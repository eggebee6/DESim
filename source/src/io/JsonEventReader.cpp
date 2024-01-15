#include "core/Event.h"
#include "io/JsonEventReader.h"
#include "nlohmann/json.hpp"
#include <istream>
#include <stdexcept>

namespace des
{

JsonEventReader::JsonEventReader(std::streambuf* buffer) :
  EventReader{buffer}
{
  if(!buffer)
  {
    throw std::invalid_argument("Buffer is null");
  }
}

JsonEventReader::JsonEventReader(std::istream& str) :
  EventReader{str}
{
}

JsonEventReader::~JsonEventReader()
{
}

Event JsonEventReader::read()
{
  nlohmann::json obj{};

  SimTime t;
  EventType n;
  EventTag g;

  // Read JSON object from stream
  try
  {
    _in >> obj;
  }
  catch(const nlohmann::json::parse_error& ex)
  {
    throw EventReadException{ex.what()};
  }

  // Check stream state
  if(!_in.good())
  {
    throw EventReadException{"Stream not good after read"};
  }

  const auto end = obj.end();

  // Get values from JSON object
  auto it = obj.find("time");
  t = (it != end) ? *it : throw EventReadException{"JSON object missing time"};

  it = obj.find("type");
  n = (it != end) ? *it : throw EventReadException{"JSON object missing type"};

  it = obj.find("tag");
  g = (it != end) ? *it : throw EventReadException{"JSON object missing tag"};

  return CreateEvent(t, n, g);
}

} // End namespace
