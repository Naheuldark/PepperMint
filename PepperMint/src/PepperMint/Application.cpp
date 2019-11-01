#include "pmpch.h"

#include "Application.h"

#include "GLFW/glfw3.h"

namespace PepperMint {

Application::Application() {
	_window = std::unique_ptr<Window>(Window::Create());
}

void Application::run() {
	while (_running) {
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		_window->onUpdate();
	}
}
}