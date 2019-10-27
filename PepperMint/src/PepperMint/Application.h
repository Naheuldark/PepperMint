#pragma once

#include "Core.h"

namespace PepperMint {

	class PM_API Application {
	public:
		Application() = default;
		virtual ~Application() = default;

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}
