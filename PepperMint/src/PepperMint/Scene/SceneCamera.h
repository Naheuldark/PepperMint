#pragma once

#include "PepperMint/Renderer/Camera.h"

namespace PepperMint {

class SceneCamera : public Camera {
  public:
    enum class ProjectionType : int { PERSPECTIVE = 0, ORTHOGRAPHIC };

  public:
    SceneCamera();

    void setPerspective(float iVerticalFOV, float iNearClip, float iFarClip);
    void setOrthographic(float iSize, float iNearClip, float iFarClip);

    void setViewportSize(uint32_t iWidth, uint32_t iHeight);

    ProjectionType projectionType() const { return _projectionType; }
    void           setProjectionType(ProjectionType iType) {
        _projectionType = iType;
        computeProjection();
    }

    ///////////////////////////////////
    // Perspective camera properties //
    ///////////////////////////////////
    float perspectiveVerticalFOC() const { return _perspectiveFOV; }
    void  setPerspectiveVerticalFOC(float iVerticalFOV) {
        _perspectiveFOV = iVerticalFOV;
        computeProjection();
    }

    float perspectiveNearClip() const { return _perspectiveNear; }
    void  setPerspectiveNearClip(float iNearClip) {
        _perspectiveNear = iNearClip;
        computeProjection();
    }

    float perspectiveFarClip() const { return _perspectiveFar; }
    void  setPerspectiveFarClip(float iFarClip) {
        _perspectiveFar = iFarClip;
        computeProjection();
    }

    ////////////////////////////////////
    // Orthographic camera properties //
    ////////////////////////////////////
    float orthographicSize() const { return _orthographicSize; }
    void  setOrthographicSize(float iSize) {
        _orthographicSize = iSize;
        computeProjection();
    }

    float orthographicNearClip() const { return _orthographicNear; }
    void  setOrthographicNearClip(float iNearClip) {
        _orthographicNear = iNearClip;
        computeProjection();
    }

    float orthographicFarClip() const { return _orthographicFar; }
    void  setOrthographicFarClip(float iFarClip) {
        _orthographicFar = iFarClip;
        computeProjection();
    }

  private:
    void computeProjection();

  private:
    ProjectionType _projectionType = ProjectionType::ORTHOGRAPHIC;

    float _perspectiveFOV  = glm::radians(45.0f);
    float _perspectiveNear = 0.01f;
    float _perspectiveFar  = 1000.0f;

    float _orthographicSize = 10.0f;
    float _orthographicNear = -1.0f;
    float _orthographicFar  = 1.0f;

    float _aspectRatio = 0.0f;
};
}
