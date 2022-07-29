#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "PepperMint/Core/UUID.h"
#include "PepperMint/Renderer/Texture.h"
#include "PepperMint/Scene/SceneCamera.h"

namespace YAML {
class Emitter;
class Node;
}

namespace PepperMint {

struct IdComponent {
    UUID uuid;

    IdComponent()                   = default;
    IdComponent(const IdComponent&) = default;
};

struct TagComponent {
    std::string tag;

    TagComponent()                    = default;
    TagComponent(const TagComponent&) = default;
};

struct TransformComponent {
    glm::vec3 translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation    = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale       = {1.0f, 1.0f, 1.0f};

    TransformComponent()                          = default;
    TransformComponent(const TransformComponent&) = default;

    glm::mat4 transform() const {
        auto&& translate = glm::translate(glm::mat4(1.0f), translation);
        auto&& rotate    = glm::toMat4(glm::quat(rotation));
        auto&& rescale   = glm::scale(glm::mat4(1.0f), scale);

        return translate * rotate * rescale;
    }
};

struct SpriteRendererComponent {
    glm::vec4      color        = {1.0f, 1.0f, 1.0f, 1.0f};
    Ref<Texture2D> texture      = nullptr;
    float          tilingFactor = 1.0f;

    SpriteRendererComponent()                               = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
};

struct CircleRendererComponent {
    glm::vec4 color     = {1.0f, 1.0f, 1.0f, 1.0f};
    float     thickness = 1.0f;
    float     fade      = 0.005f;

    CircleRendererComponent()                               = default;
    CircleRendererComponent(const CircleRendererComponent&) = default;
};

struct CameraComponent {
    SceneCamera camera;
    bool        primary          = true; // TODO Move to Scene
    bool        fixedAspectRatio = false;

    CameraComponent()                       = default;
    CameraComponent(const CameraComponent&) = default;
};

class ScriptableEntity; // TODO
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
};

struct RigidBody2DComponent {
    enum class BodyType : int { STATIC = 0, DYNAMIC, KINEMATIC };

    BodyType type          = BodyType::STATIC;
    bool     fixedRotation = false;

    // Runtime storage
    void* runtimeBody = nullptr;

    RigidBody2DComponent()                            = default;
    RigidBody2DComponent(const RigidBody2DComponent&) = default;
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
};

struct CircleCollider2DComponent {
    glm::vec2 offset = {0.0f, 0.0f};
    float     radius = 0.5f;

    // TODO Move into physics material
    float density              = 1.0f;
    float friction             = 0.5f;
    float restitution          = 0.0f;
    float restitutionThreshold = 0.5f;

    // Runtime storage
    void* runtimeFixture = nullptr;

    CircleCollider2DComponent()                                 = default;
    CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
};

template <typename... Component>
struct ComponentGroup {};

using AllComponents = ComponentGroup<TransformComponent,
                                     SpriteRendererComponent,
                                     CircleRendererComponent,
                                     CameraComponent,
                                     NativeScriptComponent,
                                     RigidBody2DComponent,
                                     BoxCollider2DComponent,
                                     CircleCollider2DComponent>;

}
