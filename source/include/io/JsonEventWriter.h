#ifndef __DES_JSONEVENTWRITER_H__
#define __DES_JSONEVENTWRITER_H__

#include "DESCommon.h"
#include "core/Event.h"
#include "io/EventWriter.h"
#include <ostream>

namespace des
{
/** @addtogroup IO
* @{
*/

/** @brief  Class for writing events to JSON format */
class JsonEventWriter : public EventWriter
{
public:
  /**
  * @param buffer  Pointer to stream buffer to write to
  */
  JsonEventWriter(std::streambuf* buffer);

  /**
  * @param str  Stream to write to
  */
  JsonEventWriter(std::ostream& str);

  ~JsonEventWriter();

  void write(const Event& e) override;
};

/** @} */
} // End namespace

#endif
