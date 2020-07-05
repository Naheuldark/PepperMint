#include "pmpch.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace PepperMint {

OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferProperties& iProperties) 
	: _properties(iProperties) {
	invalidate();
}

OpenGLFrameBuffer::~OpenGLFrameBuffer() {
	glDeleteFramebuffers(1, &_rendererId);
	glDeleteTextures(1, &_colorAttachment);
	glDeleteTextures(1, &_depthAttachment);
}

void OpenGLFrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _rendererId);
	glViewport(0, 0, _properties.width, _properties.height);
}

void OpenGLFrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::resize(uint32_t iWidth, uint32_t iHeight) {
	_properties.width = iWidth;
	_properties.height = iHeight;

	invalidate();
}

void OpenGLFrameBuffer::invalidate() {
	if (_rendererId) {
		glDeleteFramebuffers(1, &_rendererId);
		glDeleteTextures(1, &_colorAttachment);
		glDeleteTextures(1, &_depthAttachment);
	}

	glCreateFramebuffers(1, &_rendererId);
	glBindFramebuffer(GL_FRAMEBUFFER, _rendererId);

	glCreateTextures(GL_TEXTURE_2D, 1, &_colorAttachment);
	glBindTexture(GL_TEXTURE_2D, _colorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _properties.width, _properties.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorAttachment, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &_depthAttachment);
	glBindTexture(GL_TEXTURE_2D, _depthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _properties.width, _properties.height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthAttachment, 0);

	PM_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}