#pragma once

#include "Core.h"

namespace PepperMint {

class PM_API Application {
public:
	Application() = default;
	virtual ~Application() = default;

	void run();
};

// To be defined in CLIENT
Application* CreateApplication();
}
