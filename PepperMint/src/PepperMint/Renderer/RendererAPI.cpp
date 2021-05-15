#include "pmpch.h"

#include "PepperMint/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace PepperMint {

RendererAPI::API RendererAPI::sAPI = RendererAPI::API::OPENGL;

Scope<RendererAPI> RendererAPI::Create() {
    switch (sAPI) {
        case RendererAPI::API::OPENGL:
            return CreateScope<OpenGLRendererAPI>();
        default:
            PM_CORE_ASSERT(false, "Specified RendererAPI is currently not supported!");
            return nullptr;
    }
}
}
