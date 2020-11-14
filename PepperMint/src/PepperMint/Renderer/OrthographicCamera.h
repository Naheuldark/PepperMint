#pragma once

#include <glm/glm.hpp>

namespace PepperMint {

class OrthographicCamera {
  public:
    OrthographicCamera(float iLeft   = 0.0f,
                       float iRight  = 0.0f,
                       float iBottom = 0.0f,
                       float iTop    = 0.0f);
    ~OrthographicCamera() = default;

    inline const glm::vec3& position() const { return _position; }
    inline void             setPosition(const glm::vec3& iPosition) {
        _position = iPosition;
        computeViewMatrix();
    }

    inline float rotation() const { return _rotation; }
    inline void  setRotation(float iRotation) {
        _rotation = iRotation;
        computeViewMatrix();
    }

    inline const glm::mat4& projectionMatrix() const { return _projectionMatrix; }
    inline const glm::mat4& viewMatrix() const { return _viewMatrix; }
    inline const glm::mat4& viewProjectionMatrix() const { return _viewProjectionMatrix; }

    void setProjection(float iLeft, float iRight, float iBottom, float iTop);

  private:
    void computeViewMatrix();

  private:
    glm::mat4 _projectionMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _viewProjectionMatrix;

    glm::vec3 _position = {0.0f, 0.0f, 0.0f};
    float     _rotation = 0.0f;
};

}
