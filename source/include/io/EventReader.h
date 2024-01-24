#ifndef __DES_EVENTREADER_H__
#define __DES_EVENTREADER_H__

#include "DESCommon.h"
#include "core/Event.h"
#include <istream>
#include <stdexcept>

namespace des
{
/** @addtogroup IO
* @{
*/

/** @brief  Base class for reading events from a stream */
class EventReader
{
public:
  virtual ~EventReader() {}

  /**
  * @brief  Read an event from the stream
  * @return  Read event
  * @throws  EventReadException if read fails
  */
  virtual Event read() = 0;

  /** @return  Reference to internal stream */
  inline std::istream& stream() noexcept
  { return _in; }

protected:
  /**
  * @param buffer  Pointer to stream buffer
  */
  explicit EventReader(std::streambuf* buffer) :
    _in{buffer} {}

  /**
  * @param str  Stream to read from
  */
  explicit EventReader(std::istream& str) :
    _in{str.rdbuf()} {}
    
  /**
  * @brief  Create an event with the given parameters
  * @param t  Event occurence time
  * @param n  Event type
  * @param g  Event tag
  * @return  Constructed event
  */
  inline static Event CreateEvent(SimTime t, EventType n, EventTag g)
  { return Event{t, n, g}; }

  std::istream _in;     ///< Input stream for reading from buffer
};


/** @brief  Event read exception class */
class EventReadException : public std::runtime_error
{
public:
  /**
  * @param what_arg  String with information about the exception
  */
  EventReadException(const char* what_arg = "") :
    std::runtime_error(what_arg)
  { };
};

/** @} */
} // End namespace

#endif
