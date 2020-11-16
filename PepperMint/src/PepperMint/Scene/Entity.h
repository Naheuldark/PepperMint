#pragma once

#include <entt.hpp>

#include "PepperMint/Scene/Scene.h"

namespace PepperMint {

class Entity {
  public:
    Entity()              = default;
    Entity(const Entity&) = default;
    Entity(entt::entity iEntity, Scene* iScene) : _entityHandle(iEntity), _scene(iScene) {}

    ~Entity() = default;

    //////////////////////
    // Entity functions //
    //////////////////////
    template <typename Component, typename... Args>
    Component& add(Args&&... args) {
        PM_CORE_ASSERT(!has<Component>(), "Entity already has component!");
        return _scene->_registry.emplace<Component>(_entityHandle, std::forward<Args>(args)...);
    }

    template <typename Component>
    Component& get() {
        PM_CORE_ASSERT(has<Component>(), "Entity already does not have component!");
        return _scene->_registry.get<Component>(_entityHandle);
    }

    template <typename Component>
    bool has() {
        return _scene->_registry.has<Component>(_entityHandle);
    }

    template <typename Component>
    void remove() {
        PM_CORE_ASSERT(has<Component>(), "Entity already does not have component!");
        return _scene->_registry.remove<Component>(_entityHandle);
    }

    // Other functions
    operator bool() const { return _entityHandle != entt::null; }

  private:
    entt::entity _entityHandle{entt::null};
    Scene*       _scene = nullptr;
};

}
