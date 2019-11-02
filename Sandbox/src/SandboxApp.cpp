#include <PepperMint.h>

class ExampleLayer : public PepperMint::Layer {
public:
	ExampleLayer() : Layer("Example") {}
	~ExampleLayer() = default;

	void onUpdate() override {
		PM_INFO("ExampleLayer::Update");
	}

	void onEvent(PepperMint::Event& iEvent) override {
		PM_TRACE("{0}", iEvent);
	}
};


class Sandbox : public PepperMint::Application {
public:
	Sandbox() { 
		pushLayer(new ExampleLayer());
		pushOverlay(new PepperMint::ImGuiLayer());
	}

	~Sandbox() = default;
};

PepperMint::Application* PepperMint::CreateApplication() {
	return new Sandbox();
}