#pragma once

#include "../mutex/raii/unique_lock.hpp"
#include "../atomic/atomic.hpp"

class CondVar {
 private:
  Atomic var_;

 public:
  CondVar() noexcept : var_{0} {}
  ~CondVar() = default;
  CondVar(const CondVar&) = delete;
  CondVar& operator=(const CondVar&) = delete;
  CondVar(CondVar&&) = delete;
  CondVar& operator=(CondVar&&) = delete;
  void Wait(UniqueLock& lock);
  template <typename Pred>
  void Wait(UniqueLock& lock, Pred pred);
  void NotifyOne();
  void NotifyAll();
};