#pragma once

#include <exception>
#include <functional>
#include <mutex>
#include <utility>

template <typename Task>
class Once {
 private:
  enum class State {
    pending,
    done,
    failed,
  };

  std::mutex mutex_;
  Task task_;
  State state_{State::pending};
  std::exception_ptr error_;

 public:
  explicit Once(Task task) : task_(std::move(task)) {}

  Once(const Once&) = delete;
  Once& operator=(const Once&) = delete;
  Once(Once&&) = delete;
  Once& operator=(Once&&) = delete;

  void Do() {
    std::lock_guard lock(mutex_);

    switch (state_) {
      case State::done:
        return;

      case State::failed:
        std::rethrow_exception(error_);

      case State::pending:
        break;
    }

    try {
      std::invoke(task_);
      state_ = State::done;
    } catch (...) {
      error_ = std::current_exception();
      state_ = State::failed;
      throw;
    }
  }
};