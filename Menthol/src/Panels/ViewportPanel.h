#pragma once

#include <PepperMint.h>

namespace Menthol {

class ViewportPanel {
  public:
    ViewportPanel()  = default;
    ~ViewportPanel() = default;

    void onUpdate(PepperMint::Timestep iTimestep);
    void onImGuiRender();
    bool onKeyPressed(PepperMint::KeyPressedEvent& iEvent);
    bool onMouseButtonPressed(PepperMint::MouseButtonPressedEvent& iEvent);

    PepperMint::EditorCamera& editorCamera() { return _editorCamera; }
    glm::vec2                 viewportSize() const { return _viewportSize; }
    bool                      viewportFocused() const { return _viewportFocused; }
    bool                      viewportHovered() const { return _viewportHovered; }
    PepperMint::Entity        selectedEntity() const { return _selectedEntity; }
    PepperMint::Entity        hoveredEntity() const { return _hoveredEntity; }
    std::filesystem::path&    sceneToOpen() { return _sceneToOpen; }

    void setEditorMode(bool iEditorMode) { _editorMode = iEditorMode; }
    void setSelectedEntity(PepperMint::Entity iSelectedEntity) { _selectedEntity = iSelectedEntity; }
    void setFrameBuffer(const PepperMint::Ref<PepperMint::FrameBuffer>& iFrameBuffer) { _frameBuffer = iFrameBuffer; }
    void setContext(const PepperMint::Ref<PepperMint::Scene>& iContext) { _context = iContext; }

  private:
    PepperMint::Entity       _selectedEntity{};
    PepperMint::EditorCamera _editorCamera;

    PepperMint::Ref<PepperMint::FrameBuffer> _frameBuffer;
    PepperMint::Ref<PepperMint::Scene>       _context;

    // Viewport
    glm::vec2 _viewportSize = {0.0f, 0.0f};
    glm::vec2 _viewportBounds[2]{};

    bool _viewportFocused = false;
    bool _viewportHovered = false;

    // Gizmos
    bool _editorMode = true;
    int  _gizmoType  = -1;

    // Mouse picking
    PepperMint::Entity _hoveredEntity{};

    // Drag & Drop
    std::filesystem::path _sceneToOpen;
};
}
