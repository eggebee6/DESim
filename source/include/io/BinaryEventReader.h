#ifndef __DES_BINARYEVENTREADER_H__
#define __DES_BINARYEVENTREADER_H__

#include "DESCommon.h"
#include "core/Event.h"
#include "io/EventReader.h"
#include <istream>

namespace des
{
/** @addtogroup IO
* @{
*/

/** @brief  Class for reading events from binary format */
class BinaryEventReader : public EventReader
{
public:
  /**
  * @param buffer  Pointer to stream buffer to read from
  */
  BinaryEventReader(std::streambuf* buffer);

  /**
  * @param str  Stream to read from
  */
  BinaryEventReader(std::istream& str);

  ~BinaryEventReader();

  Event read() override;
};

/** @} */
} // End namespace

#endif
