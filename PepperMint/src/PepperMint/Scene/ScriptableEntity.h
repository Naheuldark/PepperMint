#pragma once

#include "PepperMint/Scene/Entity.h"

namespace PepperMint {

class ScriptableEntity {
    friend class Scene;

  public:
    ScriptableEntity()          = default;
    virtual ~ScriptableEntity() = default;

    //////////////////////
    // Entity functions //
    //////////////////////
    template <typename Component>
    Component& get() {
        return _entity.get<Component>();
    }

  protected:
    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onUpdate(Timestep) {}

  private:
    Entity _entity;
};
}
