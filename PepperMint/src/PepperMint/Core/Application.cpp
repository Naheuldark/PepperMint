#include "pmpch.h"

#include "Application.h"

#include "Input.h"
#include "PepperMint/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace PepperMint {

Application* Application::sInstance = nullptr;

Application::Application() {
	PM_CORE_ASSERT(!sInstance, "Application already exists!")
	sInstance = this;

	_window = Scope<Window>(Window::Create());
	_window->setEventCallback(PM_BIND_EVENT_FN(Application::onEvent));

	Renderer::Init();

	_imguiLayer = CreateRef<ImGuiLayer>();
	pushOverlay(_imguiLayer);
}

void Application::run() {
	while (_running) {
		float time = (float)glfwGetTime();
		Timestep timestep = time - _lastFrameTime;
		_lastFrameTime = time;

		if (!_minimized) {
			for (auto&& layer : _layerStack) {
				layer->onUpdate(timestep);
			}
		}

		_imguiLayer->begin();
		for (auto&& layer : _layerStack) {
			layer->onImGuiRender();
		}
		_imguiLayer->end();

		_window->onUpdate();
	}
}

void Application::pushLayer(Ref<Layer> iLayer) {
	_layerStack.pushLayer(iLayer);
}

void Application::pushOverlay(Ref<Layer> iOverlay) {
	_layerStack.pushOverlay(iOverlay);
}

void Application::onEvent(Event& iEvent) {
	EventDispatcher dispatcher(iEvent);
	dispatcher.dispatch<WindowCloseEvent>(PM_BIND_EVENT_FN(Application::onWindowClose));
	dispatcher.dispatch<WindowResizeEvent>(PM_BIND_EVENT_FN(Application::onWindowResize));

	//PM_CORE_TRACE("{0}", iEvent);

	for (auto it = _layerStack.end(); it != _layerStack.begin(); ) {
		(*--it)->onEvent(iEvent);
		if (iEvent.handled()) break;
	}
}

bool Application::onWindowClose(WindowCloseEvent& iEvent) {
	_running = false;
	return true;
}

bool Application::onWindowResize(WindowResizeEvent& iEvent) {
	if (iEvent.width() == 0 || iEvent.height() == 0) {
		_minimized = true;
		return false;
	}

	_minimized = false;
	Renderer::OnWindowResize(iEvent.width(), iEvent.height());

	return false;
}
}