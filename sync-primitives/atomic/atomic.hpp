#pragma once

extern "C" {
#include "atomic.h"
}

class Atomic {
 private:
  AtomicStorage storage_;

 public:
  using Value = AtomicValue;

  explicit Atomic(Value init = 0) : storage_{init} {}

  Atomic(const Atomic&) = delete;
  Atomic& operator=(const Atomic&) = delete;
  Atomic(Atomic&&) = delete;
  Atomic& operator=(Atomic&&) = delete;

  void Store(Value value) {
    AtomicStore(&storage_, value);
  }

  Value Load() {
    return AtomicLoad(&storage_);
  }

  Value Exchange(Value value) {
    return AtomicExchange(&storage_, value);
  }

  Value FetchAdd(Value delta) {
    return AtomicFetchAdd(&storage_, delta);
  }

  AtomicStorage* Data() {
    return &storage_;
  }
};