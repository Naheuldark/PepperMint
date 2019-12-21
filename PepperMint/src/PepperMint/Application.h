#pragma once

#include "PepperMint/Core.h"
#include "PepperMint/Window.h"
#include "PepperMint/Events/ApplicationEvent.h"
#include "PepperMint/LayerStack.h"

#include "PepperMint/ImGui/ImGuiLayer.h"

namespace PepperMint {

class Application {
public:
	Application();
	virtual ~Application() = default;

	void onEvent(Event& iEvent);

	void pushLayer(Layer* iLayer);
	void pushOverlay(Layer* iOverlay);

	void run();

	inline static Application& Get() { return *kInstance; }
	inline Window& window() { return *_window; }

private:
	bool onWindowCloseEvent(WindowCloseEvent& iEvent);

private:
	std::unique_ptr<Window> _window;
	ImGuiLayer* _imguiLayer;
	bool _running = true;
	LayerStack _layerStack;

	static Application* kInstance;

	unsigned int _vertexArray;
	unsigned int _vertexBuffer;
	unsigned int _indexBuffer;
};

// To be defined in CLIENT
Application* CreateApplication();
}
