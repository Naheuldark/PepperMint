#pragma once

#include "PepperMint/Renderer/Shader.h"

#include <glm/glm.hpp>

// TODO: Remove!!
using GLenum = unsigned int;

namespace PepperMint {

class OpenGLShader : public Shader {
  public:
    OpenGLShader(const std::string& iShaderFile);
    ~OpenGLShader() override;

    void bind() const override;
    void unbind() const override;

    inline std::string name() const override { return _name; }

    void setInt(const std::string& iName, int iValue) override;
    void setIntArray(const std::string& iName, int* iValues, uint32_t iCount) override;
    void setFloat(const std::string& iName, float iValue) override;
    void setFloat2(const std::string& iName, const glm::vec2& iValue) override;
    void setFloat3(const std::string& iName, const glm::vec3& iValue) override;
    void setFloat4(const std::string& iName, const glm::vec4& iValue) override;
    void setMat3(const std::string& iName, const glm::mat3& iValue) override;
    void setMat4(const std::string& iName, const glm::mat4& iValue) override;

  private:
    void uploadUniformInt(const std::string& iName, int iValue);
    void uploadUniformIntArray(const std::string& iName, int* iValues, uint32_t iCount);
    void uploadUniformFloat(const std::string& iName, float iValue);
    void uploadUniformFloat2(const std::string& iName, const glm::vec2& iValue);
    void uploadUniformFloat3(const std::string& iName, const glm::vec3& iValue);
    void uploadUniformFloat4(const std::string& iName, const glm::vec4& iValue);
    void uploadUniformMat3(const std::string& iName, const glm::mat3& iValue);
    void uploadUniformMat4(const std::string& iName, const glm::mat4& iValue);

    std::string                             readFile(const std::string& iShaderFile);
    std::unordered_map<GLenum, std::string> preProcess(const std::string& iSource);
    void compile(const std::unordered_map<GLenum, std::string>& iShaderSources);

  private:
    uint32_t    _rendererId;
    std::string _name;
};

}
