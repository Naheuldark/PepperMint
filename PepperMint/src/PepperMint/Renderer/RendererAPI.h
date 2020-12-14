#pragma once

#include "PepperMint/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace PepperMint {

class RendererAPI {
  public:
    enum class API { NONE = 0, OPENGL };

  public:
    RendererAPI()          = default;
    virtual ~RendererAPI() = default;

    virtual void init() = 0;

    virtual void setViewport(uint32_t iX, uint32_t iY, uint32_t iWidth, uint32_t iHeight) = 0;
    virtual void setClearColor(const glm::vec4& iColor)                                   = 0;
    virtual void clear()                                                                  = 0;

    virtual void drawIndexed(Ref<VertexArray> iVertexArray, uint32_t iIndexCount = 0) = 0;

    static API GetAPI() { return sAPI; }

    static Scope<RendererAPI> Create();

  private:
    static API sAPI;
};

}
