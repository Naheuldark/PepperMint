#include "pmpch.h"
#include "PepperMint/Core/Input.h"

#include "PepperMint/Core/Application.h"

#include <GLFW/glfw3.h>

namespace PepperMint {

bool Input::IsKeyPressed(KeyCode iKeyCode) {
	auto window = static_cast<GLFWwindow*>(Application::Get().window().nativeWindow());
	auto state = glfwGetKey(window, static_cast<int>(iKeyCode));
	return (state == GLFW_PRESS) || (state == GLFW_REPEAT);
}

bool Input::IsMouseButtonPressed(MouseCode iButton) {
	auto window = static_cast<GLFWwindow*>(Application::Get().window().nativeWindow());
	auto state = glfwGetMouseButton(window, static_cast<int>(iButton));
	return (state == GLFW_PRESS);
}

std::pair<float, float> Input::GetMousePosition() {
	auto window = static_cast<GLFWwindow*>(Application::Get().window().nativeWindow());

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { (float)xpos, (float)ypos };
}

float Input::GetMouseX() {
	auto [x, y] = GetMousePosition();
	return x;
}

float Input::GetMouseY() {
	auto [x, y] = GetMousePosition();
	return y;
}
}