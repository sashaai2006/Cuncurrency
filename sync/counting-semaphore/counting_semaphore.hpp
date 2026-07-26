#pragma once

#include <cstddef>
#include <condition_variable>
#include <mutex>

namespace sync {

template <ptrdiff_t MaxPermits>
class CountingSemaphore {
 private:
  std::mutex mtx_;
  std::condition_variable cv_;
  ptrdiff_t permits_;

 public:
  CountingSemaphore();
  explicit CountingSemaphore(ptrdiff_t permits);
  void Acquire();
  void Release();
};

template <ptrdiff_t MaxPermits>
CountingSemaphore<MaxPermits>::CountingSemaphore() : permits_{MaxPermits} {}

template <ptrdiff_t MaxPermits>
CountingSemaphore<MaxPermits>::CountingSemaphore(ptrdiff_t permits) : permits_{permits} {}

template <ptrdiff_t MaxPermits>
void CountingSemaphore<MaxPermits>::Acquire() {
  std::unique_lock lock(mtx_);
  cv_.wait(lock, [this]() -> bool { return permits_ > 0; });
  --permits_;
}

template <ptrdiff_t MaxPermits>
void CountingSemaphore<MaxPermits>::Release() {
  std::lock_guard lock(mtx_);
  ++permits_;
  cv_.notify_one();
}

}  // namespace sync
