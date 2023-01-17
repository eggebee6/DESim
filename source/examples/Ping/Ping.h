#include "DESCommon.h"
#include "core/Event.h"
#include "core/EventHandler.h"
#include "core/SimEngine.h"

#include <functional>

class Ping : public des::EventHandler
{
public:
  static const des::EventType PingType = 0;
  static const des::EventType PongType = 1;

  static const std::string& MessageName(const des::EventType evtType);

public:
  Ping(const std::string& name, std::function<void(des::SimEngine&, const des::Event&)> handler);
  ~Ping();

  void initialize(des::SimEngine& sim) override;
  void finalize(des::SimEngine& sim) override;
  void handleEvent(des::SimEngine& sim, const des::Event& evt) override;

private:
  std::string _name;
  std::function<void(des::SimEngine&, const des::Event&)> _handler;
};
