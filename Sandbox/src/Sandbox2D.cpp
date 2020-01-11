#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

template<typename Fn>
class Timer {
public:
	Timer(const char* iName, Fn&& iFunc) : 
		_name(iName), _func(iFunc), _stopped(false) {
		_startTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer() {
		if (!_stopped) {
			Stop();
		}
	}

	void Stop() {
		auto&& endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		_stopped = true;

		float duration = (end - start) * 0.001f;
		_func({ _name, duration });
	}

private:
	const char* _name;
	Fn _func;
	std::chrono::time_point<std::chrono::steady_clock> _startTimepoint;
	bool _stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { _profileResults.push_back(profileResult); })

void Sandbox2D::onAttach() {
	_checkerboardTexture = PepperMint::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach() {}

void Sandbox2D::onUpdate(PepperMint::Timestep iTimestep) {
	PROFILE_SCOPE("Sandbox2D::onUpdate");

	// Update
	{
		PROFILE_SCOPE("CameraController::onUpdate");
		_cameraController.onUpdate(iTimestep);
	}

	// Render
	{
		PROFILE_SCOPE("Renderer Preparation");
		PepperMint::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		PepperMint::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		PepperMint::Renderer2D::BeginScene(_cameraController.camera());
		PepperMint::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, _squareColor);
		PepperMint::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.8f, 0.3f, 0.8f, 1.0f });
		PepperMint::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, _checkerboardTexture);
		PepperMint::Renderer2D::EndScene();
	}
}

void Sandbox2D::onImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(_squareColor));

	for (auto&& result : _profileResults) {
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.name);
		ImGui::Text(label, result.time);
	}
	_profileResults.clear();

	ImGui::End();
}

void Sandbox2D::onEvent(PepperMint::Event& iEvent) {
	_cameraController.onEvent(iEvent);
}
