#ifndef __DES_EVENT_H__
#define __DES_EVENT_H__

#include "DESCommon.h"

namespace des
{
/** @addtogroup Core
* @{
*/

/** @brief Event class */
class Event
{
public:
  /**
   * @brief  Construct event from time, type, and tag
   * @param evtTime  Event occurrence time
   * @param evtType  Event type
   * @param evtTag  Event tag (optional, default = 0)
   */
  Event(const SimTime evtTime, const EventType evtType, const EventTag evtTag = 0) noexcept;

  /** @brief  Default copy constructor */
  Event(const Event&) = default;

  /** @brief  Default move constructor */
  Event(Event&&) = default;

  /** @brief  Default copy assignment operator */
  Event& operator = (const Event&) = default;

  /** @brief  Default move assignment operator */
  Event& operator = (Event&&) = default;

  ~Event();

  /** @return  Event occurrence time */
  inline SimTime time() const noexcept
  { return _time; }

  /** @return  Event type */
  inline EventType type() const noexcept
  { return _type; }

  /** @return  Event tag */
  inline EventTag tag() const noexcept
  { return _tag; }

private:
  SimTime _time;      ///< Event occurrence time
  EventType _type;    ///< Event type
  EventTag _tag;      ///< Event tag
};

/** @} */
} // End namespace

#endif
