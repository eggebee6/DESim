#ifndef __DES_EVENTHANDLER_H__
#define __DES_EVENTHANDLER_H__

#include "DESCommon.h"
#include "Event.h"

namespace des
{
/** @addtogroup Core
* @{
*/

class SimEngine;

/** @brief EventHandler interface */
class EventHandler
{
public:
  virtual ~EventHandler()
  {}

  /**
   * @brief  Handle an event being processed by a simulation
   * 
   * @param sim  Simulation processing the event
   * @param evt  Event to handle
   */
  virtual void handleEvent(SimEngine& sim, const Event& evt) = 0;

  /**
   * @brief  Initialize the event handler
   * 
   * Initialize will be called before any calls to handleEvent occur
   * 
   * @param sim  Simulation initializing the event handler
   */
  virtual void initialize(SimEngine& sim) = 0;

  /**
   * @brief  Finalize the event handler
   * 
   * No calls to handleEvent will occur after finalize is called
   * 
   * @param sim  Simulation finalizing the event handler
   */
  virtual void finalize(SimEngine& sim) = 0;

protected:
  EventHandler()
  {}
};

/** @} */
} // End namespace

#endif
