#pragma once

#include "PepperMint/Renderer/UniformBuffer.h"

namespace PepperMint {

class OpenGLUniformBuffer : public UniformBuffer {
    OpenGLUniformBuffer(uint32_t iSize, uint32_t iBinding);
    ~OpenGLUniformBuffer() noexcept override;

    void setData(const void* const iData, uint32_t iSize, uint32_t iOffset = 0) override;

  private:
    uint32_t _rendererId = 0;
};
}
