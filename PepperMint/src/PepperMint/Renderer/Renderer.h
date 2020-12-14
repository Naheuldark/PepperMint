#pragma once

#include "PepperMint/Renderer/RenderCommand.h"
#include "PepperMint/Renderer/Shader.h"

namespace PepperMint {

class Renderer {
  public:
    Renderer()  = default;
    ~Renderer() = default;

    static void Init();
    static void Shutdown();

    static void OnWindowResize(uint32_t iWidth, uint32_t iHeight);

    static RendererAPI::API API() { return RendererAPI::GetAPI(); }

  private:
    struct SceneData {
        glm::mat4 viewProjectionMatrix;
    };
    static Scope<SceneData> sSceneData;
};
}
