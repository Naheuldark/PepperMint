#pragma once

#include "PepperMint/Core/Base.h"

namespace PepperMint {

enum class FrameBufferTextureFormat {
    NONE = 0,

    // Color
    RGBA8,
    RED_INTEGER,

    // Depth / Stencil
    DEPTH24STENCIL8,

    // Defaults
    DEPTH = DEPTH24STENCIL8
};

struct FrameBufferTextureProperties {
    FrameBufferTextureFormat textureFormat = FrameBufferTextureFormat::NONE;
    // TODO: filtering / wrap

    FrameBufferTextureProperties() = default;
    FrameBufferTextureProperties(FrameBufferTextureFormat format) : textureFormat(format) {}
};

struct FrameBufferAttachmentProperties {
    std::vector<FrameBufferTextureProperties> attachments;

    FrameBufferAttachmentProperties() = default;
    FrameBufferAttachmentProperties(std::initializer_list<FrameBufferTextureProperties> attachments) : attachments(attachments) {}
};

struct FrameBufferProperties {
    uint32_t width   = 0;
    uint32_t height  = 0;
    uint32_t samples = 1;

    FrameBufferAttachmentProperties attachments;

    bool swapChainTarget = false;

    FrameBufferProperties(uint32_t width = 0, uint32_t height = 0, uint32_t samples = 1, bool swapChainTarget = false)
        : width(width), height(height), samples(samples), swapChainTarget(swapChainTarget) {}
};

class FrameBuffer {
  public:
    virtual ~FrameBuffer() = default;

    virtual void bind()   = 0;
    virtual void unbind() = 0;

    virtual void resize(uint32_t iWidth, uint32_t iHeight)                  = 0;
    virtual int  readPixel(uint32_t iAttachmentIndex, int iPosX, int iPosY) = 0;
    virtual void clearAttachment(uint32_t iAttachmentIndex, int iValue)     = 0;

    virtual uint32_t                     colorAttachmentRendererId(uint32_t iIndex = 0) const = 0;
    virtual const FrameBufferProperties& properties() const                                   = 0;

    static Ref<FrameBuffer> Create(const FrameBufferProperties& iProperties);
};

}
