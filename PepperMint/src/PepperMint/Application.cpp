#include "Application.h"

#include "PepperMint/Events/ApplicationEvent.h"
#include "PepperMint/Log.h"

namespace PepperMint {

void Application::run() {
	WindowResizeEvent e(1280, 720);
	PM_TRACE(e);

	while (true);
}
}