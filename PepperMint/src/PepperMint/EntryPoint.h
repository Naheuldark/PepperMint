#pragma once

#include <iostream>

#include "Application.h"

#ifdef PM_PLATFORM_WINDOWS

extern PepperMint::Application* PepperMint::CreateApplication();

int main(int argc, char** argv) {
	std::cout << "PepperMint Engine\n";
	auto app = PepperMint::CreateApplication();
	app->Run();
	delete app;
}

#endif