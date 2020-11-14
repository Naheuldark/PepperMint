#pragma once

#include <PepperMint.h>

#include <imgui/imgui.h>

class GraphLayer : public PepperMint::Layer {
  public:
    GraphLayer();
    ~GraphLayer() override = default;

    void onAttach() override;
    void onDetach() override;
    void onUpdate(PepperMint::Timestep iTimestep) override;
    void onImGuiRender() override;
    void onEvent(PepperMint::Event& iEvent) override;

  private:
    void updateCamera(uint32_t iWidth, uint32_t iHeight);

    bool onWindowResize(PepperMint::WindowResizeEvent& iEvent);

  private:
    PepperMint::Scope<PepperMint::OrthographicCamera> _camera;
    ImFont*                                           _font;
    float                                             _time = 0.0f;
    bool                                              _send = false;
};
