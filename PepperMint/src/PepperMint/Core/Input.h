#pragma once

#include <utility>

#include "PepperMint/Core/Base.h"
#include "PepperMint/Core/KeyCodes.h"
#include "PepperMint/Core/MouseCodes.h"

namespace PepperMint {

class Input {
  public:
    static bool IsKeyPressed(KeyCode iKeyCode);

    static bool                    IsMouseButtonPressed(MouseCode iButton);
    static std::pair<float, float> GetMousePosition();
    static float                   GetMouseX();
    static float                   GetMouseY();
};
}
