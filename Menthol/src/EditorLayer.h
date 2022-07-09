#pragma once

#include <PepperMint.h>

#include "EditorUtils.h"

#include "Panels/ContentBrowserPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/StatisticsPanel.h"
#include "Panels/ToolbarPanel.h"
#include "Panels/ViewportPanel.h"

namespace PepperMint {

class EditorLayer : public Layer {
  public:
    EditorLayer() : Layer("Menthol") {}
    ~EditorLayer() override = default;

    void onAttach() override;
    void onDetach() override;
    void onUpdate(Timestep iTimestep) override;
    void onImGuiRender() override;
    void onEvent(Event& iEvent) override;

    void onScenePlay();
    void onSceneStop();

    void duplicateSelectedEntity();

  private:
    bool onKeyPressed(KeyPressedEvent& iEvent);
    bool onMouseButtonPressed(MouseButtonPressedEvent& iEvent);

    void newScene();
    void openScene();
    void openScene(const std::filesystem::path& iPath);
    void saveScene();
    void saveSceneAs();

  private:
    // Scene
    Ref<Scene>       _activeScene;
    Ref<Scene>       _editorScene, _runtimeScene;
    Ref<FrameBuffer> _frameBuffer;
    SceneState       _sceneState = SceneState::EDIT;

    // Panels
    SceneHierarchyPanel _sceneHierarchyPanel;
    PropertiesPanel     _propertiesPanel;
    StatisticsPanel     _statisticsPanel;
    ViewportPanel       _viewportPanel;
    ContentBrowserPanel _contentBrowserPanel;
    ToolbarPanel        _toolbarPanel;
};
}
