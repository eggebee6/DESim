#ifndef __DES_SIMENGINE_H__
#define __DES_SIMENGINE_H__

#include "DESCommon.h"
#include "Event.h"
#include "EventQueue.h"
#include "EventHandler.h"
#include <set>
#include <map>

namespace des
{
/** @addtogroup Core
* @{
*/

enum class SimEngineState
{
  Uninitialized,    ///< Simulation has not been initialized
  Running,          ///< Simulation is processing events
  Finalized,        ///< Simulation has been finalized
  Error             ///< Simulation has encountered an error
};

/** @brief Simulation engine */
class SimEngine
{
public:
  SimEngine();
  ~SimEngine();

  /**
   * @brief  Subscribe an event handler to all events
   * 
   * If the handler is subscribed to any specific event types, those subscriptions will be removed
   * Handlers may not be subscribed after the simulation has been initialized
   * 
   * @param handler  Handler to subscribe
   * @throws std::runtime_error if simulation is not in the Uninitialized state
   * @throws std::invalid_argument if handler is null
   */
  void subscribe(EventHandler* handler);

  /**
   * @brief  Subscribe an event handler to a specific event type
   * 
   * If the handler is subscribed to all events, that subscrption will be removed
   * Handlers may not be subscribed after the simulation has been initialized
   * 
   * @param handler  Handler to subscribe
   * @param evtType  Event type to subscribe to
   * @throws std::runtime_error if simulation is not in the Uninitialized state
   * @throws std::invalid_argument if handler is null
   */
  void subscribe(EventHandler* handler, EventType evtType);

  /**
   * @brief  Initialize the simulation
   * 
   * Calls initialize on all subscribed event handlers
   * Simulation will be in the Running state after calling initialize
   * 
   * @throws std::runtime_error if simulation is not in the Uninitialized state
   */
  void initialize();

  /**
   * @brief  Advance the simulation one step
   * 
   * @return Event processed on the step
   * @throws std::runtime_error if simulation is not in the Running state
   * @throws std::runtime_error if the schedule is empty
   * @throws CausalityException if the event being processed occurs before the current simulation time
   */
  Event step();

  /**
   * @brief  Finalize the simulation
   * 
   * Simulation will be in the Finalized state after calling finalize
   * 
   * @throws std::runtime_error if simulation is not in the Running state
   */
  void finalize();

  /**
   * @brief  Insert an event into the simulation schedule
   * 
   * @param evt  Event to insert
   */
  inline void insertEvent(const Event& evt)
  { _schedule.insert(evt); }

  /**
   * @brief  Insert an event into the simulation schedule
   * 
   * @param evt  Event to insert
   */
  inline void insertEvent(Event&& evt)
  { _schedule.insert(std::move(evt)); }

  /** @return True if simulation has an event in the schedule, false otherwise */
  inline bool hasNextEvent() const noexcept
  { return !_schedule.empty(); }

  /** @return Number of events in the schedule */
  inline size_t eventCount() const noexcept
  { return _schedule.size(); }

  /**
   * @brief  Get the simulation time of the most recently processed event
   * 
   * Simulation time will be updated after an event is done being processed
   * 
   * @return Simulation time of most recently processed event 
   */
  inline SimTime time() const noexcept
  { return _time; }

  /** @return Simulation state */
  inline SimEngineState state() const noexcept
  { return _state; }

  /** @return Set of all subscribed handlers */
  std::set<EventHandler*> getAllHandlers() const;

private:
  SimTime _time;            ///< Simulation time of most recently processed event
  SimEngineState _state;    ///< Simulation state

  EventQueue _schedule;     ///< Simulation event schedule

  std::set<EventHandler*> _globalHandlers;                      ///< Handlers subscribed to all events
  std::map<EventType, std::set<EventHandler*>> _typeHandlers;   ///< Event handlers subscribed to specific event types
};

/**
* @brief  Causality exception class
*
*  To be thrown in case of a causality violation
*/
class CausalityException : public std::runtime_error
{
public:
  CausalityException(const Event& evt, const char* what_arg = "") :
    std::runtime_error(what_arg),
    _evt{evt}
  {}

  inline const Event& event() const noexcept
  { return _evt; }

private:
  Event _evt;
};

/** @} */
} // End namespace

#endif
