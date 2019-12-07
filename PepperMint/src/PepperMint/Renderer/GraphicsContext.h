#pragma once

namespace PepperMint {

class GraphicsContext {
public:
	GraphicsContext() = default;
	~GraphicsContext() = default;

	virtual void init() = 0;
	virtual void swapBuffers() = 0;
};

}