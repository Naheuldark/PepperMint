#pragma once

#include <glm/glm.hpp>

#include "PepperMint/Core/Timestep.h"
#include "PepperMint/Events/Event.h"
#include "PepperMint/Events/MouseEvent.h"
#include "PepperMint/Renderer/Camera.h"

namespace PepperMint {

class EditorCamera : public Camera {
  public:
    EditorCamera() = default;
    EditorCamera(float iFOV, float iAspectRatio, float iNearClip, float iFarClip);

    ~EditorCamera() = default;

    void onUpdate(Timestep iTimestep);
    void onEvent(Event& iEvent);

    float distance() const { return _distance; }
    void  setDistance(float iDistance) { _distance = iDistance; }

    void setViewportSize(float iWidth, float iHeight) {
        _viewportWidth  = iWidth;
        _viewportHeight = iHeight;
        updateProjection();
    }

    glm::mat4 viewMatrix() const { return _viewMatrix; }
    glm::mat4 viewProjection() const { return _projection * _viewMatrix; }

    glm::vec3 upDirection() const;
    glm::vec3 rightDirection() const;
    glm::vec3 forwardDirection() const;

    glm::vec3 position() const { return _position; }
    glm::quat orientation() const;

  private:
    void updateProjection();
    void updateView();

    bool onMouseScroll(MouseScrolledEvent& iEvent);

    void mousePan(const glm::vec2& iDelta);
    void mouseRotate(const glm::vec2& iDelta);
    void mouseZoom(float iDelta);

    glm::vec3 computePosition() const;

    std::pair<float, float> panSpeed() const;
    float                   rotationSpeed() const;
    float                   zoomSpeed() const;

  private:
    float _FOV         = 45.0f;
    float _aspectRatio = 1.778f;
    float _nearClip    = 0.1f;
    float _farClip     = 1000.0f;

    glm::mat4 _viewMatrix{};
    glm::vec3 _position   = {0.0f, 0.0f, 0.0f};
    glm::vec3 _focalPoint = {0.0f, 0.0f, 0.0f};

    glm::vec2 _initialMousePosition = {0.0f, 0.0f};

    float _distance = 10.0f;
    float _pitch    = 0.0f;
    float _yaw      = 0.0f;

    float _viewportWidth  = 1280.0f;
    float _viewportHeight = 720.0f;
};
}
