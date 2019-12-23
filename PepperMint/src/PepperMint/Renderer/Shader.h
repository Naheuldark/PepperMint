#pragma once

#include <string>
#include <glm/glm.hpp>

namespace PepperMint {

class Shader {
public:
	Shader(const std::string& iVertexShader, const std::string& iFragmentShader);
	~Shader();

	void bind();
	void unbind();

	void uploadUniformMat4(const std::string& iName, const glm::mat4& iMatrix);

private:
	uint32_t _rendererId;
};

}