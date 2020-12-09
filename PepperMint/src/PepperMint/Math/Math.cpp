#include "pmpch.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include "PepperMint/Math/Math.h"

namespace PepperMint {
namespace Math {

bool decompose(const glm::mat4& iTransform, glm::vec3& oTranslation, glm::vec3& oRotation, glm::vec3& oScale) {
    // Taken from glm::decompose in matrix_decompose.inl

    glm::mat4 localMatrix(iTransform);

    if (glm::epsilonEqual(localMatrix[3][3], static_cast<float>(0), glm::epsilon<float>())) {
        return false;
    }

    if (glm::epsilonNotEqual(localMatrix[0][3], static_cast<float>(0), glm::epsilon<float>()) ||
        glm::epsilonNotEqual(localMatrix[1][3], static_cast<float>(0), glm::epsilon<float>()) ||
        glm::epsilonNotEqual(localMatrix[2][3], static_cast<float>(0), glm::epsilon<float>())) {
        localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<float>(0);
        localMatrix[3][3]                                         = static_cast<float>(1);
    }

    oTranslation   = glm::vec3(localMatrix[3]);
    localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

    glm::vec3 row[3]{};

    for (glm::length_t i = 0; i < 3; ++i) {
        for (glm::length_t j = 0; j < 3; ++j) {
            row[i][j] = localMatrix[i][j];
        }
    }

    oScale.x = glm::length(row[0]);
    row[0]   = glm::detail::scale(row[0], static_cast<float>(1));
    oScale.y = glm::length(row[1]);
    row[1]   = glm::detail::scale(row[1], static_cast<float>(1));
    oScale.z = glm::length(row[2]);
    row[2]   = glm::detail::scale(row[2], static_cast<float>(1));

    oRotation.y = asin(-row[0][2]);
    if (cos(oRotation.y) != 0) {
        oRotation.x = atan2(row[1][2], row[2][2]);
        oRotation.z = atan2(row[0][1], row[0][0]);
    } else {
        oRotation.x = atan2(-row[2][0], row[1][1]);
        oRotation.z = 0;
    }

    return true;
}
}
}
