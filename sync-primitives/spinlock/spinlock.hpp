#pragma once
#include "../atomic/atomic.hpp"

class TASSpinLock {
 private:
  Atomic locked_{0};

 public:
  void Lock();
  bool TryLock();
  void Unlock();
};

class TTASSpinLock {
 private:
  Atomic locked_{0};

 public:
  void Lock();
  bool TryLock();
  void Unlock();
};