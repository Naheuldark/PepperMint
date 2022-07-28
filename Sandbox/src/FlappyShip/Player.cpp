#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

#include "Player.h"

Player::Player() {
    // Smoke
    _smoke.position     = {0.0f, 0.0f};
    _smoke.velocity     = {-2.0f, 0.0f};
    _smoke.acceleration = {4.0f, 2.0f};

    _smoke.sizeBegin     = 0.35f;
    _smoke.sizeEnd       = 0.0f;
    _smoke.sizeVariation = 0.15f;

    _smoke.colorBegin = {0.8f, 0.8f, 0.8f, 1.0f};
    _smoke.colorEnd   = {0.6f, 0.6f, 0.6f, 1.0f};

    _smoke.lifeTime = 4.0f;

    // Flames
    _flames.position     = {0.0f, 0.0f};
    _flames.velocity     = {-2.0f, 0.0f};
    _flames.acceleration = {3.0f, 1.0f};

    _flames.sizeBegin     = 0.5f;
    _flames.sizeEnd       = 0.0f;
    _flames.sizeVariation = 0.3f;

    _flames.colorBegin = {254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f};
    _flames.colorEnd   = {254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f};

    _flames.lifeTime = 1.0f;
}

void Player::init() { _shipTexture = PepperMint::Texture2D::Create("assets/textures/Ship.png"); }

void Player::onUpdate(PepperMint::Timestep iTimestep) {
    _time += iTimestep;

    if (PepperMint::Input::IsKeyPressed(PM_KEY_SPACE)) {
        _velocity.y += (_velocity.y < 0.0f ? _enginePower * 2.0f : _enginePower);

        // Flames
        glm::vec2 emissionPoint = {0.0f, -0.6f};
        glm::vec4 rotated       = glm::rotate(glm::mat4(1.0f), glm::radians(rotation()), {0.0f, 0.0f, 1.0f}) * glm::vec4(emissionPoint, 0.0f, 1.0f);

        _flames.position   = _position + glm::vec2{rotated.x, rotated.y};
        _flames.velocity.y = -_velocity.y * 0.2f - 0.2f;

        _particleSystem.emit(_flames);
    } else {
        _velocity.y -= _gravity;
    }

    _velocity.y = glm::clamp(_velocity.y, -20.0f, 20.0f);
    _position += _velocity * (float)iTimestep;

    // Particles
    if (_time > _smokeNextEmitTime) {
        _smoke.position = _position;
        _particleSystem.emit(_smoke);
        _smokeNextEmitTime += _smokeEmitInterval;
    }

    _particleSystem.onUpdate(iTimestep);
}

void Player::onRender() {
    _particleSystem.onRender();
    PepperMint::Renderer2D::DrawQuad({_position.x, _position.y, 0.5f}, glm::radians(rotation()), {1.0f, 1.3f}, 1.0f, _shipTexture);
}

void Player::onImGuiRender() {
    ImGui::DragFloat("Engine Power", &_enginePower, 0.1f);
    ImGui::DragFloat("Gravity", &_gravity, 0.1f);

    ImGui::DragFloat("Velocity.y", &_velocity.y, 0.1f);
}

void Player::reset() {
    _position = {-10.0f, 0.0f};
    _velocity = {5.0f, 0.0f};
}
