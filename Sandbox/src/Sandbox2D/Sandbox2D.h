#pragma once

#include <PepperMint.h>

class Sandbox2D : public PepperMint::Layer {
public:
	Sandbox2D() : Layer("Sandbox2D"), _cameraController(1280.0f / 720.0f, true) {}
	~Sandbox2D() override = default;

	void onAttach() override;
	void onDetach() override;
	void onUpdate(PepperMint::Timestep iTimestep) override;
	void onImGuiRender() override;
	void onEvent(PepperMint::Event& iEvent) override;

private:
	PepperMint::OrthographicCameraController _cameraController;

	PepperMint::Ref<PepperMint::VertexArray> _squareVA;
	PepperMint::Ref<PepperMint::Shader> _flatColorShader;

	PepperMint::Ref<PepperMint::Texture2D> _checkerboardTexture;

	glm::vec4 _squareColor = { 0.2f, 0.3f, 0.8f, 1.0 };
};
