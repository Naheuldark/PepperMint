#pragma once

#include "PepperMint/Window.h"

#include"GLFW/glfw3.h"

namespace PepperMint {

class PM_API WindowsWindow : public Window {
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

private:
	virtual void init(const WindowProperties& iProperties);
	virtual void shutdown();

private:
	GLFWwindow* _window;

	struct WindowData {
		std::string title;
		unsigned int width, height;
		bool vsync;

		EventCallbackFn eventCallback;
	};

	WindowData _data;
};

}