#pragma once

#include "RendererAPI.h"

namespace PepperMint {

class RenderCommand {
public:
	RenderCommand() = default;
	~RenderCommand() = default;

	inline static void SetClearColor(const glm::vec4& iColor) {
		sRendererAPI->setClearColor(iColor);
	}

	inline static void Clear() {
		sRendererAPI->clear();
	}

	inline static void DrawIndexed(const Ref<VertexArray>& iVertexArray) {
		sRendererAPI->drawIndexed(iVertexArray);
	}

private:
	static RendererAPI* sRendererAPI;
};
}