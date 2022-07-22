#pragma once

#include <PepperMint.h>

namespace Menthol {

class SettingsPanel {
  public:
    SettingsPanel()  = default;
    ~SettingsPanel() = default;

    void onImGuiRender();

    bool showPhysicsCollider() const { return _showPhysicsCollider; }

  private:
    bool _showPhysicsCollider = false;
};
}
