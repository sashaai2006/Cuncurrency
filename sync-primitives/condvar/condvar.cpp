#include "condvar.hpp"

#include <cerrno>
#include <cstdint>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace {

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
  auto current_var = var_.Load();
  lock.Unlock();

  auto uaddr = const_cast<uint32_t*>(reinterpret_cast<volatile uint32_t*>(var_.Data()));
  FutexWait(uaddr, current_var);
  lock.Lock();
}

void CondVar::NotifyOne() {
  var_.FetchAdd(1);
  auto uaddr = const_cast<uint32_t*>(reinterpret_cast<volatile uint32_t*>(var_.Data()));
  FutexWake(uaddr, 1);
}

void CondVar::NotifyAll() {
  var_.FetchAdd(1);
  auto uaddr = const_cast<uint32_t*>(reinterpret_cast<volatile uint32_t*>(var_.Data()));
  FutexWake(uaddr, -1);
}