#include "pmpch.h"

#include "PepperMint/Renderer/GraphicsContext.h"
#include "PepperMint/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace PepperMint {

Scope<GraphicsContext> GraphicsContext::Create(void* iWindow) {
    switch (Renderer::API()) {
        case RendererAPI::API::OPENGL:
            return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(iWindow));
        default:
            PM_CORE_ASSERT(false, "Specified RendererAPI is currently not supported!");
            return nullptr;
    }
}
}
