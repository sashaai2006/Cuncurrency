#include "spinlock.hpp"

namespace sync {

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

void TTASSpinLock::Lock() {
  while (true) {
    while (locked_.Load() != 0) {
    }
    if (locked_.Exchange(1) == 0) {
      return;
    }
  }
}

bool TTASSpinLock::TryLock() {
  if (locked_.Load() != 0) {
    return false;
  }

  return locked_.Exchange(1) == 0;
}

void TTASSpinLock::Unlock() {
  locked_.Store(0);
}

}  // namespace sync
