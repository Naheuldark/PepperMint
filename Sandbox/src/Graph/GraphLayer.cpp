#include "GraphLayer.h"

///////////////
// Constants //
///////////////
const float kCameraHeight = 8.0f;

GraphLayer::GraphLayer() {
	auto& window = PepperMint::Application::Get().window();
	updateCamera(window.width(), window.height());
}

void GraphLayer::onAttach() {
	ImGuiIO io = ImGui::GetIO();
	_font = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", 120.0f);
}

void GraphLayer::onDetach() {}

void GraphLayer::onUpdate(PepperMint::Timestep iTimestep) {
	_time += iTimestep;

	if ((int)(_time * 100.f) % 50 > 25) {
		_send = !_send;
	}

	PepperMint::Renderer2D::BeginScene(*_camera);
	if (_send) {
		PepperMint::Renderer2D::DrawQuad({ -10.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, 1.0f, nullptr, { 1,0,0,1 });
	}
	PepperMint::Renderer2D::EndScene();
}

void GraphLayer::onImGuiRender() {
	std::string text = "Hello, World!!";
	auto pos = ImGui::GetWindowPos();
	pos.y += 50.0f;
	ImGui::GetForegroundDrawList()->AddText(_font, 48.0f, pos, 0xffffffff, text.c_str());
}

void GraphLayer::onEvent(PepperMint::Event& iEvent) {
	PepperMint::EventDispatcher dispatcher(iEvent);
	dispatcher.dispatch<PepperMint::WindowResizeEvent>(PM_BIND_EVENT_FN(GraphLayer::onWindowResize));
}

bool GraphLayer::onWindowResize(PepperMint::WindowResizeEvent& iEvent) {
	updateCamera(iEvent.width(), iEvent.height());
	return false;
}

void GraphLayer::updateCamera(uint32_t iWidth, uint32_t iHeight) {
	float aspectRatio = (float)iWidth / (float)iHeight;

	float bottom = -kCameraHeight;
	float top = kCameraHeight;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;
	_camera = PepperMint::CreateScope<PepperMint::OrthographicCamera>(left, right, bottom, top);
}
