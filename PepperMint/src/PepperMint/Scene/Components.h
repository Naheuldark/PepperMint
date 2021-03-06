#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <yaml-cpp/yaml.h>

#include "PepperMint/Scene/SceneCamera.h"
#include "PepperMint/Scene/ScriptableEntity.h"

namespace PepperMint {

struct TagComponent {
    std::string tag;

    TagComponent()                    = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& iTag) : tag(iTag) {}

    void        serialize(YAML::Emitter& out) const;
    static void Deserialize(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity);
};

struct TransformComponent {
    glm::vec3 translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation    = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale       = {1.0f, 1.0f, 1.0f};

    TransformComponent()                          = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& iTranslation, const glm::vec3& iRotation = {0.0f, 0.0f, 0.0f}, const glm::vec3& iScale = {1.0f, 1.0f, 1.0f})
        : translation(iTranslation), rotation(iRotation), scale(iScale) {}

    glm::mat4 transform() const;

    void        serialize(YAML::Emitter& out) const;
    static void Deserialize(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity);
};

struct SpriteRendererComponent {
    glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

    SpriteRendererComponent()                               = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const glm::vec4& iColor) : color(iColor) {}

    void        serialize(YAML::Emitter& out) const;
    static void Deserialize(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity);
};

struct CameraComponent {
    SceneCamera camera;
    bool        primary          = true; // TODO Move to Scene
    bool        fixedAspectRatio = false;

    CameraComponent()                       = default;
    CameraComponent(const CameraComponent&) = default;

    void        serialize(YAML::Emitter& out) const;
    static void Deserialize(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity);
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

    void        serialize(YAML::Emitter& out) const;
    static void Deserialize(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity);
};
}
