#pragma once

#include "PepperMint/Renderer/FrameBuffer.h"

namespace PepperMint {

class OpenGLFrameBuffer : public FrameBuffer {
  public:
    OpenGLFrameBuffer(const FrameBufferProperties& iProperties);
    ~OpenGLFrameBuffer() override;

    void bind() override;
    void unbind() override;

    void resize(uint32_t iWidth, uint32_t iHeight) override;
    int  readPixel(uint32_t iAttachmentIndex, int iPosX, int iPosY) override;
    void clearAttachment(uint32_t iAttachmentIndex, int iValue) override;

    uint32_t colorAttachmentRendererId(uint32_t iIndex = 0) const override {
        PM_CORE_ASSERT(iIndex < _colorAttachments.size());
        return _colorAttachments[iIndex];
    }
    const FrameBufferProperties& properties() const override { return _properties; }

    void invalidate();

  private:
    uint32_t _rendererId = 0;

    FrameBufferProperties _properties;

    std::vector<FrameBufferTextureProperties> _colorAttachmentProperties;
    FrameBufferTextureProperties              _depthAttachmentProperties = FrameBufferTextureFormat::NONE;

    std::vector<uint32_t> _colorAttachments;
    uint32_t              _depthAttachment = 0;
};

}
