#include "FlappyShipLayer.h"

const float kCameraHeight = 8.0f;

FlappyShipLayer::FlappyShipLayer() : Layer("FlappyShip") {
	auto& window = PepperMint::Application::Get().window();
	updateCamera(window.width(), window.height());
}

void FlappyShipLayer::onAttach() {}

void FlappyShipLayer::onDetach() {}

void FlappyShipLayer::onUpdate(PepperMint::Timestep iTimestep) {
	// Update
	_camera->setPosition({ 0.0f, 0.0f, 0.0f });

	// Render
	PepperMint::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	PepperMint::RenderCommand::Clear();

	PepperMint::Renderer2D::BeginScene(*_camera);
	{
		PepperMint::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 1.0f, 2.5f }, { 0.8f, 0.3f, 0.8f, 1.0f });
	}
	PepperMint::Renderer2D::EndScene();
}

void FlappyShipLayer::onImGuiRender() {}

void FlappyShipLayer::onEvent(PepperMint::Event& iEvent) {
	PepperMint::EventDispatcher dispatcher(iEvent);
	dispatcher.dispatch<PepperMint::WindowResizeEvent>(PM_BIND_EVENT_FN(FlappyShipLayer::onWindowResize));
	dispatcher.dispatch<PepperMint::MouseButtonPressedEvent>(PM_BIND_EVENT_FN(FlappyShipLayer::onMouseButtonPressed));
}

void FlappyShipLayer::updateCamera(uint32_t iWidth, uint32_t iHeight) {
	float aspectRatio = (float)iWidth / (float)iHeight;

	float bottom = -kCameraHeight;
	float top = kCameraHeight;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;
	_camera = PepperMint::CreateScope<PepperMint::OrthographicCamera>(left, right, bottom, top);
}

bool FlappyShipLayer::onWindowResize(PepperMint::WindowResizeEvent& iEvent) {
	updateCamera(iEvent.width(), iEvent.height());
	return false;
}

bool FlappyShipLayer::onMouseButtonPressed(PepperMint::MouseButtonPressedEvent& iEvent) {
	return false;
}
