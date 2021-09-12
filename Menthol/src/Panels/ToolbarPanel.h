#pragma once

#include <PepperMint.h>

#include "../EditorStatus.h"

namespace PepperMint {

class ToolbarPanel {
  public:
    ToolbarPanel();
    ~ToolbarPanel() = default;

    void onImGuiRender();

	SceneState sceneState() const { return _sceneState; }
    void       setSceneState(SceneState state) { _sceneState = state; }

  private:
    SceneState _sceneState = SceneState::EDIT;

    // Icons
    Ref<Texture2D> _playIcon, _stopIcon;
};
}
