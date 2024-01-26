#include "Customer.h"

#include "DESCommon.h"
#include "core/Event.h"

#include <stdexcept>

int Customer::_nextCustomerId = 0;

Customer::Customer() :
  _activeTeller{nullptr},
  _currentStatus{Customer::Status::Unknown},
  _customerId{_nextCustomerId++},
  _waitingTime{0},
  _transactionTime{0}
{
}

Customer::~Customer()
{
}

void Customer::beginWaiting(const des::Event& evt)
{
  _currentStatus = Customer::Status::Waiting;
  _waitStartTime = evt.time();
}

void Customer::beginTransaction(const des::Event& evt, Teller* pTeller)
{
  if(!pTeller)
  {
    throw std::invalid_argument("Teller is null");
  }

  _activeTeller = pTeller;
  _currentStatus = Customer::Status::InTransaction;

  _transactionStartTime = evt.time();
  _waitingTime = evt.time() - _waitStartTime;
}

void Customer::endTransaction(const des::Event& evt)
{
  _activeTeller = nullptr;
  _currentStatus = Customer::Status::Finished;
  
  _transactionTime = evt.time() - _transactionStartTime;
}
