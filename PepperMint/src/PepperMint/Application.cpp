#include "pmpch.h"

#include "Application.h"

#include "Input.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace PepperMint {

Application* Application::sInstance = nullptr;

Application::Application() {
	PM_CORE_ASSERT(!sInstance, "Application already exists!")
	sInstance = this;

	_window = std::unique_ptr<Window>(Window::Create());
	_window->setEventCallback(PM_BIND_EVENT_FN(Application::onEvent));

	_imguiLayer = new ImGuiLayer();
	pushOverlay(_imguiLayer);

	glGenVertexArrays(1, &_vertexArray);
	glBindVertexArray(_vertexArray);

	float vertices[3 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	uint32_t indices[3] = { 0, 1, 2 };
	_indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

	std::string vertexShader(R"(
		#version 330 core

		layout(location = 0) in vec3 iPosition;

		out vec3 vPosition;

		void main() {
			vPosition = iPosition;
			gl_Position = vec4(iPosition, 1.0);
		}
	)");

	std::string fragmentShader(R"(
		#version 330 core

		layout(location = 0) out vec4 oColor;

		in vec3 vPosition;

		void main() {
			oColor = vec4(vPosition * 0.5 + 0.5, 1.0);
		}
	)");

	_shader.reset(new Shader(vertexShader, fragmentShader));
}

void Application::run() {
	while (_running) {
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		_shader->bind();

		glBindVertexArray(_vertexArray);
		glDrawElements(GL_TRIANGLES, _indexBuffer->count(), GL_UNSIGNED_INT, nullptr);

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