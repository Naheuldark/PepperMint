#pragma once

#include <PepperMint/Core/Base.h>
#include <PepperMint/Scene/Entity.h>
#include <PepperMint/Scene/Scene.h>

namespace PepperMint {

class SceneHierarchyPanel {
  public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Ref<Scene>& iScene) : _context(iScene) {}

    ~SceneHierarchyPanel() = default;

    void setContext(const Ref<Scene>& iScene) { _context = iScene; }

    void onImGuiRender();

  private:
    void drawEntityNode(Entity iEntity);

  private:
    Ref<Scene> _context;
    Entity     _selectionContext;
};
}
