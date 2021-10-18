#pragma once

#include <entt.hpp>

#include "PepperMint/Core/Timestep.h"
#include "PepperMint/Core/UUID.h"
#include "PepperMint/Renderer/EditorCamera.h"

class b2World;

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
    void        setName(const std::string& iName) { _name = iName; }

    Entity createEntity(const std::string& iName = "Entity");
    Entity createEntityWithUUID(const UUID& iUUID, const std::string& iName = "Entity");
    void   destroyEntity(Entity iEntity);

    void onRuntimeStart();
    void onRuntimeStop();

    void onUpdateRuntime(Timestep iTimestep);
    void onUpdateEditor(Timestep iTimestep, EditorCamera& iCamera);

    void onViewportResize(uint32_t iWidth, uint32_t iHeight);

    Entity primaryCameraEntity();

    void duplicateEntity(Entity iEntity);

    static Ref<Scene> Copy(const Ref<Scene>& iOther);

  private:
    template <typename Component>
    void onAddComponent(Entity iEntity, Component& ioComponent);

  private:
    std::string _name;

    entt::registry _registry;

    b2World* _physicsWorld = nullptr;

    uint32_t _viewportWidth  = 0;
    uint32_t _viewportHeight = 0;
};
}
