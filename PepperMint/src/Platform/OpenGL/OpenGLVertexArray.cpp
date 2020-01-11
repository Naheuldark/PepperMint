#include "pmpch.h" 
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace PepperMint {

static GLenum ShaderDataType2OpenGLBaseType(ShaderDataType iType) {
	switch (iType) {
		case ShaderDataType::BOOL:		return GL_BOOL;
		case ShaderDataType::INT:		return GL_INT;
		case ShaderDataType::INT2:		return GL_INT;
		case ShaderDataType::INT3:		return GL_INT;
		case ShaderDataType::INT4:		return GL_INT;
		case ShaderDataType::FLOAT:		return GL_FLOAT;
		case ShaderDataType::FLOAT2:	return GL_FLOAT;
		case ShaderDataType::FLOAT3:	return GL_FLOAT;
		case ShaderDataType::FLOAT4:	return GL_FLOAT;
		case ShaderDataType::MAT3:		return GL_FLOAT;
		case ShaderDataType::MAT4:		return GL_FLOAT;
		default:
			PM_CORE_ASSERT(false, "Unknown ShaderDataType"); return 0;
	}
}

OpenGLVertexArray::OpenGLVertexArray() {
	PM_PROFILE_FUNCTION();

	glCreateVertexArrays(1, &_rendererId);
}

OpenGLVertexArray::~OpenGLVertexArray() {
	PM_PROFILE_FUNCTION();

	glDeleteVertexArrays(1, &_rendererId);
}

void OpenGLVertexArray::bind() const {
	PM_PROFILE_FUNCTION();

	glBindVertexArray(_rendererId);
}

void OpenGLVertexArray::unbind() const {
	PM_PROFILE_FUNCTION();

	glBindVertexArray(0);
}

void OpenGLVertexArray::addVertexBuffer(Ref<VertexBuffer> iVertexBuffer) {
	PM_PROFILE_FUNCTION();

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
	PM_PROFILE_FUNCTION();

	glBindVertexArray(_rendererId);
	iIndexBuffer->bind();

	_indexBuffer = iIndexBuffer;
}
}