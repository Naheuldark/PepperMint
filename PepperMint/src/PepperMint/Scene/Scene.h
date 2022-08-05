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
    void   duplicateEntity(Entity iEntity);

    Entity getEntityByUUID(UUID iEntityUUID);

    template <typename... Components>
    auto getAllEntitiesWith() {
        return _registry.view<Components...>();
    }

    template <typename Func>
    void forEachEntity(Func iFunc) {
        _registry.each(iFunc);
    }

    void onRuntimeStart();
    void onRuntimeStop();

    void onSimulateStart();
    void onSimulateStop();

    void onUpdateRuntime(Timestep iTimestep);
    void onUpdateSimulate(Timestep iTimestep, const EditorCamera& iCamera);
    void onUpdateEditor(Timestep iTimestep, const EditorCamera& iCamera);

    void onViewportResize(uint32_t iWidth, uint32_t iHeight);

    Entity primaryCameraEntity();

    static Ref<Scene> Copy(const Ref<Scene>& iOther);

  private:
    template <typename Component>
    void onAddComponent(Entity iEntity, Component& ioComponent);

    void onPhysics2DStart();
    void onPhysics2DStop();

    void onUpdatePhysics2D(Timestep iTimestep);
    void onUpdateScript(Timestep iTimestep);

    void renderRuntimeScene();
    void renderEditorScene(const EditorCamera& iCamera);

  private:
    std::string _name;

    entt::registry _registry;

    b2World* _physicsWorld = nullptr;

    uint32_t _viewportWidth  = 0;
    uint32_t _viewportHeight = 0;

    std::unordered_map<UUID, entt::entity> _entityMap;
};
}
