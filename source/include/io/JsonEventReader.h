#ifndef __DES_JSONEVENTREADER_H__
#define __DES_JSONEVENTREADER_H__

#include "DESCommon.h"
#include "core/Event.h"
#include "io/EventReader.h"
#include <istream>

namespace des
{
/** @addtogroup IO
* @{
*/

/** @brief  Class for reading events from JSON format */
class JsonEventReader : public EventReader
{
public:
  /**
  * @param buffer  Pointer to stream buffer to read from
  */
  JsonEventReader(std::streambuf* buffer);

  /**
  * @param str  Stream to read from
  */
  JsonEventReader(std::istream& str);

  ~JsonEventReader();

  Event read() override;
};

/** @} */
} // End namespace

#endif
