#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "Core/Timestep.h"
#include "ImGui/ImGuiLayer.h"

namespace PepperMint {

class Application {
public:
	Application();
	virtual ~Application() = default;

	void onEvent(Event& iEvent);

	void pushLayer(Layer* iLayer);
	void pushOverlay(Layer* iOverlay);

	void run();

	inline static Application& Get() { return *sInstance; }
	inline Window& window() { return *_window; }

private:
	bool onWindowCloseEvent(WindowCloseEvent& iEvent);

private:
	std::unique_ptr<Window> _window;
	ImGuiLayer* _imguiLayer;
	bool _running = true;
	LayerStack _layerStack;
	float _lastFrameTime = 0.0f;

private:
	static Application* sInstance;
};

// To be defined in CLIENT
Application* CreateApplication();
}
