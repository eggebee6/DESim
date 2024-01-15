#include "DESCommon.h"
#include "core/Event.h"

namespace des
{

Event::Event(const SimTime t, const EventType n, const EventTag g) noexcept :
  _time{t},
  _type{n},
  _tag{g}
{
}

Event::~Event()
{
}

} // End namespace
