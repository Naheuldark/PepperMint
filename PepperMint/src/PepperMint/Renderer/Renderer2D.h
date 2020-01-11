#pragma once

#include "PepperMint/Renderer/OrthographicCamera.h"
#include "PepperMint/Renderer/Texture.h"

namespace PepperMint {

class Renderer2D {
public:
	static void Init();
	static void Shutdown();

	static void BeginScene(const OrthographicCamera& iCamera);
	static void EndScene();

	// Primitives
	static void DrawQuad(const glm::vec2& iPosition, const glm::vec2& iSize, const glm::vec4& iColor);
	static void DrawQuad(const glm::vec3& iPosition, const glm::vec2& iSize, const glm::vec4& iColor);
	static void DrawQuad(const glm::vec2& iPosition, const glm::vec2& iSize, float iRotation, const glm::vec4& iColor);
	static void DrawQuad(const glm::vec3& iPosition, const glm::vec2& iSize, float iRotation, const glm::vec4& iColor);

	static void DrawQuad(const glm::vec2& iPosition, const glm::vec2& iSize, Ref<Texture2D> iTexture);
	static void DrawQuad(const glm::vec3& iPosition, const glm::vec2& iSize, Ref<Texture2D> iTexture);
	static void DrawQuad(const glm::vec2& iPosition, const glm::vec2& iSize, float iRotation, Ref<Texture2D> iTexture);
	static void DrawQuad(const glm::vec3& iPosition, const glm::vec2& iSize, float iRotation, Ref<Texture2D> iTexture);
};

}