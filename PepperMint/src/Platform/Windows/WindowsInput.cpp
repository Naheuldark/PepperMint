#include "pmpch.h"

#include <GLFW/glfw3.h>

#include "PepperMint/Core/Application.h"
#include "PepperMint/Core/Input.h"

namespace PepperMint {

bool Input::IsKeyPressed(const KeyCode iKeyCode) {
    auto&& window = static_cast<GLFWwindow*>(Application::Get().window().nativeWindow());
    auto&& state  = glfwGetKey(window, static_cast<int>(iKeyCode));
    return (state == GLFW_PRESS);
}

bool Input::IsMouseButtonPressed(const MouseCode iButton) {
    auto&& window = static_cast<GLFWwindow*>(Application::Get().window().nativeWindow());
    auto&& state  = glfwGetMouseButton(window, static_cast<int>(iButton));
    return (state == GLFW_PRESS);
}

glm::vec2 Input::GetMousePosition() {
    auto&& window = static_cast<GLFWwindow*>(Application::Get().window().nativeWindow());

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return {(float)xpos, (float)ypos};
}

float Input::GetMouseX() { return GetMousePosition().x; }

float Input::GetMouseY() { return GetMousePosition().y; }
}
