#include <PepperMint.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public PepperMint::Layer {
public:
	ExampleLayer() : Layer("Example"), _camera(-1.6f, 1.6f, -0.9f, 0.9f), _cameraPosition(0.0f) {
		//////////////
		// Triangle //
		//////////////

		_triangleVA.reset(PepperMint::VertexArray::Create());

		// Vertex Buffer
		float triangleVertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,		0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<PepperMint::VertexBuffer> triangleVB;
		triangleVB.reset(PepperMint::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices)));
		triangleVB->setLayout({
			{ PepperMint::ShaderDataType::FLOAT3, "iPosition" },
			{ PepperMint::ShaderDataType::FLOAT4, "iColor"}
		});
		_triangleVA->addVertexBuffer(triangleVB);

		// Index Buffer
		uint32_t triangleIndices[3] = { 0, 1, 2 };

		std::shared_ptr<PepperMint::IndexBuffer> triangleIB;
		triangleIB.reset(PepperMint::IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t)));
		_triangleVA->setIndexBuffer(triangleIB);

		// Shader
		std::string triangleVS(R"(
			#version 330 core

			layout(location = 0) in vec3 iPosition;
			layout(location = 1) in vec4 iColor;

			uniform mat4 uViewProjection;
			uniform mat4 uTransform;

			out vec4 vColor;

			void main() {
				vColor = iColor;
				gl_Position = uViewProjection * uTransform * vec4(iPosition, 1.0);
			}
		)");

		std::string triangleFS(R"(
			#version 330 core

			layout(location = 0) out vec4 oColor;

			in vec4 vColor;

			void main() {
				oColor = vColor;
			}
		)");

		_triangleShader.reset(PepperMint::Shader::Create(triangleVS, triangleFS));


		////////////
		// Square //
		////////////

		_squareVA.reset(PepperMint::VertexArray::Create());

		// Vertex Buffer
		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<PepperMint::VertexBuffer> squareVB;
		squareVB.reset(PepperMint::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->setLayout({
			{ PepperMint::ShaderDataType::FLOAT3, "iPosition" },
		});
		_squareVA->addVertexBuffer(squareVB);

		// Index Buffer
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<PepperMint::IndexBuffer> squareIB;
		squareIB.reset(PepperMint::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		_squareVA->setIndexBuffer(squareIB);

		// Shader
		std::string squareVS(R"(
			#version 330 core
			
			layout(location = 0) in vec3 iPosition;

			uniform mat4 uViewProjection;
			uniform mat4 uTransform;

			void main() {
				gl_Position = uViewProjection * uTransform * vec4(iPosition, 1.0);	
			}
		)");

		std::string squareFS(R"(
			#version 330 core
			
			layout(location = 0) out vec4 oColor;

			uniform vec3 uColor;

			void main() {
				oColor = vec4(uColor, 1.0);
			}
		)");

		_squareShader.reset(PepperMint::Shader::Create(squareVS, squareFS));
	}

	~ExampleLayer() = default;

	void onUpdate(PepperMint::Timestep iTimestep) override {
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

		_camera.setPosition(_cameraPosition);
		_camera.setRotation(_cameraRotation);

		PepperMint::Renderer::BeginScene(_camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<PepperMint::OpenGLShader>(_squareShader)->bind();
		std::dynamic_pointer_cast<PepperMint::OpenGLShader>(_squareShader)->uploadUniformFloat3("uColor", _squareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				PepperMint::Renderer::Submit(_squareShader, _squareVA, transform);
			}
		}

		PepperMint::Renderer::Submit(_triangleShader, _triangleVA);

		PepperMint::Renderer::EndScene();
	}

	void onEvent(PepperMint::Event& iEvent) override {

	}

	void onImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(_squareColor));
		ImGui::End();
	}

private:

	std::shared_ptr<PepperMint::Shader> _triangleShader;
	std::shared_ptr<PepperMint::VertexArray> _triangleVA;

	std::shared_ptr<PepperMint::Shader> _squareShader;
	std::shared_ptr<PepperMint::VertexArray> _squareVA;

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