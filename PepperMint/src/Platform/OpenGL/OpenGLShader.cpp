#include "pmpch.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "PepperMint/Core/Timer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace PepperMint {

namespace {

GLenum shaderTypeFromString(const std::string& iType) {
    if (iType == "vertex") {
        return GL_VERTEX_SHADER;
    } else if (iType == "fragment" || iType == "pixel") {
        return GL_FRAGMENT_SHADER;
    }

    PM_CORE_ASSERT(false, "Unknown shader type");
    return 0;
}

shaderc_shader_kind glShaderStageToShaderC(GLenum iStage) {
    switch (iStage) {
        case GL_VERTEX_SHADER:
            return shaderc_glsl_vertex_shader;
        case GL_FRAGMENT_SHADER:
            return shaderc_glsl_fragment_shader;
        default:
            PM_CORE_ASSERT(false, "Unknown shader type");
            return (shaderc_shader_kind)0;
    }
}

const char* glShaderStageToString(GLenum iStage) {
    switch (iStage) {
        case GL_VERTEX_SHADER:
            return "GL_VERTEC_SHADER";
        case GL_FRAGMENT_SHADER:
            return "GL_FRAGMENT_SHADER";
        default:
            PM_CORE_ASSERT(false, "Unknown shader type");
            return nullptr;
    }
}

const char* glShaderStageCachedOpenGLFileExtension(GLenum iStage) {
    switch (iStage) {
        case GL_VERTEX_SHADER:
            return ".cached_opengl.vert";
        case GL_FRAGMENT_SHADER:
            return ".cached_opengl.frag";
        default:
            PM_CORE_ASSERT(false, "Unknown shader type");
            return nullptr;
    }
}

const char* glShaderStageCachedVulkanFileExtension(GLenum iStage) {
    switch (iStage) {
        case GL_VERTEX_SHADER:
            return ".cached_vulkan.vert";
        case GL_FRAGMENT_SHADER:
            return ".cached_vulkan.frag";
        default:
            PM_CORE_ASSERT(false, "Unknown shader type");
            return nullptr;
    }
}

std::filesystem::path getCacheDirectory() {
    // TODO make sure assets directory is valid
    return "assets/cache/shader";
}

void createCacheDirectoryIfNeeded() {
    auto&& cacheDirectory = getCacheDirectory();
    if (!std::filesystem::exists(cacheDirectory)) {
        std::filesystem::create_directories(cacheDirectory);
    }
}
}

OpenGLShader::OpenGLShader(const std::string& iShaderFile) : _shaderFile(iShaderFile) {
    PM_PROFILE_FUNCTION();

    createCacheDirectoryIfNeeded();

    auto&& source        = readFile(iShaderFile);
    auto&& shaderSources = preProcess(source);

    {
        Timer timer;
        compileOrGetVulkanBinaries(shaderSources);
        compileOrGetOpenGLBinaries();
        createProgram();
        PM_CORE_WARN("Shader creation took {0}ms", timer.elapsedMillis());
    }

    auto&& lastSlash = iShaderFile.find_last_of("/\\");
    lastSlash        = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto&& lastDot   = iShaderFile.rfind('.');
    auto&& count     = lastDot == std::string::npos ? iShaderFile.size() - lastSlash : lastDot - lastSlash;
    _name            = iShaderFile.substr(lastSlash, count);
}

OpenGLShader::~OpenGLShader() {
    PM_PROFILE_FUNCTION();

    glDeleteProgram(_rendererId);
}

void OpenGLShader::bind() const {
    PM_PROFILE_FUNCTION();

    glUseProgram(_rendererId);
}

void OpenGLShader::unbind() const {
    PM_PROFILE_FUNCTION();

    glUseProgram(0);
}

void OpenGLShader::setInt(const std::string& iName, int iValue) {
    PM_PROFILE_FUNCTION();

    uploadUniformInt(iName, iValue);
}

void OpenGLShader::setIntArray(const std::string& iName, int* iValues, uint32_t iCount) {
    PM_PROFILE_FUNCTION();

    uploadUniformIntArray(iName, iValues, iCount);
}

void OpenGLShader::setFloat(const std::string& iName, float iValue) {
    PM_PROFILE_FUNCTION();

    uploadUniformFloat(iName, iValue);
}

void OpenGLShader::setFloat2(const std::string& iName, const glm::vec2& iValue) {
    PM_PROFILE_FUNCTION();

    uploadUniformFloat2(iName, iValue);
}

void OpenGLShader::setFloat3(const std::string& iName, const glm::vec3& iValue) {
    PM_PROFILE_FUNCTION();

    uploadUniformFloat3(iName, iValue);
}

void OpenGLShader::setFloat4(const std::string& iName, const glm::vec4& iValue) {
    PM_PROFILE_FUNCTION();

    uploadUniformFloat4(iName, iValue);
}

void OpenGLShader::setMat3(const std::string& iName, const glm::mat3& iValue) {
    PM_PROFILE_FUNCTION();

    uploadUniformMat3(iName, iValue);
}

void OpenGLShader::setMat4(const std::string& iName, const glm::mat4& iValue) {
    PM_PROFILE_FUNCTION();

    uploadUniformMat4(iName, iValue);
}

void OpenGLShader::uploadUniformInt(const std::string& iName, int iValue) {
    GLint location = glGetUniformLocation(_rendererId, iName.c_str());
    glUniform1i(location, iValue);
}

void OpenGLShader::uploadUniformIntArray(const std::string& iName, int* iValues, uint32_t iCount) {
    GLint location = glGetUniformLocation(_rendererId, iName.c_str());
    glUniform1iv(location, iCount, iValues);
}

void OpenGLShader::uploadUniformFloat(const std::string& iName, float iValue) {
    GLint location = glGetUniformLocation(_rendererId, iName.c_str());
    glUniform1f(location, iValue);
}

void OpenGLShader::uploadUniformFloat2(const std::string& iName, const glm::vec2& iValue) {
    GLint location = glGetUniformLocation(_rendererId, iName.c_str());
    glUniform2f(location, iValue.x, iValue.y);
}

void OpenGLShader::uploadUniformFloat3(const std::string& iName, const glm::vec3& iValue) {
    GLint location = glGetUniformLocation(_rendererId, iName.c_str());
    glUniform3f(location, iValue.x, iValue.y, iValue.z);
}

void OpenGLShader::uploadUniformFloat4(const std::string& iName, const glm::vec4& iValue) {
    GLint location = glGetUniformLocation(_rendererId, iName.c_str());
    glUniform4f(location, iValue.x, iValue.y, iValue.z, iValue.w);
}

void OpenGLShader::uploadUniformMat3(const std::string& iName, const glm::mat3& iValue) {
    GLint location = glGetUniformLocation(_rendererId, iName.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(iValue));
}

void OpenGLShader::uploadUniformMat4(const std::string& iName, const glm::mat4& iValue) {
    GLint location = glGetUniformLocation(_rendererId, iName.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(iValue));
}

std::string OpenGLShader::readFile(const std::string& iShaderFile) {
    PM_PROFILE_FUNCTION();

    std::string result;

    std::ifstream in(iShaderFile, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
    if (in) {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        if (size != -1) {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
        } else {
            PM_CORE_ERROR("Could not read from file '{0}'", iShaderFile);
        }
    } else {
        PM_CORE_ERROR("Could not open file '{0}'", iShaderFile);
    }

    return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& iSource) {
    PM_PROFILE_FUNCTION();

    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken       = "#type";
    size_t      typeTokenLength = strlen(typeToken);
    size_t      pos             = iSource.find(typeToken, 0); // Start of shader type declaration line

    while (pos != std::string::npos) {
        size_t eol = iSource.find_first_of("\r\n", pos); // End of shader type declaration line
        PM_CORE_ASSERT(eol != std::string::npos, "Syntax error");

        size_t             begin = pos + typeTokenLength + 1; // Start of shader type name (after "#type " keyword)
        const std::string& type  = iSource.substr(begin, eol - begin);
        PM_CORE_ASSERT(shaderTypeFromString(type), "Invalid shader type specified");

        size_t nextLinePos = iSource.find_first_not_of("\r\n", eol); // Start of shader code after shader type declaration line
        PM_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
        pos = iSource.find(typeToken, nextLinePos); // Start of next shader type declaration line

        shaderSources[shaderTypeFromString(type)] =
            (pos == std::string::npos) ? iSource.substr(nextLinePos) : iSource.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderSources;
}

void OpenGLShader::compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& iShaderSources) {
    PM_PROFILE_FUNCTION();

    shaderc::Compiler       compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

    const bool optimize = true;
    if (optimize) {
        options.SetOptimizationLevel(shaderc_optimization_level_performance);
    }

    auto&& cacheDirectory = getCacheDirectory();
    auto&& shaderFile     = _shaderFile;

    auto&& shaderData = _vulkanSPIRV;
    shaderData.clear();
    for (auto&& [stage, source] : iShaderSources) {
        auto&& cacheFile = cacheDirectory / (shaderFile.filename().string() + glShaderStageCachedVulkanFileExtension(stage));

        std::ifstream in(cacheFile, std::ios::in | std::ios::binary);
        if (in.is_open()) {
            in.seekg(0, std::ios::end);
            auto&& size = in.tellg();
            in.seekg(0, std::ios::beg);

            auto&& data = shaderData[stage];
            data.resize(size / sizeof(uint32_t));
            in.read((char*)data.data(), size);
        } else {
            auto&& module = compiler.CompileGlslToSpv(source, glShaderStageToShaderC(stage), shaderFile.filename().string().c_str(), options);
            if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
                PM_CORE_ERROR(module.GetErrorMessage());
                PM_CORE_ASSERT(false, "An error occured during the compilation from GLSL to SPV (Vulkan)");
            }

            shaderData[stage] = std::vector<uint32_t>(module.begin(), module.end());

            std::ofstream out(cacheFile, std::ios::out | std::ios::binary);
            if (out.is_open()) {
                auto&& data = shaderData[stage];
                out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                out.flush();
                out.close();
            }
        }
    }

    for (auto&& [stage, data] : shaderData) {
        reflect(stage, data);
    }
}

void OpenGLShader::compileOrGetOpenGLBinaries() {
    PM_PROFILE_FUNCTION();

    _openglSourceCode.clear();

    shaderc::Compiler       compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

    const bool optimize = true;
    if (optimize) {
        options.SetOptimizationLevel(shaderc_optimization_level_performance);
    }

    auto&& cacheDirectory = getCacheDirectory();
    auto&& shaderFile     = _shaderFile;

    auto&& shaderData = _openglSPIRV;
    shaderData.clear();
    for (auto&& [stage, spirv] : _vulkanSPIRV) {
        auto&& cacheFile = cacheDirectory / (shaderFile.filename().string() + glShaderStageCachedOpenGLFileExtension(stage));

        std::ifstream in(cacheFile, std::ios::in | std::ios::binary);
        if (in.is_open()) {
            in.seekg(0, std::ios::end);
            auto&& size = in.tellg();
            in.seekg(0, std::ios::beg);

            auto&& data = shaderData[stage];
            data.resize(size / sizeof(uint32_t));
            in.read((char*)data.data(), size);
        } else {
            spirv_cross::CompilerGLSL glslCompiler(spirv);
            _openglSourceCode[stage] = glslCompiler.compile();
            auto&& source            = _openglSourceCode[stage];

            auto&& module = compiler.CompileGlslToSpv(source, glShaderStageToShaderC(stage), shaderFile.filename().string().c_str(), options);
            if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
                PM_CORE_ERROR(module.GetErrorMessage());
                PM_CORE_ASSERT(false, "An error occured during the compilation from GLSL to SPV (OpenGL)");
            }

            shaderData[stage] = std::vector<uint32_t>(module.begin(), module.end());

            std::ofstream out(cacheFile, std::ios::out | std::ios::binary);
            if (out.is_open()) {
                auto&& data = shaderData[stage];
                out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                out.flush();
                out.close();
            }
        }
    }
}

void OpenGLShader::createProgram() {
    PM_PROFILE_FUNCTION();

    GLuint program = glCreateProgram();

    std::vector<GLuint> shaderIds;
    for (auto&& [stage, spirv] : _openglSPIRV) {
        GLuint shaderId = shaderIds.emplace_back(glCreateShader(stage));
        glShaderBinary(1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
        glSpecializeShader(shaderId, "main", 0, nullptr, nullptr);
        glAttachShader(program, shaderId);
    }

    glLinkProgram(program);

    GLint isLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
        PM_CORE_ERROR("Shader linking failed ({0}):\n{1}", _shaderFile, infoLog.data());

        glDeleteProgram(program);
        for (auto&& shaderId : shaderIds) {
            glDeleteShader(shaderId);
        }
        return;
    }

    for (auto&& shaderId : shaderIds) {
        glDetachShader(program, shaderId);
        glDeleteShader(shaderId);
    }

    _rendererId = program;
}

void OpenGLShader::reflect(GLenum iStage, const std::vector<uint32_t>& iShaderData) {
    spirv_cross::Compiler compiler(iShaderData);
    auto&&                resources = compiler.get_shader_resources();

    PM_CORE_TRACE("OpenGLShader::reflect - {0} {1}", glShaderStageToString(iStage), _shaderFile);
    PM_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
    PM_CORE_TRACE("    {0} resources", resources.sampled_images.size());

    PM_CORE_TRACE("Uniform buffers:");
    for (auto&& resource : resources.uniform_buffers) {
        auto&& bufferType  = compiler.get_type(resource.base_type_id);
        auto&& bufferSize  = compiler.get_declared_struct_size(bufferType);
        auto&& binding     = compiler.get_decoration(resource.id, spv::DecorationBinding);
        auto&& memberCount = bufferType.member_types.size();

        PM_CORE_TRACE("  {0}", resource.name);
        PM_CORE_TRACE("    Size = {0}", bufferSize);
        PM_CORE_TRACE("    Binding = {0}", binding);
        PM_CORE_TRACE("    Members = {0}", memberCount);
    }
}
}
