#include "pmpch.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "PepperMint/Core/Input.h"
#include "PepperMint/Core/KeyCodes.h"
#include "PepperMint/Core/MouseCodes.h"
#include "PepperMint/Renderer/EditorCamera.h"

namespace PepperMint {

EditorCamera::EditorCamera(float iFOV, float iAspectRatio, float iNearClip, float iFarClip)
    : Camera(glm::perspective(glm::radians(iFOV), iAspectRatio, iNearClip, iFarClip)), _FOV(iFOV), _aspectRatio(iAspectRatio), _nearClip(iNearClip),
      _farClip(iFarClip) {
    updateView();
}

void EditorCamera::onUpdate(Timestep iTimestep) {
    if (Input::IsKeyPressed(Key::LEFT_ALT)) {
        auto&& mouse          = Input::GetMousePosition();
        auto&& delta          = (mouse - _initialMousePosition) * 0.003f;
        _initialMousePosition = mouse;

        if (Input::IsMouseButtonPressed(Mouse::BUTTON_MIDDLE)) {
            mousePan(delta);
        } else if (Input::IsMouseButtonPressed(Mouse::BUTTON_LEFT)) {
            mouseRotate(delta);
        } else if (Input::IsMouseButtonPressed(Mouse::BUTTON_RIGHT)) {
            mouseZoom(delta.y);
        }
    }

    updateView();
}

void EditorCamera::onEvent(Event& iEvent) {
    EventDispatcher dispatcher(iEvent);
    dispatcher.dispatch<MouseScrolledEvent>(PM_BIND_EVENT_FN(EditorCamera::onMouseScroll));
}

glm::vec3 EditorCamera::upDirection() const { return glm::rotate(orientation(), glm::vec3(0.0f, 1.0f, 0.0f)); }
glm::vec3 EditorCamera::rightDirection() const { return glm::rotate(orientation(), glm::vec3(1.0f, 0.0f, 0.0f)); }
glm::vec3 EditorCamera::forwardDirection() const { return glm::rotate(orientation(), glm::vec3(0.0f, 0.0f, -1.0f)); }

glm::quat EditorCamera::orientation() const { return glm::quat(glm::vec3(-_pitch, -_yaw, 0.0f)); }

void EditorCamera::updateProjection() {
    _aspectRatio = _viewportWidth / _viewportHeight;
    _projection  = glm::perspective(glm::radians(_FOV), _aspectRatio, _nearClip, _farClip);
}

void EditorCamera::updateView() {
    // _yaw = _pitch = 0.0f; // Lock the camera's rotation
    _position   = computePosition();
    _viewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), _position) * glm::toMat4(orientation()));
}

bool EditorCamera::onMouseScroll(MouseScrolledEvent& iEvent) {
    float delta = iEvent.yOffset() * 0.1f;
    mouseZoom(delta);
    updateView();
    return false;
}

void EditorCamera::mousePan(const glm::vec2& iDelta) {
    auto&& [xSpeed, ySpeed] = panSpeed();
    _focalPoint += -rightDirection() * iDelta.x * xSpeed * _distance;
    _focalPoint += upDirection() * iDelta.y * ySpeed * _distance;
}

void EditorCamera::mouseRotate(const glm::vec2& iDelta) {
    float yawSign = upDirection().y < 0.0f ? -1.0f : 1.0f;
    _yaw += yawSign * iDelta.x * rotationSpeed();
    _pitch += iDelta.y * rotationSpeed();
}

void EditorCamera::mouseZoom(float iDelta) {
    _distance -= iDelta * zoomSpeed();
    if (_distance < 1.0f) {
        _focalPoint += forwardDirection();
        _distance = 1.0f;
    }
}

glm::vec3 EditorCamera::computePosition() const { return _focalPoint - forwardDirection() * _distance; }

std::pair<float, float> EditorCamera::panSpeed() const {
    float x       = std::min(_viewportWidth / 1000.0f, 2.4f); // max = 2.4f
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y       = std::min(_viewportHeight / 1000.0f, 2.4f); // max = 2.4f
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return {xFactor, yFactor};
}

float EditorCamera::rotationSpeed() const { return 0.8f; }

float EditorCamera::zoomSpeed() const {
    float distance = std::max(_distance * 0.2f, 0.0f);
    return std::min(distance * distance, 100.0f); // max speed = 100
}
}
