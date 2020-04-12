#pragma once

#include "PepperMint/Renderer/RendererAPI.h"

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

	inline static void DrawIndexed(Ref<VertexArray> iVertexArray, uint32_t iCount = 0) {
		sRendererAPI->drawIndexed(iVertexArray, iCount);
	}

private:
	static Scope<RendererAPI> sRendererAPI;
};
}