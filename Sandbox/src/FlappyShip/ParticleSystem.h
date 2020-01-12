#pragma once

#include <PepperMint.h>

struct ParticleProperties {
	glm::vec2 position, velocity, acceleration;
	glm::vec4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifeTime = 1.0f;
};

class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem() = default;

	void emit(const ParticleProperties& iProperties);

	void onUpdate(PepperMint::Timestep iTimestep);
	void onRender();

private:
	struct Particle {
		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec4 colorBegin, colorEnd;
		float rotation = 0.0f;
		float sizeBegin, sizeEnd;

		float lifeTime = 1.0f;
		float lifeRemaining = 0.0f;

		bool active = false;
	};
	std::vector<Particle> _particles;
	uint32_t _particleIndex = 999;
};