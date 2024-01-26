#ifndef __BANK_H__
#define __BANK_H__

#include "DESCommon.h"
#include "core/SimEngine.h"
#include "core/Event.h"
#include "core/EventHandler.h"

#include "Teller.h"
#include "Customer.h"

#include <queue>
#include <memory>
#include <string>
#include <vector>
#include <ostream>
#include <functional>
#include <random>

typedef struct
{
  // Number of tellers
  int numTellers = 2;

  // Customer interarrival time range
  des::SimTime minCustomerInterarrivalTime = 1;
  des::SimTime maxCustomerInterarrivalTime = 15;

  // Transaction time range
  des::SimTime minTransactionTime = 5;
  des::SimTime maxTransactionTime = 30;
} BankParameters;

class Bank : public des::EventHandler
{
public:
  static constexpr des::EventType EVT_START_SIM = 0;
  static constexpr des::EventType EVT_STOP_SIM = 1;
  static constexpr des::EventType EVT_TRANSACTION_FINISH = 2;
  static constexpr des::EventType EVT_ORIGINATE_CUSTOMER = 3;

public:
  Bank(des::SimEngine& sim, const BankParameters& params);
  ~Bank();

  void handleEvent(des::SimEngine& sim, const des::Event& evt) override;
  void initialize(des::SimEngine& sim) override;
  void finalize(des::SimEngine& sim) override;

  void setLoggerCallback(std::function<void(const des::Event&, const std::string&)> func);
  void logResults(std::ostream& out);

private:
  void handleOriginateCustomer(des::SimEngine& sim, const des::Event& evt);
  void handleTransactionFinish(des::SimEngine& sim, const des::Event& evt);

  Teller* getAvailableTeller();
  Teller* getTellerById(int tellerId);
  void startTransaction(des::SimEngine& sim, const des::Event& evt);

  void log(const des::Event& evt, const std::string& message);

  des::SimTime getCustomerInterarrivalTime() const;
  des::SimTime getTransactionTime() const;

private:
  std::vector<Teller> _tellers;
  std::queue<Customer*> _customerQueue;

  std::vector<Customer*> _completedCustomers;
  std::vector<std::unique_ptr<Customer>> _allCustomers;

  std::function<void(const des::Event&, const std::string&)> _loggerCallback;

  BankParameters _params;

  static std::default_random_engine _rng;
};

#endif
