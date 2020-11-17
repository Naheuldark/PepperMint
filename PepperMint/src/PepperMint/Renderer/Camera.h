#pragma once

#include <glm/glm.hpp>

namespace PepperMint {

class Camera {
  public:
    Camera() = default;
    Camera(const glm::mat4& iProjection) : _projection(iProjection) {}

    virtual ~Camera() = default;

    const glm::mat4& projection() const { return _projection; }

  protected:
    glm::mat4 _projection = glm::mat4(1.0f);
};
}
