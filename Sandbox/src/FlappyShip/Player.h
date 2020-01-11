#pragma once

#include <PepperMint.h>

class Player {
public:
	Player() = default;
	~Player() = default;

	void init();

	void onUpdate(PepperMint::Timestep iTimestep);
	void onRender();
	void onImGuiRender();

	inline float rotation() const { return _velocity.y * 4.0f - 90.0f; }
	inline const glm::vec2& position() const { return _position; }

private:
	glm::vec2 _position = { -10.0f, 0.0f };
	glm::vec2 _velocity = { 5.0f, 0.0f };

	PepperMint::Ref<PepperMint::Texture2D> _shipTexture;
};
