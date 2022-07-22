#pragma once

#include <PepperMint.h>

namespace Menthol {

class SceneHierarchyPanel {
  public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const PepperMint::Ref<PepperMint::Scene>& iScene) : _context(iScene) {}

    ~SceneHierarchyPanel() = default;

    void onImGuiRender();

    PepperMint::Entity selectedEntity() const { return _selectedEntity; }
    void               setSelectedEntity(PepperMint::Entity iSelectedEntity) { _selectedEntity = iSelectedEntity; }

    void setContext(const PepperMint::Ref<PepperMint::Scene>& iScene) {
        _context        = iScene;
        _selectedEntity = {};
    }

  private:
    void drawEntityNode(PepperMint::Entity iEntity);

  private:
    PepperMint::Ref<PepperMint::Scene> _context;
    PepperMint::Entity                 _selectedEntity;
};
}
