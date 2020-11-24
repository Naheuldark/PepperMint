#pragma once

#include <entt.hpp>

#include "PepperMint/Core/Log.h"
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
        auto&& component = _scene->_registry.emplace<Component>(_entityHandle, std::forward<Args>(args)...);
        _scene->onAddComponent(*this, component);
        return component;
    }

    template <typename Component>
    Component& get() {
        PM_CORE_ASSERT(has<Component>(), "Entity does not have component!");
        return _scene->_registry.get<Component>(_entityHandle);
    }

    template <typename Component>
    bool has() {
        return _scene->_registry.has<Component>(_entityHandle);
    }

    template <typename Component>
    void remove() {
        PM_CORE_ASSERT(has<Component>(), "Entity does not have component!");
        return _scene->_registry.remove<Component>(_entityHandle);
    }

    // Other functions
    operator bool() const { return _entityHandle != entt::null; }
    operator entt::entity() const { return _entityHandle; }
    operator uint32_t() const { return (uint32_t)_entityHandle; }

    bool operator==(const Entity& iOther) const { return (_entityHandle == iOther._entityHandle) && (_scene == iOther._scene); }
    bool operator!=(const Entity& iOther) const { return !(*this == iOther); }

  private:
    entt::entity _entityHandle{entt::null};
    Scene*       _scene = nullptr;
};

}
