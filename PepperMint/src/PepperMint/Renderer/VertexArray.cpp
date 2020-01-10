#include "pmpch.h"

#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace PepperMint {

Ref<VertexArray> VertexArray::Create() {
	switch (Renderer::API()) {
		case RendererAPI::API::OPENGL:
			return CreateRef<OpenGLVertexArray>();
		default:
			PM_CORE_ASSERT(false, "Specified RendererAPI is currently not supported!");
			return nullptr;
	}
}
}