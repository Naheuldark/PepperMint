#pragma once

#include <entt.hpp>

#include "PepperMint/Core/Timestep.h"
#include "PepperMint/Renderer/EditorCamera.h"

namespace PepperMint {

class Entity;

class Scene {
    friend class Entity;
    friend class SceneHierarchyPanel;
    friend class SceneSerializer;

  public:
    Scene(const std::string& iName = "Untitled") : _name(iName) {}
    ~Scene() = default;

    std::string name() const { return _name; }

    Entity createEntity(const std::string& iName = "Entity");
    void   destroyEntity(Entity iEntity);

    void onUpdateRuntime(Timestep iTimestep);
    void onUpdateEditor(Timestep iTimestep, EditorCamera& iCamera);

    void onViewportResize(uint32_t iWidth, uint32_t iHeight);

    Entity primaryCameraEntity();

  private:
    template <typename Component>
    void onAddComponent(Entity iEntity, Component& ioComponent);

  private:
    std::string _name;

    entt::registry _registry;

    uint32_t _viewportWidth  = 0;
    uint32_t _viewportHeight = 0;
};
}
