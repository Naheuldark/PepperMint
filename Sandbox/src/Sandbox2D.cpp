#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Platform/OpenGL/OpenGLShader.h>

void Sandbox2D::onAttach() {}

void Sandbox2D::onDetach() {}

void Sandbox2D::onUpdate(PepperMint::Timestep iTimestep) {
	// Update
	_cameraController.onUpdate(iTimestep);

	// Render
	PepperMint::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	PepperMint::RenderCommand::Clear();

	PepperMint::Renderer2D::BeginScene(_cameraController.camera());
	PepperMint::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, _squareColor);
	PepperMint::Renderer2D::EndScene();
}

void Sandbox2D::onImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(_squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(PepperMint::Event& iEvent) {
	_cameraController.onEvent(iEvent);
}
