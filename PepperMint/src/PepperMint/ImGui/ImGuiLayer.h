#pragma once

#include "PepperMint/Layer.h"

#include "PepperMint/Events/MouseEvent.h"
#include "PepperMint/Events/KeyEvent.h"
#include "PepperMint/Events/ApplicationEvent.h"

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
	bool onMouseButtonPressedEvent(MouseButtonPressedEvent& iEvent);
	bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& iEvent);
	bool onMouseMovedEvent(MouseMovedEvent& iEvent);
	bool onMouseScrolledEvent(MouseScrolledEvent& iEvent);
	bool onKeyPressedEvent(KeyPressedEvent& iEvent);
	bool onKeyReleasedEvent(KeyReleasedEvent& iEvent);
	bool onKeyTypedEvent(KeyTypedEvent& iEvent);
	bool onWindowResizeEvent(WindowResizeEvent& iEvent);

private:
	float _time = 0.0f;
};

}
