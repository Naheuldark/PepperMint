#pragma once

#include <glm/glm.hpp>
#include <utility>

#include "PepperMint/Core/KeyCodes.h"
#include "PepperMint/Core/MouseCodes.h"

namespace PepperMint {

class Input {
  public:
    static bool IsKeyPressed(KeyCode iKeyCode);

    static bool      IsMouseButtonPressed(MouseCode iButton);
    static glm::vec2 GetMousePosition();
    static float     GetMouseX();
    static float     GetMouseY();
};
}
