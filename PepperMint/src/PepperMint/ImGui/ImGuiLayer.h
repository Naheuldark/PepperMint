#pragma once

#include "PepperMint/Layer.h"

namespace PepperMint {

class PM_API ImGuiLayer : public Layer {
public:
	ImGuiLayer() : Layer("ImGuiLayer") {}
	~ImGuiLayer() = default;

	void onAttach() override;
	void onDetach() override;
	void onUpdate() override;
	void onEvent(Event& iEvent) override;

private:
	float _time = 0.0f;
};

}
