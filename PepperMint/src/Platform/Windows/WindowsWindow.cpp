#include "pmpch.h"

#include "WindowsWindow.h"

#include "PepperMint/Events/ApplicationEvent.h"
#include "PepperMint/Events/KeyEvent.h"
#include "PepperMint/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>

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
		PM_CORE_ASSERT(success, "Failed to initialize GLFW!");

		glfwSetErrorCallback([](int iError, const char* iDescription) {
								PM_CORE_ERROR("GLFW Error ({0}): {1}", iError, iDescription)
							 });

		kGLFWInitialized = true;
	}

	_window = glfwCreateWindow((int)_data.width, (int)_data.height, _data.title.c_str(), nullptr, nullptr);

	_context = new OpenGLContext(_window);
	_context->init();

	glfwSetWindowUserPointer(_window, &_data);
	setVSync(true);

	// Set GLFW callbacks
	glfwSetWindowSizeCallback(_window,
							  [](GLFWwindow* window, int width, int height) {
								  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
								  data.width = width;
								  data.height = height;

								  WindowResizeEvent event(width, height);
								  data.eventCallback(event);
							  });

	glfwSetWindowCloseCallback(_window,
							  [](GLFWwindow* window) {
								  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

								  WindowCloseEvent event;
								  data.eventCallback(event);
							  });

	glfwSetKeyCallback(_window,
					   [](GLFWwindow* window, int key, int scancode, int action, int mods) {
						   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

						   switch (action) {
							   case GLFW_PRESS:
							   {
								   KeyPressedEvent event(key, 0);
								   data.eventCallback(event);
								   break;
							   }
							   case GLFW_RELEASE:
							   {
								   KeyReleasedEvent event(key);
								   data.eventCallback(event);
								   break;
							   }
							   case GLFW_REPEAT:
							   {
								   KeyPressedEvent event(key, 1);
								   data.eventCallback(event);
								   break;
							   }   
							   default:
								   break;
						   }
					   });

	glfwSetCharCallback(_window,
					    [](GLFWwindow* window, unsigned int keyCode) {
							WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

							KeyTypedEvent event(keyCode);
							data.eventCallback(event);
					    });

	glfwSetMouseButtonCallback(_window,
							   [](GLFWwindow* window, int button, int action, int mods) {
								   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

								   switch (action) {
									   case GLFW_PRESS:
									   {
										   MouseButtonPressedEvent event(button);
										   data.eventCallback(event);
										   break;
									   }
									   case GLFW_RELEASE:
									   {
										   MouseButtonReleasedEvent event(button);
										   data.eventCallback(event);
										   break;
									   }
									   default:
										   break;
								   }
							   });

	glfwSetScrollCallback(_window,
						  [](GLFWwindow* window, double xOffset, double yOffset) {
							  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

							  MouseScrolledEvent event((float)xOffset, (float)yOffset);
							  data.eventCallback(event);
						  });

	glfwSetCursorPosCallback(_window,
						     [](GLFWwindow* window, double xPos, double yPos) {
								 WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

								 MouseMovedEvent event((float)xPos, (float)yPos);
								 data.eventCallback(event);
							 });
}

void WindowsWindow::shutdown() {
	glfwDestroyWindow(_window);
}

void WindowsWindow::onUpdate() {
	glfwPollEvents();
	_context->swapBuffers();
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