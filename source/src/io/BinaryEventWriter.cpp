#include "core/Event.h"
#include "io/BinaryEventWriter.h"
#include <ostream>
#include <stdexcept>

namespace des
{

BinaryEventWriter::BinaryEventWriter(std::streambuf* buffer) :
  EventWriter{buffer}
{
  if(!buffer)
  {
    throw std::invalid_argument("Buffer is null");
  }
}

BinaryEventWriter::BinaryEventWriter(std::ostream& str) :
  EventWriter{str}
{
}

BinaryEventWriter::~BinaryEventWriter()
{
}

void BinaryEventWriter::write(const Event& e)
{
  static constexpr std::streamsize EVT_SIZE =
    sizeof(SimTime) + sizeof(EventType) + sizeof(EventTag);

  char buffer[EVT_SIZE];
  char* pBuffer = buffer;

  *(SimTime*)pBuffer = e.time();
  pBuffer += sizeof(SimTime);

  *(EventType*)pBuffer = e.type();
  pBuffer += sizeof(EventType);

  *(EventTag*)pBuffer = e.tag();
  pBuffer += sizeof(EventTag);

  _out.write(buffer, EVT_SIZE);
  if(!_out.good())
  {
    throw EventWriteException{"Stream not good after write"};
  }
}

} // End namespace
