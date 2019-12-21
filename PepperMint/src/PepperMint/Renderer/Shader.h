#pragma once

#include <string>

namespace PepperMint {

class Shader {
public:
	Shader(const std::string& iVertexShader, const std::string& iFragmentShader);
	~Shader();

	void bind();
	void unbind();

private:
	uint32_t _rendererId;
};

}