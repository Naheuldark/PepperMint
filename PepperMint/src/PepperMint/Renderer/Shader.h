#pragma once

#include <string>
#include <unordered_map>

namespace PepperMint {

class Shader {
public:
	virtual ~Shader() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual std::string name() const = 0;
	
	static Ref<Shader> Create(const std::string& iShaderFile);
};

class ShaderLibrary {
public:
	ShaderLibrary() = default;
	~ShaderLibrary() = default;

	void add(const Ref<Shader>& iShader);
	Ref<Shader> load(const std::string& iShaderFile);

	Ref<Shader> get(const std::string& iName);

	bool exists(const std::string& iName) const;

private:
	std::unordered_map<std::string, Ref<Shader>> _shaders;
};
}