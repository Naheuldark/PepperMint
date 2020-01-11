#pragma once

#include "PepperMint/Core/Layer.h"
#include "PepperMint/Events/MouseEvent.h"
#include "PepperMint/Events/KeyEvent.h"
#include "PepperMint/Events/ApplicationEvent.h"

namespace PepperMint {

class ImGuiLayer : public Layer {
public:
	ImGuiLayer() : Layer("ImGuiLayer") {}
	~ImGuiLayer() = default;

	void onAttach() override;
	void onDetach() override;
	void onImGuiRender() override;

	void begin();
	void end();

private:
	float _time = 0.0f;
};

}
