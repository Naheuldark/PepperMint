#include "pmpch.h"
#include "PepperMint/Renderer/Renderer2D.h"

#include "PepperMint/Renderer/VertexArray.h"
#include "PepperMint/Renderer/Shader.h"
#include "PepperMint/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace PepperMint {

struct QuadVertex {
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 texCoord;
	// TODO: texid
};

struct Renderer2DData {
	const uint32_t MAX_QUADS = 10000;
	const uint32_t MAX_VERTICES = MAX_QUADS * 4;
	const uint32_t MAX_INDICES = MAX_QUADS * 6;

	Ref<VertexArray> quadVertexArray;
	Ref<VertexBuffer> quadVertexBuffer;
	Ref<Shader> textureShader;
	Ref<Texture2D> whiteTexture;

	uint32_t quadIndexCount = 0;
	QuadVertex* quadVertexBufferBase = nullptr;
	QuadVertex* quadVertexBufferPtr = nullptr;
};

static Renderer2DData sData;

void Renderer2D::Init() {
	PM_PROFILE_FUNCTION();

	//////////
	// Quad //
	//////////

	// Vertex Array
	sData.quadVertexArray = VertexArray::Create();

	// Vertex Buffer
	sData.quadVertexBuffer = VertexBuffer::Create(sData.MAX_VERTICES * sizeof(QuadVertex));
	sData.quadVertexBuffer->setLayout({
		{ ShaderDataType::FLOAT3, "iPosition" },
		{ ShaderDataType::FLOAT4, "iColor" },
		{ ShaderDataType::FLOAT2, "iTexCoord" },
	});
	sData.quadVertexArray->addVertexBuffer(sData.quadVertexBuffer);

	sData.quadVertexBufferBase = new QuadVertex[sData.MAX_VERTICES];

	// Index Buffer
	uint32_t offset = 0;
	uint32_t* quadIndices = new uint32_t[sData.MAX_INDICES];

	for (uint32_t i = 0; i < sData.MAX_INDICES; i += 6) {
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}

	Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, sData.MAX_INDICES);
	sData.quadVertexArray->setIndexBuffer(quadIndexBuffer);
	delete[] quadIndices;


	/////////////
	// Shaders //
	/////////////

	sData.textureShader = Shader::Create("assets/shaders/Texture.glsl");
	sData.textureShader->bind();
	sData.textureShader->setInt("uTexture", 0);


	//////////////
	// Textures //
	//////////////

	sData.whiteTexture = Texture2D::Create(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	sData.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));
}

void Renderer2D::Shutdown() {
	PM_PROFILE_FUNCTION();
}

void Renderer2D::BeginScene(const OrthographicCamera& iCamera) {
	PM_PROFILE_FUNCTION();

	sData.textureShader->bind();
	sData.textureShader->setMat4("uViewProjection", iCamera.viewProjectionMatrix());

	sData.quadIndexCount = 0;
	sData.quadVertexBufferPtr = sData.quadVertexBufferBase;
}

void Renderer2D::EndScene() {
	PM_PROFILE_FUNCTION();

	uint32_t dataSize = (uint8_t*)sData.quadVertexBufferPtr - (uint8_t*)sData.quadVertexBufferBase;
	sData.quadVertexBuffer->setData(sData.quadVertexBufferBase, dataSize);

	Flush();
}

void Renderer2D::Flush() {
	RenderCommand::DrawIndexed(sData.quadVertexArray, sData.quadIndexCount);
}

void Renderer2D::DrawQuad(const glm::vec2& iPosition,
						  const float iRotation,
						  const glm::vec2& iScale,
						  const float iTilingFactor,
						  Ref<Texture2D> iTexture,
						  const glm::vec4& iColor) {
	DrawQuad({ iPosition.x, iPosition.y, 0.0f }, iRotation, iScale, iTilingFactor, iTexture, iColor);
}

void Renderer2D::DrawQuad(const glm::vec3& iPosition,
						  const float iRotation,
						  const glm::vec2& iScale,
						  const float iTilingFactor,
						  Ref<Texture2D> iTexture,
						  const glm::vec4& iColor) {
	PM_PROFILE_FUNCTION();

	sData.quadVertexBufferPtr->position = iPosition;
	sData.quadVertexBufferPtr->color = iColor;
	sData.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
	sData.quadVertexBufferPtr++;

	sData.quadVertexBufferPtr->position = { iPosition.x + iScale.x, iPosition.y, 0.0f };
	sData.quadVertexBufferPtr->color = iColor;
	sData.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
	sData.quadVertexBufferPtr++;

	sData.quadVertexBufferPtr->position = { iPosition.x + iScale.x, iPosition.y + iScale.y, 0.0f };
	sData.quadVertexBufferPtr->color = iColor;
	sData.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
	sData.quadVertexBufferPtr++;

	sData.quadVertexBufferPtr->position = { iPosition.x, iPosition.y + iScale.y, 0.0f };
	sData.quadVertexBufferPtr->color = iColor;
	sData.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
	sData.quadVertexBufferPtr++;

	sData.quadIndexCount += 6;

	//sData.textureShader->setFloat4("uColor", iColor);
	//sData.textureShader->setFloat("uTilingFactor", iTilingFactor);

	//if (iTexture) {
	//	iTexture->bind();
	//} else {
	//	sData.whiteTexture->bind();
	//}

	//glm::mat4 transform = glm::translate(glm::mat4(1.0f), iPosition) * 
	//					  glm::rotate(glm::mat4(1.0f), iRotation, { 0.0f, 0.0f, 1.0f }) *
	//					  glm::scale(glm::mat4(1.0f), { iScale.x, iScale.y, 1.0f });
	//sData.textureShader->setMat4("uTransform", transform);

	//sData.quadVertexArray->bind();
	//RenderCommand::DrawIndexed(sData.quadVertexArray);
}
}