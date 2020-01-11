#include "pmpch.h"
#include "Platform/Windows/WindowsInput.h"

#include "PepperMint/Core/Application.h"

#include <GLFW/glfw3.h>

namespace PepperMint {

Scope<Input> Input::sInstance = CreateScope<WindowsInput>();

bool WindowsInput::isKeyPressedImpl(KeyCode iKeyCode) {
	auto window = static_cast<GLFWwindow*>(Application::Get().window().nativeWindow());
	auto state = glfwGetKey(window, static_cast<int>(iKeyCode));
	return (state == GLFW_PRESS) || (state == GLFW_REPEAT);
}

bool WindowsInput::isMouseButtonPressedImpl(MouseCode iButton) {
	auto window = static_cast<GLFWwindow*>(Application::Get().window().nativeWindow());
	auto state = glfwGetMouseButton(window, static_cast<int>(iButton));
	return (state == GLFW_PRESS);
}

std::pair<float, float> WindowsInput::getMousePositionImpl() {
	auto window = static_cast<GLFWwindow*>(Application::Get().window().nativeWindow());

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { (float)xpos, (float)ypos };
}

float WindowsInput::getMouseXImpl() {
	auto [x, y] = getMousePositionImpl();
	return x;
}

float WindowsInput::getMouseYImpl() {
	auto [x, y] = getMousePositionImpl();
	return y;
}
}