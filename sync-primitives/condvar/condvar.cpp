#include "condvar.hpp"

#include <cerrno>
#include <cstdint>
#include <limits>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace {

constexpr int kWakeAll = std::numeric_limits<int>::max();

long Futex(uint32_t* uaddr, int futex_op, uint32_t val) {
  return ::syscall(SYS_futex, uaddr, futex_op, val,
                   /*timeout=*/nullptr,
                   /*uaddr2=*/nullptr,
                   /*val3=*/0);
}

inline int FutexWait(uint32_t* uaddr, uint32_t expected) {
  return static_cast<int>(Futex(uaddr, FUTEX_WAIT_PRIVATE, expected));
}

inline int FutexWake(uint32_t* uaddr, int n) {
  return static_cast<int>(Futex(uaddr, FUTEX_WAKE_PRIVATE, static_cast<uint32_t>(n)));
}

}  // namespace

void CondVar::Wait(UniqueLock& lock) {
  auto current_var = var_.load();
  lock.Unlock();

  auto uaddr = reinterpret_cast<uint32_t*>(&var_);
  FutexWait(uaddr, current_var);
  lock.Lock();
}

void CondVar::NotifyOne() {
  var_.fetch_add(1);
  auto uaddr = reinterpret_cast<uint32_t*>(&var_);
  FutexWake(uaddr, 1);
}

void CondVar::NotifyAll() {
  var_.fetch_add(1);
  auto uaddr = reinterpret_cast<uint32_t*>(&var_);
  FutexWake(uaddr, kWakeAll);
}