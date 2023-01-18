#ifndef __DES_EVENTQUEUE_H__
#define __DES_EVENTQUEUE_H__

#include "DESCommon.h"
#include "Event.h"
#include <queue>
#include <vector>
#include <functional>

namespace des
{
/** @addtogroup Core
* @{
*/

/** @brief  Queue of events sorted by time */
class EventQueue
{
public:
  EventQueue();
  ~EventQueue();

  /**
   * @brief  Insert an event into the queue
   * @param e  Event to insert
   */
  inline void insert(const Event& e)
  { _queue.push(e); }

  /**
   * @brief  Insert an event into the queue
   * @param e  Event to insert
   */
  inline void insert(Event&& e)
  { _queue.push(std::move(e)); }

  /**
   * @brief  Insert an event with the given parameters into the queue
   * 
   * @param evtTime  Event time
   * @param evtType  Event type
   * @param evtTag  Event tag
   */
  inline void insert(const SimTime evtTime, const EventType evtType, const EventTag evtTag = 0)
  { _queue.emplace(evtTime, evtType, evtTag); }

  /**
   * @brief  Get the next event from the queue
   *
   *  Event is removed from the queue
   *
   * @return  Next occurring event
   * @throws  std::runtime_error if queue is empty
   */
  Event getNext();

  /**
   * @brief  Peek at the next event in the queue
   *
   *  Event is not removed from the queue
   *
   * @return  Next occurring event
   * @throws std::runtime_error if queue is empty
   */
  const Event& peekNext() const;

  /** @return  True if queue is empty, false otherwise */
  inline bool empty() const noexcept
  { return _queue.empty(); }

  /** @return  Number of events in the queue */
  inline size_t size() const noexcept
  { return _queue.size(); }

private:
  /** @brief  Queue sorting comparator */
  static std::function<bool(const Event&, const Event&)> QueueSorter;

  std::priority_queue<Event, std::vector<Event>, decltype(QueueSorter)> _queue;   ///< Event queue
};

/** @} */
} // End namespace

#endif
