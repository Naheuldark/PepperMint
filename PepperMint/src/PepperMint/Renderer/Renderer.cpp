#include "pmpch.h"

#include "Renderer.h"

#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace PepperMint {

Scope<Renderer::SceneData> Renderer::sSceneData = CreateScope<Renderer::SceneData>();

void Renderer::Init() {
	RenderCommand::Init();
	Renderer2D::Init();
}

void Renderer::OnWindowResize(uint32_t iWidth, uint32_t iHeight) {
	RenderCommand::SetViewport(0, 0, iWidth, iHeight);
}

void Renderer::BeginScene(OrthographicCamera& ioCamera) {
	sSceneData->viewProjectionMatrix = ioCamera.viewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(Ref<Shader> iShader, 
					  Ref<VertexArray> iVertexArray,
					  const glm::mat4& iTransform) {
	iShader->bind();
	std::dynamic_pointer_cast<OpenGLShader>(iShader)->setMat4("uViewProjection", sSceneData->viewProjectionMatrix);
	std::dynamic_pointer_cast<OpenGLShader>(iShader)->setMat4("uTransform", iTransform);

	iVertexArray->bind();
	RenderCommand::DrawIndexed(iVertexArray);
}
}