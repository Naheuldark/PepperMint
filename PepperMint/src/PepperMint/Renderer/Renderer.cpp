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

void Renderer::OnWindowResize(uint32_t iWidth, uint32_t iHeight) { RenderCommand::SetViewport(0, 0, iWidth, iHeight); }
}
