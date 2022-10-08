#pragma once

#include <chrono>

class Timer {
public:
  Timer() : _start(std::chrono::steady_clock::now()) {
  }

  template <class Unit>
  int getElapsed() {
    return std::chrono::duration_cast<Unit>(std::chrono::steady_clock::now() - _start).count();
  }

private:
  std::chrono::time_point<std::chrono::steady_clock> _start;
};