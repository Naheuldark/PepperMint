#pragma once

#include "PepperMint/Renderer/RendererAPI.h"

namespace PepperMint {

class OpenGLRendererAPI : public RendererAPI {
  public:
    OpenGLRendererAPI()           = default;
    ~OpenGLRendererAPI() override = default;

    virtual void init() override;

    void setViewport(uint32_t iX, uint32_t iY, uint32_t iWidth, uint32_t iHeight) override;
    void setClearColor(const glm::vec4& iColor) override;
    void clear() override;

    void drawIndexed(Ref<VertexArray> iVertexArray, uint32_t iIndexCount = 0) override;
    void drawLines(Ref<VertexArray> iVertexArray, uint32_t iVertexCount = 0) override;

    void setLineWidth(float iWidth) override;
};

}
