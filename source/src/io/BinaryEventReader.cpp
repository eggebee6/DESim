#include "core/Event.h"
#include "io/BinaryEventReader.h"
#include <istream>
#include <stdexcept>

namespace des
{

BinaryEventReader::BinaryEventReader(std::streambuf* buffer) :
  EventReader{buffer}
{
  if(!buffer)
  {
    throw std::invalid_argument("Buffer is null");
  }
}

BinaryEventReader::BinaryEventReader(std::istream& str) :
  EventReader{str}
{
}

BinaryEventReader::~BinaryEventReader()
{
}

Event BinaryEventReader::read()
{
  static constexpr std::streamsize EVT_SIZE =
    sizeof(SimTime) + sizeof(EventType) + sizeof(EventTag);

  char buffer[EVT_SIZE];
  char* pBuffer = buffer;

  _in.read(buffer, EVT_SIZE);
  if(!_in.good())
  {
    throw EventReadException{"Stream not good after read"};
  }

  SimTime t = *(SimTime*)pBuffer;
  pBuffer += sizeof(SimTime);

  EventType n = *(EventType*)pBuffer;
  pBuffer += sizeof(EventType);
  
  EventTag g = *(EventTag*)pBuffer;
  pBuffer += sizeof(EventTag);

  return CreateEvent(t, n, g);
}

} // End namespace
