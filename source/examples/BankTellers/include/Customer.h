#ifndef __CUSTOMER_H__
#define __CUSTOMER_H__

#include "DESCommon.h"
#include "core/Event.h"

class Teller;

class Customer
{
public:
  enum class Status
  {
    Unknown,
    Waiting,
    InTransaction,
    Finished
  };

public:
  Customer();
  ~Customer();

  void beginWaiting(const des::Event& evt);
  void beginTransaction(const des::Event& evt, Teller* pTeller);
  void endTransaction(const des::Event& evt);

  inline int id() const noexcept
  { return _customerId; }

  inline Status currentStatus() const noexcept
  { return _currentStatus; }

  inline des::SimTime waitingTime() const noexcept
  { return _waitingTime; }

  inline des::SimTime transactionTime() const noexcept
  { return _transactionTime; }

private:
  static int _nextCustomerId;

  Teller* _activeTeller;
  Status _currentStatus;
  
  int _customerId;

  des::SimTime _waitingTime;
  des::SimTime _transactionTime;

  des::SimTime _waitStartTime;
  des::SimTime _transactionStartTime;
};

#endif
