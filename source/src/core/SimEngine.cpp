#include "DESCommon.h"
#include "core/Event.h"
#include "core/EventQueue.h"
#include "core/SimEngine.h"
#include <stdexcept>
#include <cassert>
#include <set>
#include <map>

namespace des
{

SimEngine::SimEngine() :
  _time{0},
  _state{SimEngineState::Uninitialized},
  _schedule{EventQueue{}},
  _globalHandlers{std::set<EventHandler*>{}},
  _typeHandlers{std::map<EventType, std::set<EventHandler*>>{}}
{
}

SimEngine::~SimEngine()
{
}

void SimEngine::subscribe(EventHandler* handler)
{
  if(!handler)
  {
    throw std::invalid_argument("Handler is null");
  }

  // Don't double-subscribe handler
  for(auto& it : _typeHandlers)
  {
    it.second.erase(handler);
  }

  _globalHandlers.insert(handler);
}

void SimEngine::subscribe(EventHandler* handler, EventType evtType)
{
  if(!handler)
  {
    throw std::invalid_argument("Handler is null");
  }

  // Don't double-subscribe handler
  _globalHandlers.erase(handler);

  auto& handlers = _typeHandlers[evtType];
  handlers.insert(handler);
}

void SimEngine::initialize()
{
  if(_state != SimEngineState::Uninitialized)
  {
    throw std::runtime_error("Simulation is not Uninitialized");
  }

  try
  {
    const auto& handlers = getAllHandlers();
    for(auto handler : handlers)
    {
      assert(handler);
      handler->finalize(*this);
    }

    _state = SimEngineState::Running;
    return;
  }
  catch(...)
  {
    _state = SimEngineState::Error;
    throw;
  }
}

Event SimEngine::step()
{
  if(_state != SimEngineState::Running)
  {
    throw std::runtime_error("Simulation is not Running");
  }

  if(_schedule.empty())
  {
    throw std::runtime_error("Schedule is empty");
  }
  
  try
  {
    Event evt = _schedule.getNext();

    if(evt.time() < _time)
    {
      throw CausalityException(evt, "Event violates causality");
    }

    // Pass event to all global handlers
    for(auto handler : _globalHandlers)
    {
      assert(handler);
      handler->handleEvent(*this, evt);
    }

    // Pass event to all handlers subscribed to the event type
    auto it = _typeHandlers.find(evt.type());
    if(it != _typeHandlers.cend())
    {
      for(auto handler : it->second)
      {
        assert(handler);
        handler->handleEvent(*this, evt);
      }
    }

    _time = evt.time();
    return evt;
  }
  catch(const CausalityException&)
  {
    throw;
  }
  catch(...)
  {
    _state = SimEngineState::Error;
    throw;
  }
}

void SimEngine::finalize()
{
  if(_state != SimEngineState::Finalized)
  {
    throw std::runtime_error("Simulation is not Finalized");
  }

  try
  {
    const auto& handlers = getAllHandlers();
    for(auto handler : handlers)
    {
      assert(handler);
      handler->finalize(*this);
    }

    _state = SimEngineState::Finalized;
    return;
  }
  catch(...)
  {
    _state = SimEngineState::Error;
    throw;
  }
}

std::set<EventHandler*> SimEngine::getAllHandlers() const
{
  std::set<EventHandler*> handlers{};

  handlers.insert(_globalHandlers.cbegin(), _globalHandlers.cend());

  for(const auto& it : _typeHandlers)
  {
    const auto& typeHandlers = it.second;
    handlers.insert(typeHandlers.cbegin(), typeHandlers.cend());
  }

  return handlers;
}

} // End namespace
