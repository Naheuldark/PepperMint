#include "pmpch.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace PepperMint {

Renderer::SceneData* Renderer::sSceneData = new Renderer::SceneData;

void Renderer::BeginScene(OrthographicCamera& ioCamera) {
	sSceneData->viewProjectionMatrix = ioCamera.viewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<Shader>& iShader, 
					  const Ref<VertexArray>& iVertexArray,
					  const glm::mat4& iTransform) {
	iShader->bind();
	std::dynamic_pointer_cast<OpenGLShader>(iShader)->uploadUniformMat4("uViewProjection", sSceneData->viewProjectionMatrix);
	std::dynamic_pointer_cast<OpenGLShader>(iShader)->uploadUniformMat4("uTransform", iTransform);

	iVertexArray->bind();
	RenderCommand::DrawIndexed(iVertexArray);
}
}