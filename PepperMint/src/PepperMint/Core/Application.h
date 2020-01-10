#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Timestep.h"

#include "PepperMint/Events/ApplicationEvent.h"
#include "PepperMint/ImGui/ImGuiLayer.h"

namespace PepperMint {

class Application {
public:
	Application();
	virtual ~Application() = default;

	void onEvent(Event& iEvent);

	void pushLayer(Ref<Layer> iLayer);
	void pushOverlay(Ref<Layer> iOverlay);

	void run();

	inline static Application& Get() { return *sInstance; }
	inline Window& window() { return *_window; }

private:
	bool onWindowClose(WindowCloseEvent& iEvent);
	bool onWindowResize(WindowResizeEvent& iEvent);

private:
	Scope<Window> _window;
	Ref<ImGuiLayer> _imguiLayer;
	bool _running = true;
	bool _minimized = false;
	LayerStack _layerStack;
	float _lastFrameTime = 0.0f;

private:
	static Application* sInstance;
};

// To be defined in CLIENT
Application* CreateApplication();
}
