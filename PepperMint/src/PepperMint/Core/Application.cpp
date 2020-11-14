#include "PepperMint/Core/Application.h"
#include "pmpch.h"

#include "PepperMint/Core/Input.h"
#include "PepperMint/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace PepperMint {

Application* Application::sInstance = nullptr;

Application::Application(const std::string& iName) {
    PM_PROFILE_FUNCTION();

    PM_CORE_ASSERT(!sInstance, "Application already exists!")
    sInstance = this;

    _window = Window::Create(WindowProperties(iName));
    _window->setEventCallback(PM_BIND_EVENT_FN(Application::onEvent));

    Renderer::Init();

    _imguiLayer = CreateRef<ImGuiLayer>();
    pushOverlay(_imguiLayer);
}

Application::~Application() {
    PM_PROFILE_FUNCTION();

    Renderer::Shutdown();
}

void Application::run() {
    PM_PROFILE_FUNCTION();

    while (_running) {
        PM_PROFILE_SCOPE("Run Loop");

        float    time     = (float)glfwGetTime();
        Timestep timestep = time - _lastFrameTime;
        _lastFrameTime    = time;

        if (!_minimized) {
            {
                PM_PROFILE_SCOPE("LayerStack onUpdate calls");
                for (auto&& layer : _layerStack) {
                    layer->onUpdate(timestep);
                }
            }

            _imguiLayer->begin();
            {
                PM_PROFILE_SCOPE("LayerStack onImGuiRender calls");
                for (auto&& layer : _layerStack) {
                    layer->onImGuiRender();
                }
            }
            _imguiLayer->end();
        }

        _window->onUpdate();
    }
}

void Application::close() { _running = false; }

void Application::pushLayer(Ref<Layer> iLayer) {
    PM_PROFILE_FUNCTION();

    _layerStack.pushLayer(iLayer);
    iLayer->onAttach();
}

void Application::pushOverlay(Ref<Layer> iOverlay) {
    PM_PROFILE_FUNCTION();

    _layerStack.pushOverlay(iOverlay);
    iOverlay->onAttach();
}

void Application::onEvent(Event& iEvent) {
    PM_PROFILE_FUNCTION();

    EventDispatcher dispatcher(iEvent);
    dispatcher.dispatch<WindowCloseEvent>(PM_BIND_EVENT_FN(Application::onWindowClose));
    dispatcher.dispatch<WindowResizeEvent>(PM_BIND_EVENT_FN(Application::onWindowResize));

    for (auto it = _layerStack.rbegin(); it != _layerStack.rend(); ++it) {
        if (iEvent.handled())
            break;
        (*it)->onEvent(iEvent);
    }
}

bool Application::onWindowClose(WindowCloseEvent& iEvent) {
    PM_PROFILE_FUNCTION();

    _running = false;
    return true;
}

bool Application::onWindowResize(WindowResizeEvent& iEvent) {
    PM_PROFILE_FUNCTION();

    if (iEvent.width() == 0 || iEvent.height() == 0) {
        _minimized = true;
        return false;
    }

    _minimized = false;
    Renderer::OnWindowResize(iEvent.width(), iEvent.height());

    return false;
}
}
