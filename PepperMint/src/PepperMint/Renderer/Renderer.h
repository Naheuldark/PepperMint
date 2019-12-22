#pragma once

#include "RenderCommand.h"

namespace PepperMint {

class Renderer {
public:
	Renderer() = default;
	~Renderer() = default;

	static void BeginScene();
	static void EndScene();

	static void Submit(const std::shared_ptr<VertexArray>& iVertexArray);

	inline static RendererAPI::API API() { return RendererAPI::GetAPI(); }
};
}