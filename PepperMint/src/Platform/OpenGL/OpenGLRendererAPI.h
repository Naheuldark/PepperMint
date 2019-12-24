#pragma once

#include "PepperMint/Renderer/RendererAPI.h"

namespace PepperMint {

class OpenGLRendererAPI : public RendererAPI {
public:
	OpenGLRendererAPI() = default;
	~OpenGLRendererAPI() = default;

	void setClearColor(const glm::vec4& iColor) override;
	void clear() override;

	void drawIndexed(const Ref<VertexArray>& iVertexArray) override;
};

}