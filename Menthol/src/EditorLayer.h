#pragma once

#include <PepperMint.h>

namespace Menthol {

class EditorLayer : public PepperMint::Layer {
  public:
    EditorLayer() : Layer("Menthol") {}
    ~EditorLayer() override = default;

    void onAttach() override;
    void onDetach() override;
    void onUpdate(PepperMint::Timestep iTimestep) override;
    void onImGuiRender() override;
    void onEvent(PepperMint::Event& iEvent) override;

    void onScenePlay();
    void onSceneSimulate();
    void onSceneStop();

  private:
    void onOverlayRender();

    bool onKeyPressed(PepperMint::KeyPressedEvent& iEvent);
    bool onMouseButtonPressed(PepperMint::MouseButtonPressedEvent& iEvent);

    void duplicateSelectedEntity();

    void newScene();
    void openScene();
    void openScene(const std::filesystem::path& iPath);
    void saveScene();
    void saveSceneAs();

  private:
    void UI_Toolbar();
    void UI_Statistics();
    void UI_Settings();
    void UI_Properties();
    void UI_SceneHierarchy();
    void UI_ContentBrowser();

  private:
    enum class SceneState : int {
        EDIT     = 0,
        PLAY     = 1,
        SIMULATE = 2,
    };

  private:
    // Scene
    PepperMint::Ref<PepperMint::Scene>       _activeScene, _editorScene;
    PepperMint::Ref<PepperMint::FrameBuffer> _frameBuffer;

    SceneState            _sceneState = SceneState::EDIT;
    std::filesystem::path _scenePath;

    PepperMint::EditorCamera _editorCamera;
    PepperMint::Entity       _hoveredEntity{};
    PepperMint::Entity       _selectedEntity{};

    bool _showPhysicsCollider = false;

    // Viewport
    glm::vec2 _viewportSize = {0.0f, 0.0f};
    glm::vec2 _viewportBounds[2]{};

    bool _viewportFocused = false;
    bool _viewportHovered = false;

    int _gizmoType = -1;

    // Content Browser
    std::filesystem::path _currentDirectory;

    // Icons
    PepperMint::Ref<PepperMint::Texture2D> _playIcon, _stopIcon, _simulateIcon;
    PepperMint::Ref<PepperMint::Texture2D> _directoryIcon, _fileIcon;
};
}
