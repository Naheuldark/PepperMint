#pragma once

#include "RendererAPI.h"

namespace PepperMint {

class RenderCommand {
public:
	RenderCommand() = default;
	~RenderCommand() = default;

	inline static void Init() {
		sRendererAPI->init();
	}

	inline static void SetViewport(uint32_t iX, uint32_t iY, uint32_t iWidth, uint32_t iHeight) {
		sRendererAPI->setViewport(iX, iY, iWidth, iHeight);
	}

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
	static Scope<RendererAPI> sRendererAPI;
};
}