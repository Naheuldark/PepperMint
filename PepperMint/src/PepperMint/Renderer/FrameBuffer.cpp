#include "pmpch.h"
#include "PepperMint/Renderer/FrameBuffer.h"

#include "PepperMint/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace PepperMint {

Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferProperties& iProperties) {
	switch (Renderer::API()) {
		case RendererAPI::API::OPENGL:
			return CreateRef<OpenGLFrameBuffer>(iProperties);
		default:
			PM_CORE_ASSERT(false, "Specified RendererAPI is currently not supported!");
			return nullptr;
	}
}
}