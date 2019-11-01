#include "pmpch.h"

#include "Application.h"

#include "GLFW/glfw3.h"

namespace PepperMint {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application::Application() {
	_window = std::unique_ptr<Window>(Window::Create());
	_window->setEventCallback(BIND_EVENT_FN(onEvent));
}

void Application::run() {
	while (_running) {
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		_window->onUpdate();
	}
}

void Application::onEvent(Event& iEvent) {
	EventDispatcher dispatcher(iEvent);
	dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

	PM_CORE_TRACE("{0}", iEvent)
}

bool Application::onWindowClose(WindowCloseEvent& iEvent) {
	_running = false;
	return true;
}

}