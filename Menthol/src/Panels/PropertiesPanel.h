#pragma once

#include <PepperMint.h>

namespace PepperMint {

class PropertiesPanel {
  public:
    PropertiesPanel()  = default;
    ~PropertiesPanel() = default;

    void onImGuiRender();

    void setSelectedEntity(Entity iSelectedEntity) { _selectedEntity = iSelectedEntity; }

  private:
    Entity _selectedEntity{};
};
}
