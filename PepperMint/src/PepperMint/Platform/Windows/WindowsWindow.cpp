#include "pmpch.h"

#include "WindowsWindow.h"

namespace PepperMint {

static bool kGLFWInitialized = false;

Window* Window::Create(const WindowProperties& iProperties) {
	return new WindowsWindow(iProperties);
}

WindowsWindow::WindowsWindow(const WindowProperties& iProperties) {
	init(iProperties);
}

WindowsWindow::~WindowsWindow() {
	shutdown();
}

void WindowsWindow::init(const WindowProperties& iProperties) {
	_data.title = iProperties.title;
	_data.width = iProperties.width;
	_data.height = iProperties.height;

	PM_CORE_INFO("Creating window {0} ({1} x {2})", _data.title, _data.width, _data.height);

	if (!kGLFWInitialized) {
		int success = glfwInit();
		PM_CORE_ASSERT(success, "GLFW could not be initialized!");

		kGLFWInitialized = true;
	}

	_window = glfwCreateWindow((int)_data.width, (int)_data.height, _data.title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, &_data);
	setVSync(true);
}

void WindowsWindow::shutdown() {
	glfwDestroyWindow(_window);
}

void WindowsWindow::onUpdate() {
	glfwPollEvents();
	glfwSwapBuffers(_window);
}

void WindowsWindow::setVSync(bool iEnabled) {
	if (iEnabled) {
		glfwSwapInterval(1);
	} else {
		glfwSwapInterval(0);
	}

	_data.vsync = iEnabled;
}
}