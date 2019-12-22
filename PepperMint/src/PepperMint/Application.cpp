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

	//////////////
	// Triangle //
	//////////////

	_triangleVA.reset(VertexArray::Create());

	// Vertex Buffer
	float triangleVertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f,		0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f
	};

	std::shared_ptr<VertexBuffer> triangleVB;
	triangleVB.reset(VertexBuffer::Create(triangleVertices, sizeof(triangleVertices)));
	triangleVB->setLayout({
		{ ShaderDataType::FLOAT3, "iPosition" },
		{ ShaderDataType::FLOAT4, "iColor"}
	});
	_triangleVA->addVertexBuffer(triangleVB);

	// Index Buffer
	uint32_t triangleIndices[3] = { 0, 1, 2 };

	std::shared_ptr<IndexBuffer> triangleIB;
	triangleIB.reset(IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t)));
	_triangleVA->setIndexBuffer(triangleIB);

	// Shader
	std::string triangleVS(R"(
		#version 330 core

		layout(location = 0) in vec3 iPosition;
		layout(location = 1) in vec4 iColor;

		out vec4 vColor;

		void main() {
			vColor = iColor;
			gl_Position = vec4(iPosition, 1.0);
		}
	)");

	std::string triangleFS(R"(
		#version 330 core

		layout(location = 0) out vec4 oColor;

		in vec4 vColor;

		void main() {
			oColor = vColor;
		}
	)");

	_triangleShader.reset(new Shader(triangleVS, triangleFS));


	////////////
	// Square //
	////////////

	_squareVA.reset(VertexArray::Create());

	// Vertex Buffer
	float squareVertices[3 * 4] = {
		-0.75f, -0.75f, 0.0f,
		 0.75f, -0.75f, 0.0f,
		 0.75f,  0.75f, 0.0f,
		-0.75f,  0.75f, 0.0f
	};

	std::shared_ptr<VertexBuffer> squareVB;
	squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->setLayout({
		{ ShaderDataType::FLOAT3, "iPosition" },
	});
	_squareVA->addVertexBuffer(squareVB);

	// Index Buffer
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	std::shared_ptr<IndexBuffer> squareIB;
	squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	_squareVA->setIndexBuffer(squareIB);

	// Shader
	std::string squareVS(R"(
		#version 330 core
			
		layout(location = 0) in vec3 iPosition;

		void main() {
			gl_Position = vec4(iPosition, 1.0);	
		}
	)");

	std::string squareFS(R"(
		#version 330 core
			
		layout(location = 0) out vec4 oColor;

		void main() {
			oColor = vec4(0.2, 0.3, 0.8, 1.0);
		}
	)");

	_squareShader.reset(new Shader(squareVS, squareFS));
}

void Application::run() {
	while (_running) {
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw square
		_squareShader->bind();
		_squareVA->bind();
		glDrawElements(GL_TRIANGLES, _squareVA->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);

		// Draw triangle
		_triangleShader->bind();
		_triangleVA->bind();
		glDrawElements(GL_TRIANGLES, _triangleVA->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);

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