#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PepperMint/Renderer/Texture.h"
#include "PepperMint/Scene/SceneCamera.h"
#include "PepperMint/Scene/ScriptableEntity.h"

namespace YAML {
class Emitter;
class Node;
}

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
    glm::vec4      color        = {1.0f, 1.0f, 1.0f, 1.0f};
    Ref<Texture2D> texture      = nullptr;
    float          tilingFactor = 1.0f;

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

    NativeScriptComponent()                             = default;
    NativeScriptComponent(const NativeScriptComponent&) = default;

    void        serialize(YAML::Emitter& out) const;
    static void Deserialize(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity);
};

struct RigidBody2DComponent {
    enum class BodyType : int { STATIC = 0, DYNAMIC, KINEMATIC };

    BodyType type          = BodyType::STATIC;
    bool     fixedRotation = false;

    // Runtime storage
    void* runtimeBody = nullptr;

    RigidBody2DComponent()                            = default;
    RigidBody2DComponent(const RigidBody2DComponent&) = default;

    void        serialize(YAML::Emitter& out) const;
    static void Deserialize(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity);
};

struct BoxCollider2DComponent {
    glm::vec2 offset = {0.0f, 0.0f};
    glm::vec2 size   = {0.5f, 0.5f};

    // TODO Move into physics material
    float density              = 1.0f;
    float friction             = 0.5f;
    float restitution          = 0.0f;
    float restitutionThreshold = 0.5f;

    // Runtime storage
    void* runtimeFixture = nullptr;

    BoxCollider2DComponent()                              = default;
    BoxCollider2DComponent(const BoxCollider2DComponent&) = default;

    void        serialize(YAML::Emitter& out) const;
    static void Deserialize(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity);
};

}
