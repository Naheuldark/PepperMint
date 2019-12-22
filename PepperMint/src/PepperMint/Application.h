#pragma once

#include "PepperMint/Core.h"
#include "PepperMint/Window.h"
#include "PepperMint/Events/ApplicationEvent.h"
#include "PepperMint/LayerStack.h"

#include "PepperMint/ImGui/ImGuiLayer.h"

#include "PepperMint/Renderer/Shader.h"
#include "PepperMint/Renderer/Buffer.h"
#include "PepperMint/Renderer/VertexArray.h"

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

	static Application* sInstance;

	std::shared_ptr<Shader> _triangleShader;
	std::shared_ptr<VertexArray> _triangleVA;

	std::shared_ptr<Shader> _squareShader;
	std::shared_ptr<VertexArray> _squareVA;
};

// To be defined in CLIENT
Application* CreateApplication();
}
