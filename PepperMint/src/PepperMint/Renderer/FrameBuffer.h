#pragma once

#include "PepperMint/Core/Base.h"

namespace PepperMint {

struct FrameBufferProperties {
    uint32_t width   = 0;
    uint32_t height  = 0;
    uint32_t samples = 1;

    bool swapChainTarget = false;
};

class FrameBuffer {
  public:
    virtual ~FrameBuffer() = default;

    virtual void bind()   = 0;
    virtual void unbind() = 0;

    virtual void resize(uint32_t iWidth, uint32_t iHeight) = 0;

    virtual uint32_t                     colorAttachmentRendererId() const = 0;
    virtual const FrameBufferProperties& properties() const                = 0;

    static Ref<FrameBuffer> Create(const FrameBufferProperties& iProperties);
};

}
