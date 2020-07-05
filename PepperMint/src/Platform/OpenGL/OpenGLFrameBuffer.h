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

	inline uint32_t colorAttachmentRendererId() const override { return _colorAttachment; }
	inline const FrameBufferProperties& properties() const override { return _properties; }

	void invalidate();

private:
	uint32_t _rendererId = 0;
	uint32_t _colorAttachment = 0;
	uint32_t _depthAttachment = 0;

	FrameBufferProperties _properties;
};

}