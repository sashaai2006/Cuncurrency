#pragma once
#include <condition_variable>
#include <cstddef>
#include <mutex>

template <size_t NofWorkers>
class Barrier {
 private:
  std::mutex mtx_;
  std::condition_variable cv_;
  size_t arrived_;
  size_t generation_;

 public:
  Barrier();
  void ArriveAndWait();
};

template <size_t NofWorkers>
Barrier<NofWorkers>::Barrier() : arrived_{0}, generation_{0} {}

template <size_t NofWorkers>
void Barrier<NofWorkers>::ArriveAndWait() {
  std::unique_lock lock(mtx_);
  const size_t generation = generation_;
  ++arrived_;
  if (arrived_ == NofWorkers) {
    ++generation_;
    arrived_ = 0;
    cv_.notify_all();
  }
  cv_.wait(lock, [&]() -> bool { return generation != generation_; });
}