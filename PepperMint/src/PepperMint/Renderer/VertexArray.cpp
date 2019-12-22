#include "pmpch.h"

#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace PepperMint {

VertexArray* VertexArray::Create() {
	switch (Renderer::API()) {
		case RendererAPI::OPENGL:
			return new OpenGLVertexArray();
		default:
			PM_CORE_ASSERT(false, "Specified RendererAPI is currently not supported!");
			return nullptr;
	}
}
}