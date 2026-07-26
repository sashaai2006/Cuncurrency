#pragma once

#include "../atomic/atomic.hpp"

namespace sync {

class FutexMutex {
 private:
  Atomic state_;

 public:
  FutexMutex();
  FutexMutex(const FutexMutex&) = delete;
  FutexMutex& operator=(const FutexMutex&) = delete;
  FutexMutex(FutexMutex&&) = delete;
  FutexMutex& operator=(FutexMutex&&) = delete;
  ~FutexMutex() = default;
  void Lock();
  bool TryLock();
  void Unlock();
};

}  // namespace sync
