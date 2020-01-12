#include "Player.h"

#include <imgui/imgui.h>

void Player::init() {
	_shipTexture = PepperMint::Texture2D::Create("assets/textures/Ship.png");
}

void Player::onUpdate(PepperMint::Timestep iTimestep) {
	if (PepperMint::Input::IsKeyPressed(PM_KEY_SPACE)) {
		_velocity.y += (_velocity.y < 0.0f ? _enginePower * 2.0f : _enginePower);
	} else {
		_velocity.y -= _gravity;
	}

	_velocity.y = glm::clamp(_velocity.y, -20.0f, 20.0f);
	_position += _velocity * (float)iTimestep;
}

void Player::onRender() {
	PepperMint::Renderer2D::DrawQuad({ _position.x, _position.y, 0.5f }, glm::radians(rotation()), { 1.0f, 1.3f }, _shipTexture);
}

void Player::onImGuiRender() {
	ImGui::DragFloat("Engine Power", &_enginePower, 0.1f);
	ImGui::DragFloat("Gravity", &_gravity, 0.1f);

	ImGui::DragFloat("Velocity.y", &_velocity.y, 0.1f);
}

void Player::reset() {
	_position = { -10.0f, 0.0f };
	_velocity = { 5.0f, 0.0f };
}
