#ifndef __DES_EVENTWRITER_H__
#define __DES_EVENTWRITER_H__

#include "DESCommon.h"
#include "core/Event.h"
#include <ostream>
#include <stdexcept>

namespace des
{
/** @addtogroup IO
* @{
*/

/** @brief  Base class for writing events to a stream */
class EventWriter
{
public:
  virtual ~EventWriter() {}

  /**
  * @brief  Write the given event to the stream
  * @param e  Event to write
  * @throws  EventWriteException if write fails
  */
  virtual void write(const Event& e) = 0;

  /** @return  Reference to internal stream */
  inline std::ostream& stream() noexcept
  { return _out; }

protected:
  /**
  * @param buffer  Pointer to stream buffer
  */
  explicit EventWriter(std::streambuf* buffer) :
    _out{buffer} {}

  /**
  * @param str  Stream to write to
  */
  explicit EventWriter(std::ostream& str) :
    _out{str.rdbuf()} {}
      
  std::ostream _out;    ///< Output stream for writing to buffer
};


/** @brief  Event write exception class */
class EventWriteException : public std::runtime_error
{
public:
  /**
  * @param what_arg  String with information about the exception
  */
  EventWriteException(const char* what_arg = "") :
    std::runtime_error(what_arg)
  { };
};

/** @} */
} // End namespace

#endif
