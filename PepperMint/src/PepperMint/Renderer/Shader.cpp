#include "pmpch.h"

#include "PepperMint/Renderer/Renderer.h"
#include "PepperMint/Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace PepperMint {

Ref<Shader> Shader::Create(const std::string& iShaderFile) {
    switch (Renderer::API()) {
    case RendererAPI::API::OPENGL:
        return CreateRef<OpenGLShader>(iShaderFile);
    default:
        PM_CORE_ASSERT(false, "Specified RendererAPI is currently not supported!");
        return nullptr;
    }
}

void ShaderLibrary::add(Ref<Shader> iShader) {
    const std::string& name = iShader->name();
    PM_CORE_ASSERT(!exists(name), "Shader already exists");
    _shaders[name] = iShader;
}

Ref<Shader> ShaderLibrary::load(const std::string& iShaderFile) {
    auto shader = Shader::Create(iShaderFile);
    add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::get(const std::string& iName) {
    PM_CORE_ASSERT(exists(iName), "Shader not found");
    return _shaders.at(iName);
}

bool ShaderLibrary::exists(const std::string& iName) const {
    return _shaders.find(iName) != _shaders.end();
}
}
