#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

#include "Events/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

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
