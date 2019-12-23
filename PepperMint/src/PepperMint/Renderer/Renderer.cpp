#include "pmpch.h"

#include "Renderer.h"

namespace PepperMint {

Renderer::SceneData* Renderer::sSceneData = new Renderer::SceneData;

void Renderer::BeginScene(OrthographicCamera& ioCamera) {
	sSceneData->viewProjectionMatrix = ioCamera.viewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<Shader>& iShader, 
					  const std::shared_ptr<VertexArray>& iVertexArray,
					  const glm::mat4& iTransform) {
	iShader->bind();
	iShader->uploadUniformMat4("uViewProjection", sSceneData->viewProjectionMatrix);
	iShader->uploadUniformMat4("uTransform", iTransform);

	iVertexArray->bind();
	RenderCommand::DrawIndexed(iVertexArray);
}
}