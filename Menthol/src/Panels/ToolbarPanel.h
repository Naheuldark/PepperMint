#pragma once

#include <PepperMint.h>

#include "../EditorUtils.h"

namespace Menthol {

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
    PepperMint::Ref<PepperMint::Texture2D> _playIcon, _stopIcon;
};
}
