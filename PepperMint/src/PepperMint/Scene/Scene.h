#pragma once

#include <entt.hpp>

#include "PepperMint/Core/Timestep.h"

namespace PepperMint {

class Scene {
public:
	Scene();
	~Scene() = default;

	entt::entity createEntity();

	// TEMP
	entt::registry& registry() { return _registry; }

	void onUpdate(Timestep iTimestep);

private:
	entt::registry _registry;
};
}