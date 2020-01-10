#include "pmpch.h"

#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace PepperMint {

struct Renderer2DStorage {
	Ref<VertexArray> quadVertexArray;
	Ref<Shader> flatColorShader;
};

static Scope<Renderer2DStorage> sData = CreateScope<Renderer2DStorage>();

void Renderer2D::Init() {
	////////////
	// Square //
	////////////

	sData->quadVertexArray = VertexArray::Create();

	// Vertex Buffer
	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	PepperMint::Ref<PepperMint::VertexBuffer> squareVB;
	squareVB = PepperMint::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->setLayout({
		{ PepperMint::ShaderDataType::FLOAT3, "iPosition" },
	});
	sData->quadVertexArray->addVertexBuffer(squareVB);

	// Index Buffer
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	PepperMint::Ref<PepperMint::IndexBuffer> squareIB;
	squareIB = PepperMint::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	sData->quadVertexArray->setIndexBuffer(squareIB);

	/////////////
	// Shaders //
	/////////////

	sData->flatColorShader = PepperMint::Shader::Create("assets/shaders/Flat.glsl");
}

void Renderer2D::Shutdown() {}

void Renderer2D::BeginScene(const OrthographicCamera& iCamera) {
	std::dynamic_pointer_cast<OpenGLShader>(sData->flatColorShader)->bind();
	std::dynamic_pointer_cast<OpenGLShader>(sData->flatColorShader)->uploadUniformMat4("uViewProjection", iCamera.viewProjectionMatrix());
	std::dynamic_pointer_cast<OpenGLShader>(sData->flatColorShader)->uploadUniformMat4("uTransform", glm::mat4(1.0f));
}

void Renderer2D::EndScene() {}

void Renderer2D::DrawQuad(const glm::vec2& iPosition, const glm::vec2& iSize, const glm::vec4& iColor) {
	DrawQuad({ iPosition.x, iPosition.y, 0.0f }, iSize, iColor);
}

void Renderer2D::DrawQuad(const glm::vec3& iPosition, const glm::vec2& iSize, const glm::vec4& iColor) {
	std::dynamic_pointer_cast<OpenGLShader>(sData->flatColorShader)->bind();
	std::dynamic_pointer_cast<OpenGLShader>(sData->flatColorShader)->uploadUniformFloat4("uColor", iColor);

	sData->quadVertexArray->bind();
	RenderCommand::DrawIndexed(sData->quadVertexArray);
}
}