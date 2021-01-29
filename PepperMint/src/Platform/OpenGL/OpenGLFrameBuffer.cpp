#include "pmpch.h"

#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace PepperMint {

namespace {

GLenum textureTarget(bool iMultiSampled) { return iMultiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D; }

void createTextures(bool iMultiSampled, uint32_t* oTexId, uint32_t iCount) { glCreateTextures(textureTarget(iMultiSampled), iCount, oTexId); }

void bindTexture(bool iMultiSampled, uint32_t iTexId) { glBindTexture(textureTarget(iMultiSampled), iTexId); }

void attachColorTexture(uint32_t iTexId, int iSamples, GLenum iFormat, uint32_t iWidth, uint32_t iHeight, int iIndex) {
    bool multiSampled = (iSamples > 1);
    if (multiSampled) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, iSamples, iFormat, iWidth, iHeight, GL_FALSE);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, iFormat, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + iIndex, textureTarget(multiSampled), iTexId, 0);
}

void attachDepthTexture(uint32_t iTexId, int iSamples, GLenum iFormat, GLenum iAttachmentType, uint32_t iWidth, uint32_t iHeight) {
    bool multiSampled = (iSamples > 1);
    if (multiSampled) {
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, iSamples, iFormat, iWidth, iHeight, GL_FALSE);
    } else {
        glTexStorage2D(GL_TEXTURE_2D, 1, iFormat, iWidth, iHeight);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, iAttachmentType, textureTarget(multiSampled), iTexId, 0);
}

bool isDepthFormat(FrameBufferTextureFormat iFormat) {
    switch (iFormat) {
        case FrameBufferTextureFormat::DEPTH24STENCIL8:
            return true;
    }

    return false;
}
}

static const uint32_t kMAX_FRAMEBUFFER_SIZE = 8192;

OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferProperties& iProperties) : _properties(iProperties) {
    for (auto&& property : _properties.attachments.attachments) {
        if (!isDepthFormat(property.textureFormat)) {
            _colorAttachmentProperties.emplace_back(property);
        } else {
            _depthAttachmentProperties = property;
        }
    }

    invalidate();
}

OpenGLFrameBuffer::~OpenGLFrameBuffer() {
    glDeleteFramebuffers(1, &_rendererId);
    glDeleteTextures(_colorAttachments.size(), _colorAttachments.data());
    glDeleteTextures(1, &_depthAttachment);
}

void OpenGLFrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, _rendererId);
    glViewport(0, 0, _properties.width, _properties.height);
}

void OpenGLFrameBuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenGLFrameBuffer::resize(uint32_t iWidth, uint32_t iHeight) {
    if (iWidth == 0 || iHeight == 0 || iWidth > kMAX_FRAMEBUFFER_SIZE || iHeight > kMAX_FRAMEBUFFER_SIZE) {
        PM_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", iWidth, iHeight);
        return;
    }
    _properties.width  = iWidth;
    _properties.height = iHeight;

    invalidate();
}

void OpenGLFrameBuffer::invalidate() {
    if (_rendererId) {
        glDeleteFramebuffers(1, &_rendererId);
        glDeleteTextures(_colorAttachments.size(), _colorAttachments.data());
        glDeleteTextures(1, &_depthAttachment);

        _colorAttachments.clear();
        _depthAttachment = 0;
    }

    glCreateFramebuffers(1, &_rendererId);
    glBindFramebuffer(GL_FRAMEBUFFER, _rendererId);

    bool multiSampled = (_properties.samples > 1);

    // Attachments
    if (!_colorAttachmentProperties.empty()) {
        _colorAttachments.resize(_colorAttachmentProperties.size());
        createTextures(multiSampled, _colorAttachments.data(), _colorAttachments.size());

        for (size_t i = 0; i < _colorAttachments.size(); ++i) {
            bindTexture(multiSampled, _colorAttachments[i]);
            switch (_colorAttachmentProperties[i].textureFormat) {
                case FrameBufferTextureFormat::RGBA8:
                    attachColorTexture(_colorAttachments[i], _properties.samples, GL_RGBA8, _properties.width, _properties.height, i);
                    break;
            }
        }
    }

    if (_depthAttachmentProperties.textureFormat != FrameBufferTextureFormat::NONE) {
        createTextures(multiSampled, &_depthAttachment, 1);
        bindTexture(multiSampled, _depthAttachment);

        switch (_depthAttachmentProperties.textureFormat) {
            case FrameBufferTextureFormat::DEPTH24STENCIL8:
                attachDepthTexture(
                    _depthAttachment, _properties.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, _properties.width, _properties.height);
                break;
        }
    }

    if (_colorAttachments.size() > 1) {
        PM_CORE_ASSERT(_colorAttachments.size() <= 4);
        GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        glDrawBuffers(_colorAttachments.size(), buffers);
    } else if (_colorAttachments.empty()) {
        // Only depth-pass
        glDrawBuffer(GL_NONE);
    }

    PM_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}
