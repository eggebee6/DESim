#include "Bank.h"
#include "Teller.h"
#include "Customer.h"

#include <sstream>
#include <ctime>

std::default_random_engine Bank::_rng;

Bank::Bank(des::SimEngine& sim, const BankParameters& params)
{
  // Store parameters
  _params = params;

  // Initialize tellers
  for(int i = 0; i < _params.numTellers; ++i)
  {
    _tellers.push_back(Teller{});
  }

  // Subscribe to all simulation events
  sim.subscribe(this);

  // Seed random number generator
  _rng.seed(std::time(nullptr));
}

Bank::~Bank()
{
}

des::SimTime Bank::getCustomerInterarrivalTime() const
{
  // Get uniform random time between customer arrivals
  std::uniform_int_distribution<int> dist(
    _params.minCustomerInterarrivalTime,
    _params.maxCustomerInterarrivalTime);
  return dist(_rng);
}

des::SimTime Bank::getTransactionTime() const
{
  // Get uniform random transaction duration
  std::uniform_int_distribution<int> dist(
    _params.minTransactionTime,
    _params.maxTransactionTime);
  return dist(_rng);
}

Teller* Bank::getAvailableTeller()
{
  // Return first available teller, or null if all tellers are busy
  for(auto& teller : _tellers)
  {
    if(!teller.isBusy())
    {
      return &(teller);
    }
  }

  return nullptr;
}

Teller* Bank::getTellerById(int tellerId)
{
  // Linear search for teller ID
  for(auto& it : _tellers)
  {
    if(it.id() == tellerId)
    {
      return &(it);
    }
  }

  // Teller ID not found
  return nullptr;
}

void Bank::startTransaction(des::SimEngine& sim, const des::Event& evt)
{
  // Get customer at front of line
  Customer* pCustomer = _customerQueue.front();
  _customerQueue.pop();
  if(!pCustomer)
  {
    throw std::runtime_error("Start transaction but customer at front of queue is null");
  }

  // Get available teller
  Teller* pTeller = getAvailableTeller();
  if(!pTeller)
  {
    throw std::runtime_error("Start transaction but available teller is null");
  }

  // Begin transaction
  pTeller->beginTransaction(evt, pCustomer);
  pCustomer->beginTransaction(evt, pTeller);

  // Schedule transaction finish
  sim.insertEvent(sim.time() + getTransactionTime(), Bank::EVT_TRANSACTION_FINISH, pTeller->id());

  // Print event
  std::stringstream sstr{};
  sstr << "Teller " << pTeller->id() << " starts transaction with customer " << pCustomer->id();
  sstr << " (" << _customerQueue.size() << " customers in line)";
  log(evt, sstr.str());
}

void Bank::handleOriginateCustomer(des::SimEngine& sim, const des::Event& evt)
{
  // Allocate customer object
  _allCustomers.emplace_back(new Customer{});
  const auto& pCustomer = _allCustomers.back();

  // Put customer in line and begin waiting
  _customerQueue.push(pCustomer.get());
  pCustomer->beginWaiting(evt);

  // Check if customer is the only customer in line
  if(_customerQueue.size() == 1)
  {
    // Check for an available teller
    Teller* pTeller = getAvailableTeller();
    if(pTeller != nullptr)
    {
      // Immediately begin transaction
      startTransaction(sim, evt);
    }
  }

  // Schedule next customer arrival
  sim.insertEvent(sim.time() + getCustomerInterarrivalTime(), Bank::EVT_ORIGINATE_CUSTOMER);

  // Print event
  std::stringstream sstr{};
  sstr << "Customer " << pCustomer->id() << " arrival";
  sstr << " (" << _customerQueue.size() << " customers in line)";
  log(evt, sstr.str());
}

void Bank::handleTransactionFinish(des::SimEngine& sim, const des::Event& evt)
{
  // Get teller who originated this event
  Teller* pTeller = getTellerById(evt.tag());
  if(!pTeller)
  {
    throw std::runtime_error("Transaction finish occurred with invalid teller ID");
  }

  // Get teller customer
  Customer* pCustomer = pTeller->activeCustomer();
  if(!pCustomer)
  {
    throw std::runtime_error("Transaction finish occurred but teller has null customer");
  }

  // End transaction
  pTeller->endTransaction(evt);
  pCustomer->endTransaction(evt);

  // Add customer to completed customer list
  _completedCustomers.push_back(pCustomer);

  // Print event
  std::stringstream sstr{};
  sstr << "Teller " << pTeller->id() << " finishes with customer " << pCustomer->id();
  sstr << " (" << _customerQueue.size() << " customers in line)";
  log(evt, sstr.str());

  // Start next transaction if a customer is in line
  if(!_customerQueue.empty())
  {
    startTransaction(sim, evt);
  }
}

void Bank::setLoggerCallback(std::function<void(const des::Event&, const std::string&)> func)
{
  _loggerCallback = func;
}

void Bank::log(const des::Event& evt, const std::string& message)
{
  if(_loggerCallback)
  {
    _loggerCallback(evt, message);
  }
}

void Bank::logResults(std::ostream& out)
{
  // Print teller statistics
  for(const auto& teller : _tellers)
  {
    double busyRatio = 0.0;
    if(teller.totalTime() > 0)
    {
      busyRatio = ((double)teller.activeTime() / (double)teller.totalTime()) * 100.0;
    }

    out << "Teller " << teller.id() << ": ";
    out << teller.transactionCount() << " transactions";
    out << ", busy time " << teller.activeTime() << ", total time " << teller.totalTime() << " (" << busyRatio << "%)";
    out << std::endl;
  }

  // Print customer statistics
  int numCustomersComplete = _completedCustomers.size();
  out << "Total customers completed: " << numCustomersComplete << std::endl;
  
  double avgWaitTime = 0.0;
  for(const Customer* pCustomer : _completedCustomers)
  {
    avgWaitTime += (double)pCustomer->waitingTime();
  }
  avgWaitTime /= (double)numCustomersComplete;
  out << "Average customer wait time: " << avgWaitTime << std::endl;

  out << _customerQueue.size() << " customers still in line" << std::endl;
}

void Bank::handleEvent(des::SimEngine& sim, const des::Event& evt)
{
  switch(evt.type())
  {
    case Bank::EVT_START_SIM:
      // Originate first customer on simulation start
      sim.insertEvent(sim.time() + getCustomerInterarrivalTime(), Bank::EVT_ORIGINATE_CUSTOMER);
      break;

    case Bank::EVT_ORIGINATE_CUSTOMER:
      handleOriginateCustomer(sim, evt);
      break;

    case Bank::EVT_TRANSACTION_FINISH:
      handleTransactionFinish(sim, evt);
      break;

    default:
      break;
  }
}

void Bank::initialize(des::SimEngine& sim)
{
  // Schedule start event
  sim.insertEvent(0.0, Bank::EVT_START_SIM);
}

void Bank::finalize(des::SimEngine& sim)
{
}
