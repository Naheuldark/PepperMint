#pragma once

#include "PepperMint/Core/Base.h"
#include "PepperMint/Core/LayerStack.h"
#include "PepperMint/Core/Timestep.h"
#include "PepperMint/Core/Window.h"
#include "PepperMint/Events/ApplicationEvent.h"
#include "PepperMint/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace PepperMint {

struct ApplicationCommandLineArgs {
    int    count = 0;
    char** args  = nullptr;

    const char* operator[](int iIndex) const {
        PM_CORE_ASSERT(iIndex < count);
        return args[iIndex];
    }
};

struct ApplicationSpecification {
    std::string                name = "PepperMint App";
    std::string                workingDirectory;
    ApplicationCommandLineArgs commandLineArgs;
};

class Application {
  public:
    Application(const ApplicationSpecification& iSpecifications);
    virtual ~Application();

    void onEvent(Event& iEvent);

    void pushLayer(Ref<Layer> iLayer);
    void pushOverlay(Ref<Layer> iOverlay);

    void close();

    static Application&             Get() { return *sInstance; }
    Window&                         window() { return *_window; }
    Ref<ImGuiLayer>                 imguiLayer() { return _imguiLayer; }
    const ApplicationSpecification& specification() const { return _specification; }

  private:
    void run();

    bool onWindowClose(WindowCloseEvent& iEvent);
    bool onWindowResize(WindowResizeEvent& iEvent);

  private:
    Scope<Window>            _window;
    Ref<ImGuiLayer>          _imguiLayer;
    ApplicationSpecification _specification;
    bool                     _running   = true;
    bool                     _minimized = false;
    LayerStack               _layerStack;
    float                    _lastFrameTime = 0.0f;

  private:
    static Application* sInstance;
    friend int ::main(int argc, char** argv);
};

// To be defined in CLIENT
Application* CreateApplication(ApplicationCommandLineArgs iArgs);
}
