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

	static void Submit(const std::shared_ptr<Shader>& iShader, const std::shared_ptr<VertexArray>& iVertexArray);

	inline static RendererAPI::API API() { return RendererAPI::GetAPI(); }

private:
	struct SceneData {
		glm::mat4 viewProjectionMatrix;
	};
	static SceneData* sSceneData;
};
}