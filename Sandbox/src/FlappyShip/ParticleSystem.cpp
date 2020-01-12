#include "ParticleSystem.h"

#include "Random.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem() {
	_particles.resize(1000);
}

void ParticleSystem::emit(const ParticleProperties& iProperties) {
	Particle& particle = _particles[_particleIndex];
	particle.active = true;
	particle.position = iProperties.position;
	particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.velocity = iProperties.velocity;
	particle.velocity.x += iProperties.acceleration.x * (Random::Float() - 0.5f);
	particle.velocity.y += iProperties.acceleration.y * (Random::Float() - 0.5f);

	// Color
	particle.colorBegin = iProperties.colorBegin;
	particle.colorEnd = iProperties.colorEnd;

	// Size
	particle.sizeBegin = iProperties.sizeBegin + iProperties.sizeVariation * (Random::Float() - 0.5f);
	particle.sizeEnd = iProperties.sizeEnd;

	// Life
	particle.lifeTime = iProperties.lifeTime;
	particle.lifeRemaining = iProperties.lifeTime;

	_particleIndex = --_particleIndex % _particles.size();
}

void ParticleSystem::onUpdate(PepperMint::Timestep iTimestep) {
	for (auto& particle : _particles) {
		if (!particle.active)
			continue;

		if (particle.lifeRemaining <= 0.0f) {
			particle.active = false;
			continue;
		}

		particle.lifeRemaining -= iTimestep;
		particle.position += particle.velocity * (float)iTimestep;
		particle.rotation += 0.01f * iTimestep;
	}
}

void ParticleSystem::onRender() {
	for (auto&& particle : _particles) {
		if (!particle.active)
			continue;

		float life = particle.lifeRemaining / particle.lifeTime;
		glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);
		color.a = color.a * life;

		float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);

		PepperMint::Renderer2D::DrawQuad(particle.position, particle.rotation, { size, size }, nullptr, color);
	}
}
