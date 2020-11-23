#include "pmpch.h"

#include <glm/gtc/matrix_transform.hpp>

#include "PepperMint/Scene/SceneCamera.h"

namespace PepperMint {

SceneCamera::SceneCamera() { computeProjection(); }

void SceneCamera::setPerspective(float iVerticalFOV, float iNearClip, float iFarClip) {
    _projectionType  = ProjectionType::PERSPECTIVE;
    _perspectiveFOV  = iVerticalFOV;
    _perspectiveNear = iNearClip;
    _perspectiveFar  = iFarClip;
    computeProjection();
}

void SceneCamera::setOrthographic(float iSize, float iNearClip, float iFarClip) {
    _projectionType   = ProjectionType::ORTHOGRAPHIC;
    _orthographicSize = iSize;
    _orthographicNear = iNearClip;
    _orthographicFar  = iFarClip;
    computeProjection();
}

void SceneCamera::setViewportSize(uint32_t iWidth, uint32_t iHeight) {
    _aspectRatio = (float)iWidth / (float)iHeight;
    computeProjection();
}

void SceneCamera::computeProjection() {
    if (_projectionType == ProjectionType::PERSPECTIVE) {
        _projection = glm::perspective(_perspectiveFOV, _aspectRatio, _perspectiveNear, _perspectiveFar);
    } else {
        float orthoLeft   = -_orthographicSize * _aspectRatio * 0.5f;
        float orthoRight  = _orthographicSize * _aspectRatio * 0.5f;
        float orthoBottom = -_orthographicSize * 0.5f;
        float orthoTop    = _orthographicSize * 0.5f;

        _projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, _orthographicNear, _orthographicFar);
    }
}
}
