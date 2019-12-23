#include "pmpch.h"

#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace PepperMint {

Shader* Shader::Create(const std::string& iVertexSrc, const std::string& iFragmentSrc) {
	switch (Renderer::API()) {
		case RendererAPI::API::OPENGL:
			return new OpenGLShader(iVertexSrc, iFragmentSrc);
		default:
			PM_CORE_ASSERT(false, "Specified RendererAPI is currently not supported!");
			return nullptr;
	}
}
}