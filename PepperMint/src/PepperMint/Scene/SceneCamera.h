#pragma once

#include "PepperMint/Renderer/Camera.h"

namespace PepperMint {

class SceneCamera : public Camera {
  public:
    SceneCamera();

    void setOrthographic(float iSize, float iNearClip, float iFarClip);
    void setViewportSize(uint32_t iWidth, uint32_t iHeight);

    float orthographicSize() const { return _orthographicSize; }
    void  setOrthographicSize(float iSize) {
        _orthographicSize = iSize;
        computeProjection();
    }

  private:
    void computeProjection();

  private:
    float _orthographicSize = 10.0f;
    float _orthographicNear = -1.0f;
    float _orthographicFar  = 1.0f;

    float _aspectRatio = 0.0f;
};
}
