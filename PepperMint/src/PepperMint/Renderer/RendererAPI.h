#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace PepperMint {

class RendererAPI {
public:
	enum class API {
		NONE = 0,
		OPENGL
	};

public:
	RendererAPI() = default;
	~RendererAPI() = default;

	virtual void setClearColor(const glm::vec4& iColor) = 0;
	virtual void clear() = 0;

	virtual void drawIndexed(const Ref<VertexArray>& iVertexArray) = 0;

	inline static API GetAPI() { return sAPI; }

private:
	static API sAPI;
};

}