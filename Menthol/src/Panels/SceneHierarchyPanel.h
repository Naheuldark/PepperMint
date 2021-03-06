#pragma once

#include <PepperMint/Core/Base.h>
#include <PepperMint/ImGui/ImGuiLayer.h>
#include <PepperMint/Scene/Entity.h>
#include <PepperMint/Scene/Scene.h>

namespace PepperMint {

class SceneHierarchyPanel {
  public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Ref<Scene>& iScene) : _context(iScene) {}

    ~SceneHierarchyPanel() = default;

    void onImGuiRender();

    Entity selectedEntity() const { return _selectedEntity; }
    void   setHoveredEntity(Entity iHoveredEntity) { _hoveredEntity = iHoveredEntity; }
    void   setContext(const Ref<Scene>& iScene) {
        _context        = iScene;
        _selectedEntity = {};
    }

  private:
    void drawEntityNode(Entity iEntity);

  private:
    Ref<Scene> _context;
    Entity     _selectedEntity;
    Entity     _hoveredEntity;
};
}
