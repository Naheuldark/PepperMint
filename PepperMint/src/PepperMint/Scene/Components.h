#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PepperMint/Scene/SceneCamera.h"
#include "PepperMint/Scene/ScriptableEntity.h"

namespace PepperMint {

struct TagComponent {
    std::string tag;

    TagComponent()                    = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& iTag) : tag(iTag) {}
};

struct TransformComponent {
    glm::vec3 translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation    = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale       = {1.0f, 1.0f, 1.0f};

    TransformComponent()                          = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& iTranslation, const glm::vec3& iRotation = {0.0f, 0.0f, 0.0f}, const glm::vec3& iScale = {1.0f, 1.0f, 1.0f})
        : translation(iTranslation), rotation(iRotation), scale(iScale) {}

    glm::mat4 transform() const {
        auto&& translate = glm::translate(glm::mat4(1.0f), translation);
        auto&& rotate    = glm::rotate(glm::mat4(1.0f), rotation.x, {1, 0, 0}) * glm::rotate(glm::mat4(1.0f), rotation.y, {0, 1, 0}) *
                        glm::rotate(glm::mat4(1.0f), rotation.z, {0, 0, 1});
        auto&& rescale = glm::scale(glm::mat4(1.0f), scale);

        return translate * rotate * rescale;
    }
};

struct SpriteRendererComponent {
    glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

    SpriteRendererComponent()                               = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const glm::vec4& iColor) : color(iColor) {}
};

struct CameraComponent {
    SceneCamera camera;
    bool        primary          = true; // TODO Move to Scene
    bool        fixedAspectRatio = false;

    CameraComponent()                       = default;
    CameraComponent(const CameraComponent&) = default;
};

struct NativeScriptComponent {
    ScriptableEntity* script = nullptr;

    ScriptableEntity* (*instantiateScript)();
    void (*destroyScript)(NativeScriptComponent*);

    template <typename Script>
    void bind() {
        instantiateScript = []() { return static_cast<ScriptableEntity*>(new Script()); };
        destroyScript     = [](NativeScriptComponent* component) {
            delete component->script;
            component->script = nullptr;
        };
    }
};
}
