#include "pmpch.h"

#include "Application.h"

#include "Input.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace PepperMint {

Application* Application::kInstance = nullptr;

Application::Application() {
	PM_CORE_ASSERT(!kInstance, "Application already exists!")
	kInstance = this;

	_window = std::unique_ptr<Window>(Window::Create());
	_window->setEventCallback(PM_BIND_EVENT_FN(Application::onEvent));

	_imguiLayer = new ImGuiLayer();
	pushOverlay(_imguiLayer);
}

void Application::run() {
	while (_running) {
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto&& layer : _layerStack) {
			layer->onUpdate();
		}

		_imguiLayer->begin();
		for (auto&& layer : _layerStack) {
			layer->onImGuiRender();
		}
		_imguiLayer->end();

		_window->onUpdate();
	}
}

void Application::pushLayer(Layer* iLayer) {
	_layerStack.pushLayer(iLayer);
	iLayer->onAttach();
}

void Application::pushOverlay(Layer* iOverlay) {
	_layerStack.pushOverlay(iOverlay);
	iOverlay->onAttach();
}

void Application::onEvent(Event& iEvent) {
	EventDispatcher dispatcher(iEvent);
	dispatcher.dispatch<WindowCloseEvent>(PM_BIND_EVENT_FN(Application::onWindowCloseEvent));

	PM_CORE_TRACE("{0}", iEvent);

	for (auto it = _layerStack.end(); it != _layerStack.begin(); ) {
		(*--it)->onEvent(iEvent);
		if (iEvent.handled()) break;
	}
}

bool Application::onWindowCloseEvent(WindowCloseEvent& iEvent) {
	_running = false;
	return true;
}

}