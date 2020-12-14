#pragma once

#include <PepperMint.h>

namespace PepperMint {

class ViewportPanel {
  public:
    ViewportPanel()  = default;
    ~ViewportPanel() = default;

    void onImGuiRender();
    bool onKeyPressed(KeyPressedEvent& iEvent);

    EditorCamera& editorCamera() { return _editorCamera; }
    glm::vec2     viewportSize() const { return _viewportSize; }
    bool          viewportFocused() const { return _viewportFocused; }
    bool          viewportHovered() const { return _viewportHovered; }

    void setEditorMode(bool iEditorMode) { _editorMode = iEditorMode; }
    void setSelectedEntity(Entity iSelectedEntity) { _selectedEntity = iSelectedEntity; }
    void setTextureId(uint64_t iTextureId) { _textureId = iTextureId; }

  private:
    Entity       _selectedEntity{};
    EditorCamera _editorCamera;

    uint64_t _textureId = 0;

    // Viewport
    glm::vec2 _viewportSize    = {0.0f, 0.0f};
    bool      _viewportFocused = false;
    bool      _viewportHovered = false;

    // Gizmos
    bool _editorMode = true;
    int  _gizmoType  = -1;
};
}
