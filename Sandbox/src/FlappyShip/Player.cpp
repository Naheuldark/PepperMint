#include "Player.h"

void Player::init() {
	_shipTexture = PepperMint::Texture2D::Create("assets/textures/Ship.png");
}

void Player::onUpdate(PepperMint::Timestep iTimestep) {}

void Player::onRender() {
	PepperMint::Renderer2D::DrawQuad({ _position.x, _position.y, 0.5f }, { 1.0f, 1.3f }, glm::radians(rotation()), _shipTexture);
}

void Player::onImGuiRender() {}
