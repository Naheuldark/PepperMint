#pragma once

#ifdef PM_PLATFORM_WINDOWS

extern PepperMint::Application* PepperMint::CreateApplication();

int main(int argc, char** argv) {
	PepperMint::Log::Init();
	PM_CORE_WARN("Initiliazed Log!!");
	int a = 5;
	PM_INFO("Hello World!! Var={0}", a);

	auto app = PepperMint::CreateApplication();
	app->run();
	delete app;
}

#endif