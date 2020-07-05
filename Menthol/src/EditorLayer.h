#pragma once

#include <PepperMint.h>

namespace Menthol {

class EditorLayer : public PepperMint::Layer {
public:
	EditorLayer() : Layer("Menthol"), _cameraController(1280.0f / 720.0f, true) {}
	~EditorLayer() override = default;

	void onAttach() override;
	void onDetach() override;
	void onUpdate(PepperMint::Timestep iTimestep) override;
	void onImGuiRender() override;
	void onEvent(PepperMint::Event& iEvent) override;

private:
	PepperMint::OrthographicCameraController _cameraController;

	PepperMint::Ref<PepperMint::VertexArray> _squareVA;
	PepperMint::Ref<PepperMint::Shader> _flatColorShader;
	PepperMint::Ref<PepperMint::FrameBuffer> _frameBuffer;

	PepperMint::Ref<PepperMint::Texture2D> _checkerboardTexture;

	glm::vec2 _viewportSize = { 0.0f, 0.0f };
	bool _viewportFocused = false;
	bool _viewportHovered = false;

	glm::vec4 _squareColor = { 0.2f, 0.3f, 0.8f, 1.0 };
};

}