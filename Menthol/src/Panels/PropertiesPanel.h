#pragma once

#include <PepperMint.h>

namespace Menthol {

class PropertiesPanel {
  public:
    PropertiesPanel()  = default;
    ~PropertiesPanel() = default;

    void onImGuiRender();

    void setSelectedEntity(PepperMint::Entity iSelectedEntity) { _selectedEntity = iSelectedEntity; }

  private:
    PepperMint::Entity _selectedEntity{};
};
}
