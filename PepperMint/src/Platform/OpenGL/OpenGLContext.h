#pragma once

#include "PepperMint/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace PepperMint {

class OpenGLContext : public GraphicsContext {
public:
	OpenGLContext(GLFWwindow* iWindowHandle);
	~OpenGLContext() = default;

	void init() override;
	void swapBuffers() override;

private:
	GLFWwindow* _windowHandle;
};
}