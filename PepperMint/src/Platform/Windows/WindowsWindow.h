#pragma once

#include "PepperMint/Core/Window.h"
#include "PepperMint/Renderer/GraphicsContext.h"

// Forward declaration of GLFWwindow to avoit multi OpenGL declaration
struct GLFWwindow;

namespace PepperMint {

class WindowsWindow : public Window {
public:
	WindowsWindow(const WindowProperties& iProperties);
	~WindowsWindow() override;

	void onUpdate() override;

	inline unsigned int width() const override { return _data.width; }
	inline unsigned int height() const override { return _data.height; }

	// Window attributes
	inline void setEventCallback(const EventCallbackFn& iCallback) override { _data.eventCallback = iCallback; }
	void setVSync(bool iEnabled) override;
	bool isVSync() const override { return _data.vsync; }

	// Get the native Window pointer
	inline void* nativeWindow() const override { return _window; }

private:
	virtual void init(const WindowProperties& iProperties);
	virtual void shutdown();

private:
	GLFWwindow* _window;
	Scope<GraphicsContext> _context;

	struct WindowData {
		std::string title;
		unsigned int width = 0;
		unsigned int height = 0;
		bool vsync = false;

		EventCallbackFn eventCallback;
	};

	WindowData _data;
};

}