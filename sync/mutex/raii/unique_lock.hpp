#pragma once

#include "../mutex.hpp"

namespace sync {

class UniqueLock {
 private:
  FutexMutex* mutex_{nullptr};
  bool owns_{false};

 public:
  UniqueLock(FutexMutex& mutex) : mutex_(&mutex), owns_(true) {
    mutex_->Lock();
  }

  ~UniqueLock() {
    if (owns_ && mutex_) {
      mutex_->Unlock();
    }
  }

  UniqueLock(const UniqueLock&) = delete;
  UniqueLock& operator=(const UniqueLock&) = delete;

  UniqueLock(UniqueLock&& other) noexcept : mutex_(other.mutex_), owns_(other.owns_) {
    other.mutex_ = nullptr;
    other.owns_ = false;
  }

  UniqueLock& operator=(UniqueLock&& other) noexcept {
    if (this != &other) {
      if (owns_ && mutex_) {
        mutex_->Unlock();
      }
      mutex_ = other.mutex_;
      owns_ = other.owns_;
      other.mutex_ = nullptr;
      other.owns_ = false;
    }
    return *this;
  }

  void Lock() {
    if (mutex_) {
      mutex_->Lock();
      owns_ = true;
    }
  }

  bool TryLock() {
    if (mutex_) {
      owns_ = mutex_->TryLock();
      return owns_;
    }
    return false;
  }

  void Unlock() {
    if (owns_ && mutex_) {
      mutex_->Unlock();
      owns_ = false;
    }
  }

  bool OwnsLock() const {
    return owns_;
  }
};

}  // namespace sync
