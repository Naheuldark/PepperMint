#pragma once

#include <PepperMint.h>

namespace PepperMint {

class ViewportPanel {
  public:
    ViewportPanel()  = default;
    ~ViewportPanel() = default;

    void onUpdate(Timestep iTimestep);
    void onImGuiRender();
    bool onKeyPressed(KeyPressedEvent& iEvent);
    bool onMouseButtonPressed(MouseButtonPressedEvent& iEvent);

    EditorCamera& editorCamera() { return _editorCamera; }
    glm::vec2     viewportSize() const { return _viewportSize; }
    bool          viewportFocused() const { return _viewportFocused; }
    bool          viewportHovered() const { return _viewportHovered; }
    Entity        selectedEntity() const { return _selectedEntity; }
    Entity        hoveredEntity() const { return _hoveredEntity; }

    void setEditorMode(bool iEditorMode) { _editorMode = iEditorMode; }
    void setSelectedEntity(Entity iSelectedEntity) { _selectedEntity = iSelectedEntity; }
    void setFrameBuffer(const Ref<FrameBuffer>& iFrameBuffer) { _frameBuffer = iFrameBuffer; }
    void setActiveScene(const Ref<Scene>& iActiveScene) { _activeScene = iActiveScene; }

  private:
    Entity       _selectedEntity{};
    EditorCamera _editorCamera;

    Ref<FrameBuffer> _frameBuffer;
    Ref<Scene>       _activeScene;

    // Viewport
    glm::vec2 _viewportSize = {0.0f, 0.0f};
    glm::vec2 _viewportBounds[2]{};

    bool _viewportFocused = false;
    bool _viewportHovered = false;

    // Gizmos
    bool _editorMode = true;
    int  _gizmoType  = -1;

    // Mouse picking
    Entity _hoveredEntity{};
};
}
