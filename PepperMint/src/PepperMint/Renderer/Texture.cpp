#include "pmpch.h"

#include "PepperMint/Renderer/Renderer.h"
#include "PepperMint/Renderer/Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace PepperMint {

Ref<Texture2D> Texture2D::Create(uint32_t iWidth, uint32_t iHeight) {
    switch (Renderer::API()) {
        case RendererAPI::API::OPENGL:
            return CreateRef<OpenGLTexture2D>(iWidth, iHeight);
        default:
            PM_CORE_ASSERT(false, "Specified RendererAPI is currently not supported!");
            return nullptr;
    }
}

Ref<Texture2D> Texture2D::Create(const std::string& iPath) {
    switch (Renderer::API()) {
        case RendererAPI::API::OPENGL:
            return CreateRef<OpenGLTexture2D>(iPath);
        default:
            PM_CORE_ASSERT(false, "Specified RendererAPI is currently not supported!");
            return nullptr;
    }
}
}
