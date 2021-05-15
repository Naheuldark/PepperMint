#pragma once

#include "PepperMint/Core/Application.h"
#include "PepperMint/Core/Base.h"

#ifdef PM_PLATFORM_WINDOWS

extern PepperMint::Application* PepperMint::CreateApplication(PepperMint::ApplicationCommandLineArgs iArgs);

int main(int argc, char** argv) {
    PepperMint::Log::Init();

    PM_PROFILE_BEGIN_SESSION("Startup", "PepperMint-Startup.json");
    auto&& app = PepperMint::CreateApplication({argc, argv});
    PM_PROFILE_END_SESSION();

    PM_PROFILE_BEGIN_SESSION("Runtime", "PepperMint-Runtime.json");
    app->run();
    PM_PROFILE_END_SESSION();

    PM_PROFILE_BEGIN_SESSION("Shutdown", "PepperMint-Shutdown.json");
    delete app;
    PM_PROFILE_END_SESSION();
}

#endif
