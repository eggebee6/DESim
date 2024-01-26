#ifndef __TELLER_H__
#define __TELLER_H__

#include "DESCommon.h"
#include "core/Event.h"

class Customer;

class Teller
{
public:
  Teller();
  ~Teller();

  void beginTransaction(des::Event evt, Customer* pCustomer);
  void endTransaction(des::Event evt);

  inline bool isBusy() const noexcept
  { return (_activeCustomer != nullptr); }

  inline Customer* activeCustomer() const noexcept
  { return _activeCustomer; }

  inline int id() const noexcept
  { return _tellerId; }

  inline int transactionCount() const noexcept
  { return _transactionCount; }

  inline des::SimTime totalTime() const noexcept
  { return _totalTime; }

  inline des::SimTime activeTime() const noexcept
  { return _activeTime; }

private:
  static int _nextTellerId;

  Customer* _activeCustomer;

  int _tellerId;

  des::SimTime _transactionStartTime;
  des::SimTime _transactionEndTime;

  int _transactionCount;
  des::SimTime _totalTime;
  des::SimTime _activeTime;
};

#endif
