#include "DESCommon.h"
#include "core/Event.h"
#include "core/EventQueue.h"
#include <stdexcept>

namespace des
{

// Sort events such that events with a smaller time are further ahead in the queue
std::function<bool(const Event&, const Event&)> EventQueue::QueueSorter
  { [] (const Event& lhs, const Event& rhs) { return (rhs.time() < lhs.time()); } };

EventQueue::EventQueue() :
  _queue{EventQueue::QueueSorter}
{
}

EventQueue::~EventQueue()
{
}

Event EventQueue::getNext()
{
  if(_queue.empty())
  {
    throw std::runtime_error("Queue is empty");
  }
  
  Event e = std::move(const_cast<Event&>(_queue.top()));
  _queue.pop();

  return e;
}

const Event& EventQueue::peekNext() const
{
  if(_queue.empty())
  {
    throw std::runtime_error("Queue is empty");
  }

  return _queue.top();
}

} // End namespace
