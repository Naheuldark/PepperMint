#pragma once

#include <entt.hpp>

#include "PepperMint/Core/Timestep.h"

namespace PepperMint {

class Entity;

class Scene {
    friend class Entity;
    friend class SceneHierarchyPanel;

  public:
    Scene()  = default;
    ~Scene() = default;

    Entity createEntity(const std::string& iName = "Entity");

    void onUpdate(Timestep iTimestep);
    void onViewportResize(uint32_t iWidth, uint32_t iHeight);

  private:
    entt::registry _registry;

    uint32_t _viewportWidth  = 0;
    uint32_t _viewportHeight = 0;
};
}
