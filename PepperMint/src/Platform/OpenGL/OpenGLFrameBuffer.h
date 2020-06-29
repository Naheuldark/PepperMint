#pragma once

#include "PepperMint/Renderer/FrameBuffer.h"

namespace PepperMint {

class OpenGLFrameBuffer : public FrameBuffer {
public:
	OpenGLFrameBuffer(const FrameBufferProperties& iProperties);
	~OpenGLFrameBuffer() override;

	void bind() override;
	void unbind() override;

	inline uint32_t colorAttachmentRendererId() const override { return _colorAttachment; }
	inline const FrameBufferProperties& properties() const override { return _properties; }

	void invalidate();

private:
	uint32_t _rendererId;
	uint32_t _colorAttachment, _depthAttachment;

	const FrameBufferProperties& _properties;
};

}