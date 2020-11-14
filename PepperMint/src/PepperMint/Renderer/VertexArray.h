#pragma once

#include <memory>
#include <vector>

#include "PepperMint/Renderer/Buffer.h"

namespace PepperMint {

class VertexArray {
  public:
    VertexArray()          = default;
    virtual ~VertexArray() = default;

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual void addVertexBuffer(Ref<VertexBuffer> iVertexBuffer) = 0;
    virtual void setIndexBuffer(Ref<IndexBuffer> iIndexBuffer)    = 0;

    virtual const std::vector<Ref<VertexBuffer>>& vertexBuffers() const = 0;
    virtual Ref<IndexBuffer>                      indexBuffer() const   = 0;

    static Ref<VertexArray> Create();
};

}
