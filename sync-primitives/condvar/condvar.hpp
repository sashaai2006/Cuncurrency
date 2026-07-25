#pragma once

#include "../mutex/raii/unique_lock.hpp"

#include <atomic>
#include <cstdint>

class CondVar {
 private:
  std::atomic<uint32_t> var_{0};

 public:
  CondVar() noexcept = default;
  ~CondVar() = default;
  CondVar(const CondVar&) = delete;
  CondVar& operator=(const CondVar&) = delete;
  CondVar(CondVar&&) = delete;
  CondVar& operator=(CondVar&&) = delete;
  void Wait(UniqueLock& lock);
  void NotifyOne();
  void NotifyAll();
};