#pragma once

#include <entt.hpp>

#include "PepperMint/Core/Timestep.h"

namespace PepperMint {

class Entity;

class Scene {
    friend class Entity;

  public:
    Scene()  = default;
    ~Scene() = default;

    Entity createEntity(const std::string& iName = "Entity");

    void onUpdate(Timestep iTimestep);

  private:
    entt::registry _registry;
};
}
