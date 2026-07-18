#include "spinlock.hpp"

void TASSpinLock::Lock() {
  while (locked_.Exchange(1) != 0) {
  }
}

bool TASSpinLock::TryLock() {
  return locked_.Exchange(1) == 0;
}

void TASSpinLock::Unlock() {
  locked_.Store(0);
}
