#include "pmpch.h"

#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace PepperMint {

static GLenum ShaderDataType2OpenGLBaseType(ShaderDataType iType) {
	switch (iType) {
		case PepperMint::ShaderDataType::BOOL:		return GL_BOOL;
		case PepperMint::ShaderDataType::INT:		return GL_INT;
		case PepperMint::ShaderDataType::INT2:		return GL_INT;
		case PepperMint::ShaderDataType::INT3:		return GL_INT;
		case PepperMint::ShaderDataType::INT4:		return GL_INT;
		case PepperMint::ShaderDataType::FLOAT:		return GL_FLOAT;
		case PepperMint::ShaderDataType::FLOAT2:	return GL_FLOAT;
		case PepperMint::ShaderDataType::FLOAT3:	return GL_FLOAT;
		case PepperMint::ShaderDataType::FLOAT4:	return GL_FLOAT;
		case PepperMint::ShaderDataType::MAT3:		return GL_FLOAT;
		case PepperMint::ShaderDataType::MAT4:		return GL_FLOAT;
		default:
			PM_CORE_ASSERT(false, "Unknown ShaderDataType"); return 0;
	}
}

OpenGLVertexArray::OpenGLVertexArray() {
	glCreateVertexArrays(1, &_rendererId);
}

OpenGLVertexArray::~OpenGLVertexArray() {
	glDeleteVertexArrays(1, &_rendererId);
}

void OpenGLVertexArray::bind() const {
	glBindVertexArray(_rendererId);
}

void OpenGLVertexArray::unbind() const {
	glBindVertexArray(0);
}

void OpenGLVertexArray::addVertexBuffer(Ref<VertexBuffer> iVertexBuffer) {
	PM_CORE_ASSERT(iVertexBuffer->layout().elements().size(), "VertexBuffer has no layout!");

	glBindVertexArray(_rendererId);
	iVertexBuffer->bind();

	const auto& layout = iVertexBuffer->layout();
	for (auto&& element : layout) {
		glEnableVertexAttribArray(_vertexBufferIndex);
		glVertexAttribPointer(_vertexBufferIndex,
							  element.componentCount(),
							  ShaderDataType2OpenGLBaseType(element.type),
							  element.normalized ? GL_TRUE : GL_FALSE,
							  layout.stride(),
							  (const void*)element.offset);
		_vertexBufferIndex++;
	}

	_vertexBuffers.push_back(iVertexBuffer);
}

void OpenGLVertexArray::setIndexBuffer(Ref<IndexBuffer> iIndexBuffer) {
	glBindVertexArray(_rendererId);
	iIndexBuffer->bind();

	_indexBuffer = iIndexBuffer;
}
}