#pragma once

#include "../mutex.hpp"

class LockGuard {
 private:
  FutexMutex* mutex_;

 public:
  explicit LockGuard(FutexMutex& mutex) : mutex_(&mutex) {
    mutex_->Lock();
  }

  ~LockGuard() {
    mutex_->Unlock();
  }

  LockGuard(const LockGuard&) = delete;
  LockGuard& operator=(const LockGuard&) = delete;
  LockGuard(LockGuard&&) = delete;
  LockGuard& operator=(LockGuard&&) = delete;
};