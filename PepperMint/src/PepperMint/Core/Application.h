#pragma once

#include "PepperMint/Core/Base.h"
#include "PepperMint/Core/Window.h"
#include "PepperMint/Core/LayerStack.h"
#include "PepperMint/Core/Timestep.h"
#include "PepperMint/Events/ApplicationEvent.h"
#include "PepperMint/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace PepperMint {

class Application {
public:
	Application(const std::string& iName = "PepperMint App");
	virtual ~Application();

	void onEvent(Event& iEvent);

	void pushLayer(Ref<Layer> iLayer);
	void pushOverlay(Ref<Layer> iOverlay);

	void close();

	inline static Application& Get() { return *sInstance; }
	inline Window& window() { return *_window; }

private:
	void run();

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
	friend int ::main(int argc, char** argv);
};

// To be defined in CLIENT
Application* CreateApplication();
}
