#include <imgui/imgui.h>

#include "SettingsPanel.h"

namespace Menthol {

void SettingsPanel::onImGuiRender() {
    ImGui::Begin("Settings");
    { ImGui::Checkbox("Show physics colliders", &_showPhysicsCollider); }
    ImGui::End();
}
}
