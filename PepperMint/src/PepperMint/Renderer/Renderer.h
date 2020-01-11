#pragma once

#include "PepperMint/Renderer/RenderCommand.h"
#include "PepperMint/Renderer/OrthographicCamera.h"
#include "PepperMint/Renderer/Shader.h"

namespace PepperMint {

class Renderer {
public:
	Renderer() = default;
	~Renderer() = default;

	static void Init();
	static void Shutdown();

	static void OnWindowResize(uint32_t iWidth, uint32_t iHeight);

	static void BeginScene(OrthographicCamera& ioCamera);
	static void EndScene();

	static void Submit(Ref<Shader> iShader, 
					   Ref<VertexArray> iVertexArray,
					   const glm::mat4& iTransform = glm::mat4(1.0f));

	inline static RendererAPI::API API() { return RendererAPI::GetAPI(); }

private:
	struct SceneData {
		glm::mat4 viewProjectionMatrix;
	};
	static Scope<SceneData> sSceneData;
};
}