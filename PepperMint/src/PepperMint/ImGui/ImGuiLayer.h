#pragma once

#include "PepperMint/Core/Layer.h"
#include "PepperMint/Events/ApplicationEvent.h"
#include "PepperMint/Events/KeyEvent.h"
#include "PepperMint/Events/MouseEvent.h"

namespace PepperMint {

class ImGuiLayer : public Layer {
  public:
    ImGuiLayer() : Layer("ImGuiLayer") {}
    ~ImGuiLayer() = default;

    void onAttach() override;
    void onDetach() override;
    void onEvent(Event& iEvent) override;

    inline void setBlockEvents(bool iBlockEvents) { _blockEvents = iBlockEvents; }

    void begin();
    void end();

  private:
    float _time        = 0.0f;
    bool  _blockEvents = true;
};

}
