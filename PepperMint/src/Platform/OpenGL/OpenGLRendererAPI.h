#pragma once

#include "PepperMint/Renderer/RendererAPI.h"

namespace PepperMint {

class OpenGLRendererAPI : public RendererAPI {
public:
	OpenGLRendererAPI() = default;
	~OpenGLRendererAPI() = default;

	void setClearColor(const glm::vec4& iColor) override;
	void clear() override;

	void drawIndexed(const std::shared_ptr<VertexArray>& iVertexArray) override;
};

}