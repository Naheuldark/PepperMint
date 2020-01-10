#pragma once

#ifdef PM_PLATFORM_WINDOWS

extern PepperMint::Application* PepperMint::CreateApplication();

int main(int argc, char** argv) {
	PepperMint::Log::Init();

	auto app = PepperMint::CreateApplication();
	app->run();
	delete app;
}

#endif