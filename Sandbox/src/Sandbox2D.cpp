#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Platform/OpenGL/OpenGLShader.h>

void Sandbox2D::onAttach() {
	////////////
	// Square //
	////////////

	_squareVA = PepperMint::VertexArray::Create();

	// Vertex Buffer
	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	PepperMint::Ref<PepperMint::VertexBuffer> squareVB;
	squareVB = PepperMint::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->setLayout({
		{ PepperMint::ShaderDataType::FLOAT3, "iPosition" },
	});
	_squareVA->addVertexBuffer(squareVB);

	// Index Buffer
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	PepperMint::Ref<PepperMint::IndexBuffer> squareIB;
	squareIB = PepperMint::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	_squareVA->setIndexBuffer(squareIB);

	/////////////
	// Shaders //
	/////////////

	_flatShader = PepperMint::Shader::Create("assets/shaders/Flat.glsl");
}

void Sandbox2D::onDetach() {}

void Sandbox2D::onUpdate(PepperMint::Timestep iTimestep) {
	// Update
	_cameraController.onUpdate(iTimestep);

	// Render
	PepperMint::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	PepperMint::RenderCommand::Clear();

	PepperMint::Renderer::BeginScene(_cameraController.camera());
	{
		std::dynamic_pointer_cast<PepperMint::OpenGLShader>(_flatShader)->bind();
		std::dynamic_pointer_cast<PepperMint::OpenGLShader>(_flatShader)->uploadUniformFloat4("uColor", _squareColor);

		PepperMint::Renderer::Submit(_flatShader, _squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	}
	PepperMint::Renderer::EndScene();
}

void Sandbox2D::onImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(_squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(PepperMint::Event& iEvent) {
	_cameraController.onEvent(iEvent);
}
