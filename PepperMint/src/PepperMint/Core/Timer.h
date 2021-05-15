#pragma once

#include <chrono>

namespace PepperMint {

class Timer {
  public:
    Timer()  = default;
    ~Timer() = default;

    void  reset() { _start = std::chrono::high_resolution_clock::now(); }
    float elapsed() {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _start).count() * 0.0000001f;
    }
    float elapsedMillis() { return elapsed() * 1000.0f; }

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
};
}
