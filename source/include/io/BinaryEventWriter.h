#ifndef __DES_BINARYEVENTWRITER_H__
#define __DES_BINARYEVENTWRITER_H__

#include "DESCommon.h"
#include "core/Event.h"
#include "io/EventWriter.h"
#include <ostream>

namespace des
{
/** @addtogroup IO
* @{
*/

/** @brief  Class for writing events to binary format */
class BinaryEventWriter : public EventWriter
{
public:
  /**
  * @param buffer  Pointer to stream buffer to write to
  */
  BinaryEventWriter(std::streambuf* buffer);

  /**
  * @param str  Stream to write to
  */
  BinaryEventWriter(std::ostream& str);

  ~BinaryEventWriter();

  void write(const Event& e) override;
};

/** @} */
} // End namespace

#endif
