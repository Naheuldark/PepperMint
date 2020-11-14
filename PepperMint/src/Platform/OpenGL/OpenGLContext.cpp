#include "Platform/OpenGL/OpenGLContext.h"
#include "pmpch.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

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

#ifdef PM_ENABLE_ASSERTS
    int versionMaj, versionMin;
    glGetIntegerv(GL_MAJOR_VERSION, &versionMaj);
    glGetIntegerv(GL_MINOR_VERSION, &versionMin);

    PM_CORE_ASSERT(versionMaj > 4 || (versionMaj == 4 && versionMin >= 5),
                   "PepperMint requires at least OpenGL v4.5");
#endif // PM_ENABLE_ASSERTS
}

void OpenGLContext::swapBuffers() {
    PM_PROFILE_FUNCTION();

    glfwSwapBuffers(_windowHandle);
}
}
