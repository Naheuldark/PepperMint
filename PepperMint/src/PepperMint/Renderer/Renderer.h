#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace PepperMint {

class Renderer {
public:
	Renderer() = default;
	~Renderer() = default;

	static void BeginScene(OrthographicCamera& ioCamera);
	static void EndScene();

	static void Submit(const Ref<Shader>& iShader, 
					   const Ref<VertexArray>& iVertexArray,
					   const glm::mat4& iTransform = glm::mat4(1.0f));

	inline static RendererAPI::API API() { return RendererAPI::GetAPI(); }

private:
	struct SceneData {
		glm::mat4 viewProjectionMatrix;
	};
	static SceneData* sSceneData;
};
}