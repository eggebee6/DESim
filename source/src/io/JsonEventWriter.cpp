#include "core/Event.h"
#include "io/JsonEventWriter.h"
#include "nlohmann/json.hpp"
#include <ostream>

namespace des
{

JsonEventWriter::JsonEventWriter(std::streambuf* buffer) :
  EventWriter{buffer}
{
  if(!buffer)
  {
    throw std::invalid_argument("Buffer is null");
  }
}

JsonEventWriter::JsonEventWriter(std::ostream& str) :
  EventWriter{str}
{
}

JsonEventWriter::~JsonEventWriter()
{
}

void JsonEventWriter::write(const Event& e)
{
  nlohmann::json obj{};

  // Create JSON object
  try
  {
    obj["time"] = e.time();
    obj["type"] = e.type();
    obj["tag"] = e.tag();
  }
  catch(const std::exception& ex)
  {
    throw EventWriteException(ex.what());
  }

  // Write JSON object to stream
  try
  {
    _out << obj;
  }
  catch(const std::exception& ex)
  {
    throw EventWriteException(ex.what());
  }

  // Check stream state
  if(!_out.good())
  {
    throw EventWriteException{"Stream not good after write"};
  }
}

} // End namespace
