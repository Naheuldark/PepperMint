#include "pmpch.h"
#include "PepperMint/Renderer/Renderer2D.h"

#include "PepperMint/Renderer/VertexArray.h"
#include "PepperMint/Renderer/Shader.h"
#include "PepperMint/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace PepperMint {

struct Renderer2DStorage {
	Ref<VertexArray> quadVertexArray;
	Ref<Shader> textureShader;
	Ref<Texture2D> whiteTexture;
};

static Scope<Renderer2DStorage> sData = CreateScope<Renderer2DStorage>();

void Renderer2D::Init() {
	////////////
	// Square //
	////////////

	sData->quadVertexArray = VertexArray::Create();

	// Vertex Buffer
	float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f
	};

	Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->setLayout({
		{ ShaderDataType::FLOAT3, "iPosition" },
		{ ShaderDataType::FLOAT2, "iTexCoord" },
	});
	sData->quadVertexArray->addVertexBuffer(squareVB);

	// Index Buffer
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	sData->quadVertexArray->setIndexBuffer(squareIB);

	/////////////
	// Shaders //
	/////////////

	sData->whiteTexture = Texture2D::Create(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	sData->whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));
	
	sData->textureShader = Shader::Create("assets/shaders/Texture.glsl");
	sData->textureShader->bind();
	sData->textureShader->setInt("uTexture", 0);
}

void Renderer2D::Shutdown() {}

void Renderer2D::BeginScene(const OrthographicCamera& iCamera) {
	sData->textureShader->bind();
	sData->textureShader->setMat4("uViewProjection", iCamera.viewProjectionMatrix());
}

void Renderer2D::EndScene() {}

void Renderer2D::DrawQuad(const glm::vec2& iPosition, const glm::vec2& iSize, const glm::vec4& iColor) {
	DrawQuad({ iPosition.x, iPosition.y, 0.0f }, iSize, iColor);
}

void Renderer2D::DrawQuad(const glm::vec3& iPosition, const glm::vec2& iSize, const glm::vec4& iColor) {
	sData->textureShader->setFloat4("uColor", iColor);
	sData->whiteTexture->bind();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), iPosition) * glm::scale(glm::mat4(1.0f), { iSize.x, iSize.y, 1.0f });
	sData->textureShader->setMat4("uTransform", transform);

	sData->quadVertexArray->bind();
	RenderCommand::DrawIndexed(sData->quadVertexArray);
}

void Renderer2D::DrawQuad(const glm::vec2& iPosition, const glm::vec2& iSize, Ref<Texture2D> iTexture) {
	DrawQuad({ iPosition.x, iPosition.y, 0.0f }, iSize, iTexture);
}
	
void Renderer2D::DrawQuad(const glm::vec3& iPosition, const glm::vec2& iSize, Ref<Texture2D> iTexture) {
	sData->textureShader->setFloat4("uColor", glm::vec4(1.0f));
	iTexture->bind();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), iPosition) * glm::scale(glm::mat4(1.0f), { iSize.x, iSize.y, 1.0f });
	sData->textureShader->setMat4("uTransform", transform);

	sData->quadVertexArray->bind();
	RenderCommand::DrawIndexed(sData->quadVertexArray);
}
}