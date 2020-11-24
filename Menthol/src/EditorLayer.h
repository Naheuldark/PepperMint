#pragma once

#include <PepperMint.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/StatisticsPanel.h"

namespace PepperMint {

class EditorLayer : public Layer {
  public:
    EditorLayer() : Layer("Menthol"), _cameraController(1280.0f / 720.0f, true) {}
    ~EditorLayer() override = default;

    void onAttach() override;
    void onDetach() override;
    void onUpdate(Timestep iTimestep) override;
    void onImGuiRender() override;
    void onEvent(Event& iEvent) override;

  private:
    OrthographicCameraController _cameraController;

    Ref<VertexArray> _squareVA;
    Ref<Shader>      _flatColorShader;
    Ref<FrameBuffer> _frameBuffer;

    Ref<Scene> _activeScene;
    Entity     _squareEntity;

    Entity _mainCamera;
    Entity _secondCamera;
    bool   _primaryCamera = true;

    Ref<Texture2D> _checkerboardTexture;

    glm::vec2 _viewportSize    = {0.0f, 0.0f};
    bool      _viewportFocused = false;
    bool      _viewportHovered = false;

    glm::vec4 _squareColor = {0.2f, 0.3f, 0.8f, 1.0};

    // Panels
    SceneHierarchyPanel _sceneHierarchyPanel;
    StatisticsPanel     _statisticsPanel;
};
}
