#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace PepperMint {

class Shader {
public:
	virtual ~Shader() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual std::string name() const = 0;

	virtual void setInt(const std::string& iName, int iValue) = 0;
	virtual void setIntArray(const std::string& iName, int* iValues, uint32_t iCount) = 0;
	virtual void setFloat(const std::string& iName, float iValue) = 0;
	virtual void setFloat2(const std::string& iName, const glm::vec2& iValue) = 0;
	virtual void setFloat3(const std::string& iName, const glm::vec3& iValue) = 0;
	virtual void setFloat4(const std::string& iName, const glm::vec4& iValue) = 0;
	virtual void setMat3(const std::string& iName, const glm::mat3& iValue) = 0;
	virtual void setMat4(const std::string& iName, const glm::mat4& iValue) = 0;
	
	static Ref<Shader> Create(const std::string& iShaderFile);
};

class ShaderLibrary {
public:
	ShaderLibrary() = default;
	~ShaderLibrary() = default;

	void add(Ref<Shader> iShader);
	Ref<Shader> load(const std::string& iShaderFile);

	Ref<Shader> get(const std::string& iName);

	bool exists(const std::string& iName) const;

private:
	std::unordered_map<std::string, Ref<Shader>> _shaders;
};
}