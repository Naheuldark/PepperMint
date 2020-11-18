#pragma once

#include <glm/glm.hpp>

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
    glm::mat4 transform{1.0f};

    TransformComponent()                          = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::mat4& iTransform) : transform(iTransform) {}

    operator glm::mat4&() { return transform; }
    operator const glm::mat4&() const { return transform; }
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
    ScriptableEntity* instance = nullptr;

    std::function<void()> createInstanceFunc;
    std::function<void()> destroyInstanceFunc;

    std::function<void(ScriptableEntity*)>           onCreateFunc;
    std::function<void(ScriptableEntity*)>           onDestroyFunc;
    std::function<void(ScriptableEntity*, Timestep)> onUpdateFunc;

    template <typename Script>
    void bind() {
        createInstanceFunc  = [&]() { instance = new Script(); };
        destroyInstanceFunc = [&]() {
            delete (Script*)instance;
            instance = nullptr;
        };

        onCreateFunc  = [](ScriptableEntity* script) { ((Script*)script)->onCreate(); };
        onDestroyFunc = [](ScriptableEntity* script) { ((Script*)script)->onDestroy(); };
        onUpdateFunc  = [](ScriptableEntity* script, Timestep timestep) { ((Script*)script)->onUpdate(timestep); };
    }
};
}
