#include "pmpch.h"

#include <GLFW/glfw3.h>

#include "PepperMint/Core/Input.h"
#include "PepperMint/Events/ApplicationEvent.h"
#include "PepperMint/Events/KeyEvent.h"
#include "PepperMint/Events/MouseEvent.h"
#include "PepperMint/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/Windows/WindowsWindow.h"

namespace PepperMint {

static uint8_t sGLFWWindowCount = 0;

WindowsWindow::WindowsWindow(const WindowProperties& iProperties) {
    PM_PROFILE_FUNCTION();

    init(iProperties);
}

WindowsWindow::~WindowsWindow() {
    PM_PROFILE_FUNCTION();

    shutdown();
}

void WindowsWindow::init(const WindowProperties& iProperties) {
    PM_PROFILE_FUNCTION();

    _data.title  = iProperties.title;
    _data.width  = iProperties.width;
    _data.height = iProperties.height;

    PM_CORE_INFO("Creating window {0} ({1} x {2})", _data.title, _data.width, _data.height);

    if (sGLFWWindowCount == 0) {
        PM_PROFILE_SCOPE("glfwInit");
        int success = glfwInit();
        PM_CORE_ASSERT(success, "Failed to initialize GLFW!");

        glfwSetErrorCallback([](int iError, const char* iDescription) {
            PM_CORE_ERROR("GLFW Error ({0}): {1}", iError, iDescription)
        });
    }

    {
        PM_PROFILE_SCOPE("glfwCreateWindow");
#ifdef PM_DEBUG
        if (RendererAPI::GetAPI() == RendererAPI::API::OPENGL) {
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        }
#endif // PM_DEBUG

        _window = glfwCreateWindow(
            (int)_data.width, (int)_data.height, _data.title.c_str(), nullptr, nullptr);
        ++sGLFWWindowCount;
    }

    _context = GraphicsContext::Create(_window);
    _context->init();

    glfwSetWindowUserPointer(_window, &_data);
    setVSync(true);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.width       = width;
        data.height      = height;

        WindowResizeEvent event(width, height);
        data.eventCallback(event);
    });

    glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        WindowCloseEvent event;
        data.eventCallback(event);
    });

    glfwSetKeyCallback(_window,
                       [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                           WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                           switch (action) {
                           case GLFW_PRESS: {
                               KeyPressedEvent event(static_cast<KeyCode>(key), 0);
                               data.eventCallback(event);
                               break;
                           }
                           case GLFW_RELEASE: {
                               KeyReleasedEvent event(static_cast<KeyCode>(key));
                               data.eventCallback(event);
                               break;
                           }
                           case GLFW_REPEAT: {
                               KeyPressedEvent event(static_cast<KeyCode>(key), 1);
                               data.eventCallback(event);
                               break;
                           }
                           default:
                               break;
                           }
                       });

    glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keyCode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        KeyTypedEvent event(static_cast<KeyCode>(keyCode));
        data.eventCallback(event);
    });

    glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS: {
            MouseButtonPressedEvent event(static_cast<MouseCode>(button));
            data.eventCallback(event);
            break;
        }
        case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
            data.eventCallback(event);
            break;
        }
        default:
            break;
        }
    });

    glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.eventCallback(event);
    });

    glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float)xPos, (float)yPos);
        data.eventCallback(event);
    });
}

void WindowsWindow::shutdown() {
    PM_PROFILE_FUNCTION();

    glfwDestroyWindow(_window);
    --sGLFWWindowCount;

    if (sGLFWWindowCount == 0) {
        glfwTerminate();
    }
}

void WindowsWindow::onUpdate() {
    PM_PROFILE_FUNCTION();

    glfwPollEvents();
    _context->swapBuffers();
}

void WindowsWindow::setVSync(bool iEnabled) {
    PM_PROFILE_FUNCTION();

    if (iEnabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    _data.vsync = iEnabled;
}
}
