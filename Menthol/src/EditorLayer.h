#pragma once

#include <PepperMint.h>

#include "Panels/PropertiesPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/StatisticsPanel.h"
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

  private:
    bool onKeyPressed(KeyPressedEvent& iEvent);
    bool onMouseButtonPressed(MouseButtonPressedEvent& iEvent);

    void newScene();
    void openScene();
    void saveScene();
    void saveSceneAs();

  private:
    // Scene
    Ref<Scene>       _activeScene;
    Ref<FrameBuffer> _frameBuffer;

    bool _playing = false;

    // Panels
    SceneHierarchyPanel _sceneHierarchyPanel;
    PropertiesPanel     _propertiesPanel;
    ViewportPanel       _viewportPanel;
    StatisticsPanel     _statisticsPanel;
};
}
