#include "pmpch.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLContext.h"

namespace PepperMint {

OpenGLContext::OpenGLContext(GLFWwindow* iWindowHandle) : _windowHandle(iWindowHandle) {
    PM_CORE_ASSERT(iWindowHandle, "Window handle shall not be null!")
}

void OpenGLContext::init() {
    PM_PROFILE_FUNCTION();

    glfwMakeContextCurrent(_windowHandle);
    int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    PM_CORE_ASSERT(success, "Failed to intialize glad!")

    PM_CORE_INFO("OpenGL Info:");
    PM_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
    PM_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
    PM_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

    PM_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "PepperMint requires at least OpenGL v4.5");
}

void OpenGLContext::swapBuffers() {
    PM_PROFILE_FUNCTION();

    glfwSwapBuffers(_windowHandle);
}
}
