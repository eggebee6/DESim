#include "Ping.h"
#include <iostream>
#include <map>
#include <stdexcept>

static const std::map<des::EventType, std::string> eventNames
{
  {Ping::PingType, "Ping"},
  {Ping::PongType, "Pong"}
};

const std::string& Ping::MessageName(const des::EventType evtType)
{
  auto it = eventNames.find(evtType);
  if(it != eventNames.cend())
  {
    return it->second;
  }
  else
  {
    throw std::invalid_argument("Unknown event type");
  }
}

Ping::Ping(const std::string& name, std::function<void(des::SimEngine&, const des::Event&)> handler) :
  _name{name}
{
  if(!handler)
  {
    throw std::invalid_argument("Invalid handler");
  }

  _handler = handler;
}

Ping::~Ping()
{
}

void Ping::initialize(des::SimEngine& sim)
{
  std::cout << "Initializing " << _name << std::endl;
}

void Ping::finalize(des::SimEngine& sim)
{
  std::cout << "Finalizing " << _name << std::endl;
}

void Ping::handleEvent(des::SimEngine& sim, const des::Event& evt)
{
  _handler(sim, evt);
}
