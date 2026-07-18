#include "mutex.hpp"

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

FutexMutex::FutexMutex() : state_() {}

void FutexMutex::Lock() {
  if (state_.Exchange(1) == 0) {
    return;
  }

  auto* uaddr = const_cast<uint32_t*>(reinterpret_cast<volatile uint32_t*>(state_.Data()));

  while (state_.Exchange(2) != 0) {
    FutexWait(uaddr, 2);
  }
}

void FutexMutex::Unlock() {
  if (state_.Exchange(0) == 1) {
    return;
  }
  auto* uaddr = const_cast<uint32_t*>(reinterpret_cast<volatile uint32_t*>(state_.Data()));
  FutexWake(uaddr, 1);
}

bool FutexMutex::TryLock() {
  if (state_.Load() != 0) {
    return false;
  }
  return state_.Exchange(1) == 0;
