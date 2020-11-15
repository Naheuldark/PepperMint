#include "pmpch.h"

#include <glm/gtc/matrix_transform.hpp>

#include "PepperMint/Renderer/OrthographicCamera.h"

namespace PepperMint {

OrthographicCamera::OrthographicCamera(float iLeft, float iRight, float iBottom, float iTop)
    : _projectionMatrix(glm::ortho(iLeft, iRight, iBottom, iTop, -1.0f, 1.0f)), _viewMatrix(1.0f) {
    PM_PROFILE_FUNCTION();

    _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
}

void OrthographicCamera::setProjection(float iLeft, float iRight, float iBottom, float iTop) {
    PM_PROFILE_FUNCTION();

    _projectionMatrix     = glm::ortho(iLeft, iRight, iBottom, iTop, -1.0f, 1.0f);
    _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
}

void OrthographicCamera::computeViewMatrix() {
    PM_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(0, 0, 1));

    _viewMatrix           = glm::inverse(transform);
    _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
}
}
