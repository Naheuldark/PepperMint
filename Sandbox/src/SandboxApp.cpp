#include <PepperMint.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public PepperMint::Layer {
public:
	ExampleLayer() : Layer("Example"), _camera(-1.6f, 1.6f, -0.9f, 0.9f), _cameraPosition(0.0f) {
		////////////
		// Square //
		////////////

		_squareVA.reset(PepperMint::VertexArray::Create());

		// Vertex Buffer
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f
		};

		PepperMint::Ref<PepperMint::VertexBuffer> squareVB;
		squareVB.reset(PepperMint::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->setLayout({
			{ PepperMint::ShaderDataType::FLOAT3, "iPosition" },
			{ PepperMint::ShaderDataType::FLOAT2, "iTexCoord" },
		});
		_squareVA->addVertexBuffer(squareVB);

		// Index Buffer
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		PepperMint::Ref<PepperMint::IndexBuffer> squareIB;
		squareIB.reset(PepperMint::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		_squareVA->setIndexBuffer(squareIB);

		/////////////
		// Shaders //
		/////////////

		// Flat Color Shader
		std::string flatColorVertexShader(R"(
			#version 330 core

			layout(location = 0) in vec3 iPosition;

			uniform mat4 uViewProjection;
			uniform mat4 uTransform;

			void main() {
				gl_Position = uViewProjection * uTransform * vec4(iPosition, 1.0);
			}
		)");

		std::string flatColorFragmentShader(R"(
			#version 330 core

			layout(location = 0) out vec4 oColor;

			in vec4 vColor;

			uniform vec3 uColor;

			void main() {
				oColor = vec4(uColor, 1.0);
			}
		)");

		_flatColorShader.reset(PepperMint::Shader::Create(flatColorVertexShader, flatColorFragmentShader));


		// Texture Shader
		std::string textureVertexShader(R"(
			#version 330 core
			
			layout(location = 0) in vec3 iPosition;
			layout(location = 1) in vec2 iTexCoord;

			uniform mat4 uViewProjection;
			uniform mat4 uTransform;

			out vec2 vTexCoord;

			void main() {
				vTexCoord = iTexCoord;
				gl_Position = uViewProjection * uTransform * vec4(iPosition, 1.0);	
			}
		)");

		std::string textureFragmentShader(R"(
			#version 330 core
			
			layout(location = 0) out vec4 oColor;

			in vec2 vTexCoord;

			uniform sampler2D uTexture;

			void main() {
				oColor = texture(uTexture, vTexCoord);
			}
		)");

		_textureShader.reset(PepperMint::Shader::Create(textureVertexShader, textureFragmentShader));

		//////////////
		// Textures //
		//////////////

		// Checkerboard
		_texture = PepperMint::Texture2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<PepperMint::OpenGLShader>(_textureShader)->bind();
		std::dynamic_pointer_cast<PepperMint::OpenGLShader>(_textureShader)->uploadUniformInt("uTexture", 0);
	}

	~ExampleLayer() = default;

	void onUpdate(PepperMint::Timestep iTimestep) override {
		// Event polling
		if (PepperMint::Input::IsKeyPressed(PM_KEY_LEFT))
			_cameraPosition.x -= _cameraMoveSpeed * iTimestep;
		else if (PepperMint::Input::IsKeyPressed(PM_KEY_RIGHT))
			_cameraPosition.x += _cameraMoveSpeed * iTimestep;

		if (PepperMint::Input::IsKeyPressed(PM_KEY_UP))
			_cameraPosition.y += _cameraMoveSpeed * iTimestep;
		else if (PepperMint::Input::IsKeyPressed(PM_KEY_DOWN))
			_cameraPosition.y -= _cameraMoveSpeed * iTimestep;

		if (PepperMint::Input::IsKeyPressed(PM_KEY_A))
			_cameraRotation += _cameraRotationSpeed * iTimestep;
		if (PepperMint::Input::IsKeyPressed(PM_KEY_D))
			_cameraRotation -= _cameraRotationSpeed * iTimestep;

		PepperMint::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		PepperMint::RenderCommand::Clear();

		// Setup Orthographic Camera
		_camera.setPosition(_cameraPosition);
		_camera.setRotation(_cameraRotation);

		PepperMint::Renderer::BeginScene(_camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<PepperMint::OpenGLShader>(_flatColorShader)->bind();
		std::dynamic_pointer_cast<PepperMint::OpenGLShader>(_flatColorShader)->uploadUniformFloat3("uColor", _squareColor);

		// Draw squares
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				PepperMint::Renderer::Submit(_flatColorShader, _squareVA, transform);
			}
		}

		_texture->bind();
		PepperMint::Renderer::Submit(_textureShader, _squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		PepperMint::Renderer::EndScene();
	}

	void onImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(_squareColor));
		ImGui::End();
	}

private:

	PepperMint::Ref<PepperMint::Shader> _flatColorShader;
	PepperMint::Ref<PepperMint::Shader> _textureShader;

	PepperMint::Ref<PepperMint::VertexArray> _squareVA;

	PepperMint::Ref<PepperMint::Texture2D> _texture;

	PepperMint::OrthographicCamera _camera;

	glm::vec3 _cameraPosition;
	float _cameraMoveSpeed = 5.0f;

	float _cameraRotation = 0.0f;
	float _cameraRotationSpeed = 180.0f;

	glm::vec3 _squareColor = { 0.2f, 0.3f, 0.8f };
};


class Sandbox : public PepperMint::Application {
public:
	Sandbox() { 
		pushLayer(new ExampleLayer());
	}

	~Sandbox() = default;
};

PepperMint::Application* PepperMint::CreateApplication() {
	return new Sandbox();
}