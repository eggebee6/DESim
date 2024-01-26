#include "Teller.h"

#include "DESCommon.h"
#include "core/Event.h"

#include "Customer.h"

#include <stdexcept>

int Teller::_nextTellerId = 0;

Teller::Teller() :
  _activeCustomer{nullptr},
  _tellerId{_nextTellerId++},
  _transactionStartTime{0},
  _transactionEndTime{0},
  _transactionCount{0},
  _totalTime{0},
  _activeTime{0}
{
}

Teller::~Teller()
{
}

void Teller::beginTransaction(des::Event evt, Customer* pCustomer)
{
  if(!pCustomer)
  {
    throw std::invalid_argument("Customer is null");
  }

  _activeCustomer = pCustomer;

  ++_transactionCount;
  _transactionStartTime = evt.time();
  _totalTime += evt.time() - _transactionEndTime;
}

void Teller::endTransaction(des::Event evt)
{
  _activeCustomer = nullptr;

  _transactionEndTime = evt.time();
  _activeTime += evt.time() - _transactionStartTime;
  _totalTime += evt.time() - _transactionStartTime;
}
