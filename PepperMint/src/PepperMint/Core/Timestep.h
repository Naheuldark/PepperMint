#pragma once

namespace PepperMint {

class Timestep {
  public:
    Timestep(float iTime = 0.0f) : _time(iTime) {}
    ~Timestep() = default;

    operator float() const { return _time; }

    float seconds() const { return _time; }
    float milliseconds() const { return _time * 1000.0f; }

  private:
    float _time;
};

}
