#include "pmpch.h"

#include "ImGuiLayer.h"

#include "PepperMint/Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "PepperMint/Application.h"

// TODO remove
#include <GLFW/glfw3.h>

namespace PepperMint {

void ImGuiLayer::onAttach() {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	// TODO: use PepperMint key codes
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::onDetach() {

}

void ImGuiLayer::onUpdate() {
	ImGuiIO& io = ImGui::GetIO();

	Application& app = Application::Get();
	io.DisplaySize = ImVec2(app.window().width(), app.window().height());

	float time = (float)glfwGetTime();
	io.DeltaTime = _time > 0.0f ? (time - _time) : (1.0f / 60.0f);
	_time = time;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::onEvent(Event& iEvent) {
	EventDispatcher dispatcher(iEvent);
	dispatcher.dispatch<MouseButtonPressedEvent>(PM_BIND_EVENT_FN(ImGuiLayer::onMouseButtonPressedEvent));
	dispatcher.dispatch<MouseButtonReleasedEvent>(PM_BIND_EVENT_FN(ImGuiLayer::onMouseButtonReleasedEvent));
	dispatcher.dispatch<MouseMovedEvent>(PM_BIND_EVENT_FN(ImGuiLayer::onMouseMovedEvent));
	dispatcher.dispatch<MouseScrolledEvent>(PM_BIND_EVENT_FN(ImGuiLayer::onMouseScrolledEvent));
	dispatcher.dispatch<KeyPressedEvent>(PM_BIND_EVENT_FN(ImGuiLayer::onKeyPressedEvent));
	dispatcher.dispatch<KeyReleasedEvent>(PM_BIND_EVENT_FN(ImGuiLayer::onKeyReleasedEvent));
	dispatcher.dispatch<KeyTypedEvent>(PM_BIND_EVENT_FN(ImGuiLayer::onKeyTypedEvent));
	dispatcher.dispatch<WindowResizeEvent>(PM_BIND_EVENT_FN(ImGuiLayer::onWindowResizeEvent));
}

bool ImGuiLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& iEvent) {
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[iEvent.mouseButton()] = true;

	return false;
}

bool ImGuiLayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& iEvent) {
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[iEvent.mouseButton()] = false;

	return false;
}

bool ImGuiLayer::onMouseMovedEvent(MouseMovedEvent& iEvent) {
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2(iEvent.x(), iEvent.y());

	return false;
}

bool ImGuiLayer::onMouseScrolledEvent(MouseScrolledEvent& iEvent) {
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH += iEvent.xOffset();
	io.MouseWheel += iEvent.yOffset();

	return false;
}

bool ImGuiLayer::onKeyPressedEvent(KeyPressedEvent& iEvent) {
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[iEvent.keyCode()] = true;

	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

	return false;
}

bool ImGuiLayer::onKeyReleasedEvent(KeyReleasedEvent& iEvent) {
	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[iEvent.keyCode()] = false;

	return false;
}

bool ImGuiLayer::onKeyTypedEvent(KeyTypedEvent& iEvent) {
	ImGuiIO& io = ImGui::GetIO();
	
	int keyCode = iEvent.keyCode();
	if (keyCode > 0 && keyCode < 0x10000) {
		io.AddInputCharacter((unsigned short)keyCode);
	}

	return false;
}

bool ImGuiLayer::onWindowResizeEvent(WindowResizeEvent& iEvent) {
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(iEvent.width(), iEvent.height());
	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	glViewport(0, 0, iEvent.width(), iEvent.height());

	return false;
}
}