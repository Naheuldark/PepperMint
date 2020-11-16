#pragma once

#include <glm/glm.hpp>

namespace PepperMint {

class Camera {
  public:
    Camera(const glm::mat4& iProjection) : _projection(iProjection) {}

    const glm::mat4& projection() const { return _projection; }

  private:
    glm::mat4 _projection;
};
}
