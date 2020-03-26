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
	static void DrawQuad(const glm::vec2& iPosition,
						 const float iRotation = 0.0f,
						 const glm::vec2& iScale = glm::vec2(1.0f),
						 const float iTilingFactor = 1.0f,
						 Ref<Texture2D> iTexture = nullptr,
						 const glm::vec4& iColor = glm::vec4(1.0f));

	static void DrawQuad(const glm::vec3& iPosition, 
						 const float iRotation = 0.0f,
						 const glm::vec2& iScale = glm::vec2(1.0f), 
						 const float iTilingFactor = 1.0f,
						 Ref<Texture2D> iTexture = nullptr,
						 const glm::vec4& iColor = glm::vec4(1.0f));
};

}