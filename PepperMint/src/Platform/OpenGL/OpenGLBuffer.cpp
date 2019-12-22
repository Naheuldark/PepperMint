#include "pmpch.h"

#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace PepperMint {

///////////////////
// Vertex Buffer //
///////////////////

OpenGLVertexBuffer::OpenGLVertexBuffer(float* iVertices, uint32_t iSize) {
	glCreateBuffers(1, &_rendererId);
	glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
	glBufferData(GL_ARRAY_BUFFER, iSize, iVertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
	glDeleteBuffers(1, &_rendererId);
}

void OpenGLVertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
}

void OpenGLVertexBuffer::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


//////////////////
// Index Buffer //
//////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* iIndices, uint32_t iCount)
	: _count(iCount) {
	glCreateBuffers(1, &_rendererId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iCount * sizeof(uint32_t), iIndices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
	glDeleteBuffers(1, &_rendererId);
}

void OpenGLIndexBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
}

void OpenGLIndexBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}