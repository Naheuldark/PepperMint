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

	glGenVertexArrays(1, &_vertexArray);
	glBindVertexArray(_vertexArray);

	glGenBuffers(1, &_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

	float vertices[3 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	glGenBuffers(1, &_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

	unsigned int indices[3] = { 0, 1, 2 };
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Application::run() {
	while (_running) {
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(_vertexArray);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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

	//PM_CORE_TRACE("{0}", iEvent);

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