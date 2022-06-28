#pragma once

#include "PepperMint/Renderer/RendererAPI.h"

namespace PepperMint {

class RenderCommand {
  public:
    RenderCommand()  = default;
    ~RenderCommand() = default;

    static void Init() { sRendererAPI->init(); }
    static void Clear() { sRendererAPI->clear(); }

    static void SetViewport(uint32_t iX, uint32_t iY, uint32_t iWidth, uint32_t iHeight) { sRendererAPI->setViewport(iX, iY, iWidth, iHeight); }
    static void SetClearColor(const glm::vec4& iColor) { sRendererAPI->setClearColor(iColor); }
    static void SetLineWidth(float iWidth) { sRendererAPI->setLineWidth(iWidth); }

    static void DrawIndexed(Ref<VertexArray> iVertexArray, uint32_t iIndexCount = 0) { sRendererAPI->drawIndexed(iVertexArray, iIndexCount); }
    static void DrawLines(Ref<VertexArray> iVertexArray, uint32_t iVertexCount = 0) { sRendererAPI->drawLines(iVertexArray, iVertexCount); }

  private:
    static Scope<RendererAPI> sRendererAPI;
};
}
