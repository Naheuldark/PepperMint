#pragma once

#include "PepperMint/Scene/Entity.h"

namespace PepperMint {

class ScriptableEntity {
    friend class Scene;

  public:
    ScriptableEntity()  = default;
    ~ScriptableEntity() = default;

    //////////////////////
    // Entity functions //
    //////////////////////
    template <typename Component>
    Component& get() {
        return _entity.get<Component>();
    }

  private:
    Entity _entity;
};
}
