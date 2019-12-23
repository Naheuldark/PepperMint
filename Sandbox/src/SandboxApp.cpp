#include <PepperMint.h>

#include <imgui/imgui.h>

class ExampleLayer : public PepperMint::Layer {
public:
	ExampleLayer() : Layer("Example"), _camera(-1.6f, 1.6f, -0.9f, 0.9f) {
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

			out vec4 vColor;

			void main() {
				vColor = iColor;
				gl_Position = uViewProjection * vec4(iPosition, 1.0);
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

		_triangleShader.reset(new PepperMint::Shader(triangleVS, triangleFS));


		////////////
		// Square //
		////////////

		_squareVA.reset(PepperMint::VertexArray::Create());

		// Vertex Buffer
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
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

			void main() {
				gl_Position = uViewProjection * vec4(iPosition, 1.0);	
			}
		)");

		std::string squareFS(R"(
			#version 330 core
			
			layout(location = 0) out vec4 oColor;

			void main() {
				oColor = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)");

		_squareShader.reset(new PepperMint::Shader(squareVS, squareFS));
	}

	~ExampleLayer() = default;

	void onUpdate() override {
		PepperMint::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		PepperMint::RenderCommand::Clear();

		_camera.setPosition({ 0.5f, 0.5f, 0.0f });
		_camera.setRotation(45.0f);

		PepperMint::Renderer::BeginScene(_camera);

		PepperMint::Renderer::Submit(_squareShader, _squareVA);
		PepperMint::Renderer::Submit(_triangleShader, _triangleVA);

		PepperMint::Renderer::EndScene();
	}

	void onEvent(PepperMint::Event& iEvent) override {

	}

	void onImGuiRender() override {

	}

private:

	std::shared_ptr<PepperMint::Shader> _triangleShader;
	std::shared_ptr<PepperMint::VertexArray> _triangleVA;

	std::shared_ptr<PepperMint::Shader> _squareShader;
	std::shared_ptr<PepperMint::VertexArray> _squareVA;

	PepperMint::OrthographicCamera _camera;
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