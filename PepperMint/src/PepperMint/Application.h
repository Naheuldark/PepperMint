#pragma once

#include "PepperMint/Core.h"
#include "PepperMint/Window.h"

namespace PepperMint {

class PM_API Application {
public:
	Application();
	virtual ~Application() = default;

	void run();

private:
	std::unique_ptr<Window> _window;
	bool _running = true;
};

// To be defined in CLIENT
Application* CreateApplication();
}
