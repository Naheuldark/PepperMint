#include "pmpch.h"

#include "PepperMint/Renderer/Renderer.h"
#include "PepperMint/Renderer/Renderer2D.h"

namespace PepperMint {

Scope<Renderer::SceneData> Renderer::sSceneData = CreateScope<Renderer::SceneData>();

void Renderer::Init() {
    PM_PROFILE_FUNCTION();

    RenderCommand::Init();
    Renderer2D::Init();
}

void Renderer::Shutdown() { Renderer2D::Shutdown(); }

void Renderer::OnWindowResize(uint32_t iWidth, uint32_t iHeight) {
    RenderCommand::SetViewport(0, 0, iWidth, iHeight);
}

void Renderer::BeginScene(OrthographicCamera& ioCamera) {
    sSceneData->viewProjectionMatrix = ioCamera.viewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(Ref<Shader>      iShader,
                      Ref<VertexArray> iVertexArray,
                      const glm::mat4& iTransform) {
    iShader->bind();
    iShader->setMat4("uViewProjection", sSceneData->viewProjectionMatrix);
    iShader->setMat4("uTransform", iTransform);

    iVertexArray->bind();
    RenderCommand::DrawIndexed(iVertexArray);
}
}
