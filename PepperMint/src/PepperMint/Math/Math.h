#pragma once

#include <glm/glm.hpp>

namespace PepperMint {
namespace Math {

bool decompose(const glm::mat4& iTransform, glm::vec3& oTranslation, glm::vec3& oRotation, glm::vec3& oScale);
}
}
