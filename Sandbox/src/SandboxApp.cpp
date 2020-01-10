#include <PepperMint.h>
#include <PepperMint/Core/EntryPoint.h>

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Platform/OpenGL/OpenGLShader.h>

#include "Sandbox2D.h"

class ExampleLayer : public PepperMint::Layer {
public:
	ExampleLayer() : Layer("Example"), _cameraController(1280.0f / 720.0f, true) {
		////////////
		// Square //
		////////////

		_squareVA = PepperMint::VertexArray::Create();

		// Vertex Buffer
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f
		};

		PepperMint::Ref<PepperMint::VertexBuffer> squareVB;
		squareVB = PepperMint::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->setLayout({
			{ PepperMint::ShaderDataType::FLOAT3, "iPosition" },
			{ PepperMint::ShaderDataType::FLOAT2, "iTexCoord" },
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

		auto flatColorShader = _shaderLibrary.load("assets/shaders/Flat.glsl");
		auto textureShader = _shaderLibrary.load("assets/shaders/Texture.glsl");

		//////////////
		// Textures //
		//////////////

		_texture = PepperMint::Texture2D::Create("assets/textures/Checkerboard.png");
		_chernoTexture = PepperMint::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<PepperMint::OpenGLShader>(textureShader)->bind();
		std::dynamic_pointer_cast<PepperMint::OpenGLShader>(textureShader)->uploadUniformInt("uTexture", 0);
	}

	~ExampleLayer() = default;

	void onUpdate(PepperMint::Timestep iTimestep) override {
		// Update
		_cameraController.onUpdate(iTimestep);

		// Render
		PepperMint::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		PepperMint::RenderCommand::Clear();

		PepperMint::Renderer::BeginScene(_cameraController.camera());

		auto textureShader = _shaderLibrary.get("Texture");

		auto flatColorShader = _shaderLibrary.get("Flat");
		std::dynamic_pointer_cast<PepperMint::OpenGLShader>(flatColorShader)->bind();
		std::dynamic_pointer_cast<PepperMint::OpenGLShader>(flatColorShader)->uploadUniformFloat3("uColor", _squareColor);

		// Draw squares
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				PepperMint::Renderer::Submit(flatColorShader, _squareVA, transform);
			}
		}

		_texture->bind();
		PepperMint::Renderer::Submit(textureShader, _squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		_chernoTexture->bind();
		PepperMint::Renderer::Submit(textureShader, _squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		PepperMint::Renderer::EndScene();
	}

	void onImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(_squareColor));
		ImGui::End();
	}

	void onEvent(PepperMint::Event& iEvent) override {
		_cameraController.onEvent(iEvent);
	}

private:
	PepperMint::ShaderLibrary _shaderLibrary;

	PepperMint::Ref<PepperMint::VertexArray> _squareVA;
	glm::vec3 _squareColor = { 0.2f, 0.3f, 0.8f };

	PepperMint::Ref<PepperMint::Texture2D> _texture, _chernoTexture;

	PepperMint::OrthographicCameraController _cameraController;
};


class Sandbox : public PepperMint::Application {
public:
	Sandbox() { 
		// pushLayer(PepperMint::CreateRef<ExampleLayer>());
		pushLayer(PepperMint::CreateRef<Sandbox2D>());
	}

	~Sandbox() = default;
};

PepperMint::Application* PepperMint::CreateApplication() {
	return new Sandbox();
}