#pragma once

#include "PepperMint/Renderer/Buffer.h"

namespace PepperMint {

class OpenGLVertexBuffer : public VertexBuffer {
  public:
    OpenGLVertexBuffer(uint32_t iSize);
    OpenGLVertexBuffer(float* iVertices, uint32_t iSize);
    ~OpenGLVertexBuffer() override;

    void bind() const override;
    void unbind() const override;

    void setData(void* iData, uint32_t iSize) override;

    inline const BufferLayout& layout() const override { return _layout; }
    inline void setLayout(const BufferLayout& iLayout) override { _layout = iLayout; }

  private:
    uint32_t     _rendererId;
    BufferLayout _layout;
};

class OpenGLIndexBuffer : public IndexBuffer {
  public:
    OpenGLIndexBuffer(uint32_t* iIndices, uint32_t iCount);
    ~OpenGLIndexBuffer() override;

    void bind() const override;
    void unbind() const override;

    inline uint32_t count() const override { return _count; }

  private:
    uint32_t _rendererId;
    uint32_t _count;
};
}
