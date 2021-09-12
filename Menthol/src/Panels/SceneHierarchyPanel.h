#pragma once

#include <PepperMint.h>

namespace PepperMint {

class SceneHierarchyPanel {
  public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Ref<Scene>& iScene) : _context(iScene) {}

    ~SceneHierarchyPanel() = default;

    void onImGuiRender();

    Entity selectedEntity() const { return _selectedEntity; }
    void   setSelectedEntity(Entity iSelectedEntity) { _selectedEntity = iSelectedEntity; }

    void setContext(const Ref<Scene>& iScene) {
        _context        = iScene;
        _selectedEntity = {};
    }

  private:
    void drawEntityNode(Entity iEntity);

  private:
    Ref<Scene> _context;
    Entity     _selectedEntity;
};
}
