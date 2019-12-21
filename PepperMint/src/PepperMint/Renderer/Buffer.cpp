#include "pmpch.h"

#include "Buffer.h"

#include "PepperMint/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace PepperMint {

VertexBuffer* VertexBuffer::Create(float* iVertices, uint32_t iSize) {
	switch (Renderer::API()) {
		case RendererAPI::OPENGL:
			return new OpenGLVertexBuffer(iVertices, iSize);
		default:
			PM_CORE_ASSERT(false, "Specified RendererAPI is currently not supported!"); 
			return nullptr;
	}
}

IndexBuffer* IndexBuffer::Create(uint32_t* iIndices, uint32_t iSize) {
	switch (Renderer::API()) {
		case RendererAPI::OPENGL:
			return new OpenGLIndexBuffer(iIndices, iSize);
		default:
			PM_CORE_ASSERT(false, "Specified RendererAPI is currently not supported!");
			return nullptr;
	}
}

}