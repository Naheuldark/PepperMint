#include "pmpch.h"

#include "PepperMint/Renderer/Renderer.h"
#include "PepperMint/Renderer/UniformBuffer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace PepperMint {

Ref<UniformBuffer> UniformBuffer::Create(uint32_t iSize, uint32_t iBinding) {
    switch (Renderer::API()) {
        case RendererAPI::API::OPENGL:
            return CreateRef<OpenGLUniformBuffer>(iSize, iBinding);
        default:
            PM_CORE_ASSERT(false, "Specified RendererAPI is currently not supported!");
            return nullptr;
    }
}
}
