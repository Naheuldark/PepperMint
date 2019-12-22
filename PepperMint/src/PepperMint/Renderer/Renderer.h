#pragma once

namespace PepperMint {

enum class RendererAPI {
	NONE = 0,
	OPENGL
};

class Renderer {
public:
	Renderer() = default;
	~Renderer() = default;

	inline static RendererAPI API() { return sRendererAPI; }

private:
	static RendererAPI sRendererAPI;
};

}