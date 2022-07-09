#pragma once

#include <PepperMint.h>

#include "../EditorUtils.h"

namespace PepperMint {

class ToolbarPanel {
  public:
    ToolbarPanel();
    ~ToolbarPanel() = default;

    void onImGuiRender();

    bool playButtonClicked() const { return _playButtonClicked; }
    void setSceneState(SceneState state) { _sceneState = state; }

  private:
    SceneState _sceneState        = SceneState::EDIT;
    bool       _playButtonClicked = false;

    // Icons
    Ref<Texture2D> _playIcon, _stopIcon;
};
}
