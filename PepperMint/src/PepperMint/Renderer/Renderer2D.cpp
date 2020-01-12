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
	PM_PROFILE_FUNCTION();

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

	sData->textureShader = Shader::Create("assets/shaders/Texture.glsl");
	sData->textureShader->bind();
	sData->textureShader->setInt("uTexture", 0);

	//////////////
	// Textures //
	//////////////

	sData->whiteTexture = Texture2D::Create(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	sData->whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));
}

void Renderer2D::Shutdown() {
	PM_PROFILE_FUNCTION();
}

void Renderer2D::BeginScene(const OrthographicCamera& iCamera) {
	PM_PROFILE_FUNCTION();

	sData->textureShader->bind();
	sData->textureShader->setMat4("uViewProjection", iCamera.viewProjectionMatrix());
}

void Renderer2D::EndScene() {
	PM_PROFILE_FUNCTION();
}

void Renderer2D::DrawQuad(const glm::vec2& iPosition,
						  float iRotation,
						  const glm::vec2& iScale,
						  Ref<Texture2D> iTexture,
						  const glm::vec4& iColor) {
	DrawQuad({ iPosition.x, iPosition.y, 0.0f }, iRotation, iScale, iTexture, iColor);
}

void Renderer2D::DrawQuad(const glm::vec3& iPosition,
						  float iRotation,
						  const glm::vec2& iScale,
						  Ref<Texture2D> iTexture,
						  const glm::vec4& iColor) {
	PM_PROFILE_FUNCTION();

	sData->textureShader->setFloat4("uColor", iColor);

	if (iTexture) {
		iTexture->bind();
	} else {
		sData->whiteTexture->bind();
	}

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), iPosition) * 
						  glm::rotate(glm::mat4(1.0f), iRotation, { 0.0f, 0.0f, 1.0f }) *
						  glm::scale(glm::mat4(1.0f), { iScale.x, iScale.y, 1.0f });
	sData->textureShader->setMat4("uTransform", transform);

	sData->quadVertexArray->bind();
	RenderCommand::DrawIndexed(sData->quadVertexArray);
}
}