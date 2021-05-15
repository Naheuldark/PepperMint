#include "pmpch.h"

#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace PepperMint {

OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t iSize, uint32_t iBinding) {
    glCreateBuffers(1, &_rendererId);
    glNamedBufferData(_rendererId, iSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, iBinding, _rendererId);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer() { glDeleteBuffers(1, &_rendererId); }

void OpenGLUniformBuffer::setData(const void* const iData, uint32_t iSize, uint32_t iOffset) {
    glNamedBufferSubData(_rendererId, iOffset, iSize, iData);
}
}
