#include "pmpch.h"

#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace PepperMint {

Ref<Texture2D> Texture2D::Create(const std::string& iPath) {
	switch (Renderer::API()) {
		case RendererAPI::API::OPENGL:
			return std::make_shared<OpenGLTexture2D>(iPath);
		default:
			PM_CORE_ASSERT(false, "Specified RendererAPI is currently not supported!");
			return nullptr;
	}
}
}