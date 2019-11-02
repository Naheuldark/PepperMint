#pragma once

#include "PepperMint/Core.h"
#include "PepperMint/Window.h"
#include "PepperMint/Events/ApplicationEvent.h"
#include "PepperMint/LayerStack.h"

namespace PepperMint {

class PM_API Application {
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
	bool onWindowClose(WindowCloseEvent& iEvent);

private:
	std::unique_ptr<Window> _window;
	bool _running = true;
	LayerStack _layerStack;

	static Application* kInstance;
};

// To be defined in CLIENT
Application* CreateApplication();
}
