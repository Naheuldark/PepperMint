#pragma once

#include <PepperMint.h>

class ExampleLayer : public PepperMint::Layer {
public:
	ExampleLayer();
	~ExampleLayer() override = default;

	virtual void onAttach() override;
	virtual void onDetach() override;
	void onUpdate(PepperMint::Timestep iTimestep) override;
	virtual void onImGuiRender() override;
	void onEvent(PepperMint::Event& iEvent) override;

private:
	PepperMint::ShaderLibrary _shaderLibrary;

	PepperMint::Ref<PepperMint::VertexArray> _squareVA;
	glm::vec3 _squareColor = { 0.2f, 0.3f, 0.8f };

	PepperMint::Ref<PepperMint::Texture2D> _texture, _chernoTexture;

	PepperMint::OrthographicCameraController _cameraController;
};