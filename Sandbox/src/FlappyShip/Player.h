#pragma once

#include <PepperMint.h>

#include "ParticleSystem.h"

class Player {
  public:
    Player();
    ~Player() = default;

    void init();

    void onUpdate(PepperMint::Timestep iTimestep);
    void onRender();
    void onImGuiRender();

    void reset();

    uint32_t         score() const { return (uint32_t)(_position.x + 10.0f) / 10.0f; }
    float            rotation() const { return _velocity.y * 4.0f - 90.0f; }
    const glm::vec2& position() const { return _position; }

  private:
    glm::vec2 _position = {-10.0f, 0.0f};
    glm::vec2 _velocity = {5.0f, 0.0f};

    float _enginePower = 0.5f;
    float _gravity     = 0.4f;

    PepperMint::Ref<PepperMint::Texture2D> _shipTexture;

    // Particles
    ParticleProperties _smoke, _flames;
    ParticleSystem     _particleSystem;

    float _time              = 0.0f;
    float _smokeEmitInterval = 0.4f;
    float _smokeNextEmitTime = _smokeEmitInterval;
};
