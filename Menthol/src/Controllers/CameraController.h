#pragma once

#include "PepperMint/Core/Input.h"
#include "PepperMint/Scene/Components.h"
#include "PepperMint/Scene/ScriptableEntity.h"

namespace PepperMint {

class CameraController : public ScriptableEntity {
  public:
    void onUpdate(Timestep iTimestep) override {
        auto&& translation = get<TransformComponent>().translation;
        float  speed       = 5.0f;

        if (Input::IsKeyPressed(Key::A)) {
            translation.x -= speed * iTimestep;
        } else if (Input::IsKeyPressed(Key::D)) {
            translation.x += speed * iTimestep;
        }

        if (Input::IsKeyPressed(Key::S)) {
            translation.y -= speed * iTimestep;
        } else if (Input::IsKeyPressed(Key::W)) {
            translation.y += speed * iTimestep;
        }
    }
};
}
