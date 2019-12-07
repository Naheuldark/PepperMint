#include <PepperMint.h>

#include <imgui/imgui.h>

class ExampleLayer : public PepperMint::Layer {
public:
	ExampleLayer() : Layer("Example") {}
	~ExampleLayer() = default;

	void onUpdate() override {
		if (PepperMint::Input::IsKeyPressed(PM_KEY_TAB)) {
			PM_TRACE("Tab key is pressed (poll)")
		}
	}

	void onEvent(PepperMint::Event& iEvent) override {
		if (iEvent.eventType() == PepperMint::EventType::KEY_PRESSED) {
			PepperMint::KeyPressedEvent& e = (PepperMint::KeyPressedEvent&)iEvent;
			if (e.keyCode() == PM_KEY_TAB) {
				PM_TRACE("Tab key is pressed (event)")
			}
			PM_TRACE("{0}", (char)e.keyCode())
		}
	}

	void onImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::End();
	}
};


class Sandbox : public PepperMint::Application {
public:
	Sandbox() { 
		pushLayer(new ExampleLayer());
	}

	~Sandbox() = default;
};

PepperMint::Application* PepperMint::CreateApplication() {
	return new Sandbox();
}