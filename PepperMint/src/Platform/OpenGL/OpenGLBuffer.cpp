#include "Platform/OpenGL/OpenGLBuffer.h"
#include "pmpch.h"

#include <glad/glad.h>

namespace PepperMint {

///////////////////
// Vertex Buffer //
///////////////////

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t iSize) {
    PM_PROFILE_FUNCTION();

    glCreateBuffers(1, &_rendererId);
    glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
    glBufferData(GL_ARRAY_BUFFER, iSize, nullptr, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* iVertices, uint32_t iSize) {
    PM_PROFILE_FUNCTION();

    glCreateBuffers(1, &_rendererId);
    glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
    glBufferData(GL_ARRAY_BUFFER, iSize, iVertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    PM_PROFILE_FUNCTION();

    glDeleteBuffers(1, &_rendererId);
}

void OpenGLVertexBuffer::bind() const {
    PM_PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
}

void OpenGLVertexBuffer::unbind() const {
    PM_PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::setData(void* iData, uint32_t iSize) {
    glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
    glBufferSubData(GL_ARRAY_BUFFER, 0, iSize, iData);
}

//////////////////
// Index Buffer //
//////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* iIndices, uint32_t iCount) : _count(iCount) {
    PM_PROFILE_FUNCTION();

    glCreateBuffers(1, &_rendererId);
    glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
    glBufferData(GL_ARRAY_BUFFER, iCount * sizeof(uint32_t), iIndices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    PM_PROFILE_FUNCTION();

    glDeleteBuffers(1, &_rendererId);
}

void OpenGLIndexBuffer::bind() const {
    PM_PROFILE_FUNCTION();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
}

void OpenGLIndexBuffer::unbind() const {
    PM_PROFILE_FUNCTION();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}
