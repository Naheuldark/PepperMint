#include "pmpch.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "PepperMint/Scene/Components.h"
#include "PepperMint/Scene/Entity.h"
#include "PepperMint/Scene/SceneSerializer.h"

namespace YAML {

template <>
struct convert<glm::vec2> {
    static Node encode(const glm::vec2& rhs) {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }

    static bool decode(const Node& node, glm::vec2& rhs) {
        if (!node.IsSequence() || node.size() != 2)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        return true;
    }
};

template <>
struct convert<glm::vec3> {
    static Node encode(const glm::vec3& rhs) {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }

    static bool decode(const Node& node, glm::vec3& rhs) {
        if (!node.IsSequence() || node.size() != 3)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        return true;
    }
};

template <>
struct convert<glm::vec4> {
    static Node encode(const glm::vec4& rhs) {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.push_back(rhs.w);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }

    static bool decode(const Node& node, glm::vec4& rhs) {
        if (!node.IsSequence() || node.size() != 4)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        rhs.w = node[3].as<float>();
        return true;
    }
};
}

namespace PepperMint {

namespace {

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
    return out;
}

// Tag Component
void serializeTagComponent(YAML::Emitter& out, const TagComponent& iComponent) {
    out << YAML::Key << "TagComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Tag" << YAML::Value << iComponent.tag;
    out << YAML::EndMap;
}

void deserializeTagComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    auto&& serializedComponent = iSerializedEntity["TagComponent"];
    if (serializedComponent) {
        auto&& tag = serializedComponent["Tag"].as<std::string>();

        if (ioDeserializedEntity.has<TagComponent>()) {
            ioDeserializedEntity.get<TagComponent>().tag = tag;
        } else {
            ioDeserializedEntity.add<TagComponent>(tag);
        }
    }
}

// Transform Component
void serializeTransformComponent(YAML::Emitter& out, const TransformComponent& iComponent) {
    out << YAML::Key << "TransformComponent";
    out << YAML::BeginMap;
    {
        out << YAML::Key << "Translation" << YAML::Value << iComponent.translation;
        out << YAML::Key << "Rotation" << YAML::Value << iComponent.rotation;
        out << YAML::Key << "Scale" << YAML::Value << iComponent.scale;
    }
    out << YAML::EndMap;
}

void deserializeTransformComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    auto&& serializedComponent = iSerializedEntity["TransformComponent"];
    if (serializedComponent) {
        auto&& translation = serializedComponent["Translation"].as<glm::vec3>();
        auto&& rotation    = serializedComponent["Rotation"].as<glm::vec3>();
        auto&& scale       = serializedComponent["Scale"].as<glm::vec3>();

        if (ioDeserializedEntity.has<TransformComponent>()) {
            ioDeserializedEntity.get<TransformComponent>().translation = translation;
            ioDeserializedEntity.get<TransformComponent>().rotation    = rotation;
            ioDeserializedEntity.get<TransformComponent>().scale       = scale;
        } else {
            ioDeserializedEntity.add<TransformComponent>(translation, rotation, scale);
        }
    }
}

// Sprite Renderer Component
void serializeSpriteRendererComponent(YAML::Emitter& out, const SpriteRendererComponent& iComponent) {
    out << YAML::Key << "SpriteRendererComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Color" << YAML::Value << iComponent.color;
    out << YAML::EndMap;
}

void deserializeSpriteRendererComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    auto&& serializedComponent = iSerializedEntity["SpriteRendererComponent"];
    if (serializedComponent) {
        auto&& color = serializedComponent["Color"].as<glm::vec4>();

        if (ioDeserializedEntity.has<SpriteRendererComponent>()) {
            ioDeserializedEntity.get<SpriteRendererComponent>().color = color;
        } else {
            ioDeserializedEntity.add<SpriteRendererComponent>(color);
        }
    }
}

// Circle Renderer Component
void serializeCircleRendererComponent(YAML::Emitter& out, const CircleRendererComponent& iComponent) {
    out << YAML::Key << "CircleRendererComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Color" << YAML::Value << iComponent.color;
    out << YAML::Key << "Thickness" << YAML::Value << iComponent.thickness;
    out << YAML::Key << "Fade" << YAML::Value << iComponent.fade;
    out << YAML::EndMap;
}

void deserializeCircleRendererComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    auto&& serializedComponent = iSerializedEntity["CircleRendererComponent"];
    if (serializedComponent) {
        auto&& color     = serializedComponent["Color"].as<glm::vec4>();
        auto&& thickness = serializedComponent["Thickness"].as<float>();
        auto&& fade      = serializedComponent["Fade"].as<float>();

        if (ioDeserializedEntity.has<CircleRendererComponent>()) {
            ioDeserializedEntity.get<CircleRendererComponent>().color     = color;
            ioDeserializedEntity.get<CircleRendererComponent>().thickness = thickness;
            ioDeserializedEntity.get<CircleRendererComponent>().fade      = fade;
        } else {
            ioDeserializedEntity.add<CircleRendererComponent>(color, thickness, fade);
        }
    }
}

// Camera Component
void serializeCameraComponent(YAML::Emitter& out, const CameraComponent& iComponent) {
    out << YAML::Key << "CameraComponent";
    out << YAML::BeginMap;
    {
        out << YAML::Key << "Camera" << YAML::Value;
        out << YAML::BeginMap;
        {
            out << YAML::Key << "ProjectionType" << YAML::Value << (int)iComponent.camera.projectionType();
            out << YAML::Key << "PerspectiveFOV" << YAML::Value << iComponent.camera.perspectiveVerticalFOV();
            out << YAML::Key << "PerspectiveNear" << YAML::Value << iComponent.camera.perspectiveNearClip();
            out << YAML::Key << "PerspectiveFar" << YAML::Value << iComponent.camera.perspectiveFarClip();
            out << YAML::Key << "OrthographicSize" << YAML::Value << iComponent.camera.orthographicSize();
            out << YAML::Key << "OrthographicNear" << YAML::Value << iComponent.camera.orthographicNearClip();
            out << YAML::Key << "OrthographicFar" << YAML::Value << iComponent.camera.orthographicFarClip();
        }
        out << YAML::EndMap;
        out << YAML::Key << "Primary" << YAML::Value << iComponent.primary;
        out << YAML::Key << "FixedAspectRatio" << YAML::Value << iComponent.fixedAspectRatio;
    }
    out << YAML::EndMap;
}

void deserializeCameraComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    auto&& serializedComponent = iSerializedEntity["CameraComponent"];
    if (serializedComponent) {
        auto&& camera           = serializedComponent["Camera"];
        auto&& primary          = serializedComponent["Primary"].as<bool>();
        auto&& fixedAspectRatio = serializedComponent["FixedAspectRatio"].as<bool>();

        if (ioDeserializedEntity.has<CameraComponent>()) {
            ioDeserializedEntity.get<CameraComponent>().camera.setProjectionType((SceneCamera::ProjectionType)camera["ProjectionType"].as<int>());

            ioDeserializedEntity.get<CameraComponent>().camera.setPerspectiveVerticalFOV(camera["PerspectiveFOV"].as<float>());
            ioDeserializedEntity.get<CameraComponent>().camera.setPerspectiveNearClip(camera["PerspectiveNear"].as<float>());
            ioDeserializedEntity.get<CameraComponent>().camera.setPerspectiveFarClip(camera["PerspectiveFar"].as<float>());

            ioDeserializedEntity.get<CameraComponent>().camera.setOrthographicSize(camera["OrthographicSize"].as<float>());
            ioDeserializedEntity.get<CameraComponent>().camera.setOrthographicNearClip(camera["OrthographicNear"].as<float>());
            ioDeserializedEntity.get<CameraComponent>().camera.setOrthographicFarClip(camera["OrthographicFar"].as<float>());

            ioDeserializedEntity.get<CameraComponent>().primary          = primary;
            ioDeserializedEntity.get<CameraComponent>().fixedAspectRatio = fixedAspectRatio;
        } else {
            auto&& cameraComponent = ioDeserializedEntity.add<CameraComponent>();
            cameraComponent.camera.setProjectionType((SceneCamera::ProjectionType)camera["ProjectionType"].as<int>());

            cameraComponent.camera.setPerspectiveVerticalFOV(camera["PerspectiveFOV"].as<float>());
            cameraComponent.camera.setPerspectiveNearClip(camera["PerspectiveNear"].as<float>());
            cameraComponent.camera.setPerspectiveFarClip(camera["PerspectiveFar"].as<float>());

            cameraComponent.camera.setOrthographicSize(camera["OrthographicSize"].as<float>());
            cameraComponent.camera.setOrthographicNearClip(camera["OrthographicNear"].as<float>());
            cameraComponent.camera.setOrthographicFarClip(camera["OrthographicFar"].as<float>());

            cameraComponent.primary          = primary;
            cameraComponent.fixedAspectRatio = fixedAspectRatio;
        }
    }
}

// Native Script Component
void serializeNativeScriptComponent(YAML::Emitter& out, const NativeScriptComponent& iComponent) {
    out << YAML::Key << "NativeScriptComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Script" << YAML::Value << "Script"; // TODO
    out << YAML::EndMap;
}

void deserializeNativeScriptComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {}

// Rigid Body 2D Component
void serializeRigidBody2DComponent(YAML::Emitter& out, const RigidBody2DComponent& iComponent) {
    out << YAML::Key << "RigidBody2DComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "BodyType" << YAML::Value << (int)iComponent.type;
    out << YAML::Key << "FixedRotation" << YAML::Value << iComponent.fixedRotation;
    out << YAML::EndMap;
}

void deserializeRigidBody2DComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    auto&& serializedComponent = iSerializedEntity["RigidBody2DComponent"];
    if (serializedComponent) {
        auto&& bodyType      = serializedComponent["BodyType"].as<int>();
        auto&& fixedRotation = serializedComponent["FixedRotation"].as<bool>();

        if (ioDeserializedEntity.has<RigidBody2DComponent>()) {
            ioDeserializedEntity.get<RigidBody2DComponent>().type          = (RigidBody2DComponent::BodyType)bodyType;
            ioDeserializedEntity.get<RigidBody2DComponent>().fixedRotation = fixedRotation;
        } else {
            auto&& rigidBodyComponent        = ioDeserializedEntity.add<RigidBody2DComponent>();
            rigidBodyComponent.type          = (RigidBody2DComponent::BodyType)bodyType;
            rigidBodyComponent.fixedRotation = fixedRotation;
        }
    }
}

// Box Collider 2D Component
void serializeBoxCollider2DComponent(YAML::Emitter& out, const BoxCollider2DComponent& iComponent) {
    out << YAML::Key << "BoxCollider2DComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Offset" << YAML::Value << iComponent.offset;
    out << YAML::Key << "Size" << YAML::Value << iComponent.size;
    out << YAML::Key << "Density" << YAML::Value << iComponent.density;
    out << YAML::Key << "Friction" << YAML::Value << iComponent.friction;
    out << YAML::Key << "Restitution" << YAML::Value << iComponent.restitution;
    out << YAML::Key << "RestitutionThreshold" << YAML::Value << iComponent.restitutionThreshold;
    out << YAML::EndMap;
}

void deserializeBoxCollider2DComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    auto&& serializedComponent = iSerializedEntity["BoxCollider2DComponent"];
    if (serializedComponent) {
        auto&& offset               = serializedComponent["Offset"].as<glm::vec2>();
        auto&& size                 = serializedComponent["Size"].as<glm::vec2>();
        auto&& density              = serializedComponent["Density"].as<float>();
        auto&& friction             = serializedComponent["Friction"].as<float>();
        auto&& restitution          = serializedComponent["Restitution"].as<float>();
        auto&& restitutionThreshold = serializedComponent["RestitutionThreshold"].as<float>();

        if (ioDeserializedEntity.has<BoxCollider2DComponent>()) {
            ioDeserializedEntity.get<BoxCollider2DComponent>().offset               = offset;
            ioDeserializedEntity.get<BoxCollider2DComponent>().size                 = size;
            ioDeserializedEntity.get<BoxCollider2DComponent>().density              = density;
            ioDeserializedEntity.get<BoxCollider2DComponent>().friction             = friction;
            ioDeserializedEntity.get<BoxCollider2DComponent>().restitution          = restitution;
            ioDeserializedEntity.get<BoxCollider2DComponent>().restitutionThreshold = restitutionThreshold;
        } else {
            auto&& boxColliderComponent               = ioDeserializedEntity.add<BoxCollider2DComponent>();
            boxColliderComponent.offset               = offset;
            boxColliderComponent.size                 = size;
            boxColliderComponent.density              = density;
            boxColliderComponent.friction             = friction;
            boxColliderComponent.restitution          = restitution;
            boxColliderComponent.restitutionThreshold = restitutionThreshold;
        }
    }
}

// Circle Collider 2D Component
void serializeCircleCollider2DComponent(YAML::Emitter& out, const CircleCollider2DComponent& iComponent) {
    out << YAML::Key << "CircleCollider2DComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Offset" << YAML::Value << iComponent.offset;
    out << YAML::Key << "Radius" << YAML::Value << iComponent.radius;
    out << YAML::Key << "Density" << YAML::Value << iComponent.density;
    out << YAML::Key << "Friction" << YAML::Value << iComponent.friction;
    out << YAML::Key << "Restitution" << YAML::Value << iComponent.restitution;
    out << YAML::Key << "RestitutionThreshold" << YAML::Value << iComponent.restitutionThreshold;
    out << YAML::EndMap;
}

void deserializeCircleCollider2DComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    auto&& serializedComponent = iSerializedEntity["CircleCollider2DComponent"];
    if (serializedComponent) {
        auto&& offset               = serializedComponent["Offset"].as<glm::vec2>();
        auto&& radius               = serializedComponent["Radius"].as<float>();
        auto&& density              = serializedComponent["Density"].as<float>();
        auto&& friction             = serializedComponent["Friction"].as<float>();
        auto&& restitution          = serializedComponent["Restitution"].as<float>();
        auto&& restitutionThreshold = serializedComponent["RestitutionThreshold"].as<float>();

        if (ioDeserializedEntity.has<CircleCollider2DComponent>()) {
            ioDeserializedEntity.get<CircleCollider2DComponent>().offset               = offset;
            ioDeserializedEntity.get<CircleCollider2DComponent>().radius               = radius;
            ioDeserializedEntity.get<CircleCollider2DComponent>().density              = density;
            ioDeserializedEntity.get<CircleCollider2DComponent>().friction             = friction;
            ioDeserializedEntity.get<CircleCollider2DComponent>().restitution          = restitution;
            ioDeserializedEntity.get<CircleCollider2DComponent>().restitutionThreshold = restitutionThreshold;
        } else {
            auto&& boxColliderComponent               = ioDeserializedEntity.add<CircleCollider2DComponent>();
            boxColliderComponent.offset               = offset;
            boxColliderComponent.radius               = radius;
            boxColliderComponent.density              = density;
            boxColliderComponent.friction             = friction;
            boxColliderComponent.restitution          = restitution;
            boxColliderComponent.restitutionThreshold = restitutionThreshold;
        }
    }
}

void serializeEntity(YAML::Emitter& out, Entity iEntityToSerialize) {
    PM_CORE_ASSERT(iEntityToSerialize.has<IdComponent>());

    out << YAML::BeginMap;
    {
        out << YAML::Key << "Entity" << YAML::Value << iEntityToSerialize.uuid();
        if (iEntityToSerialize.has<TagComponent>()) {
            serializeTagComponent(out, iEntityToSerialize.get<TagComponent>());
        }
        if (iEntityToSerialize.has<TransformComponent>()) {
            serializeTransformComponent(out, iEntityToSerialize.get<TransformComponent>());
        }
        if (iEntityToSerialize.has<SpriteRendererComponent>()) {
            serializeSpriteRendererComponent(out, iEntityToSerialize.get<SpriteRendererComponent>());
        }
        if (iEntityToSerialize.has<CircleRendererComponent>()) {
            serializeCircleRendererComponent(out, iEntityToSerialize.get<CircleRendererComponent>());
        }
        if (iEntityToSerialize.has<CameraComponent>()) {
            serializeCameraComponent(out, iEntityToSerialize.get<CameraComponent>());
        }
        if (iEntityToSerialize.has<NativeScriptComponent>()) {
            serializeNativeScriptComponent(out, iEntityToSerialize.get<NativeScriptComponent>());
        }
        if (iEntityToSerialize.has<RigidBody2DComponent>()) {
            serializeRigidBody2DComponent(out, iEntityToSerialize.get<RigidBody2DComponent>());
        }
        if (iEntityToSerialize.has<BoxCollider2DComponent>()) {
            serializeBoxCollider2DComponent(out, iEntityToSerialize.get<BoxCollider2DComponent>());
        }
        if (iEntityToSerialize.has<CircleCollider2DComponent>()) {
            serializeCircleCollider2DComponent(out, iEntityToSerialize.get<CircleCollider2DComponent>());
        }
    }
    out << YAML::EndMap;
}
}

void SceneSerializer::serialize(const std::string& iFilepath) {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << _scene->name();
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
    _scene->_registry.each([&](auto&& entity) {
        Entity entityToSerialize = {entity, _scene.get()};
        if (!entityToSerialize) {
            return;
        }

        serializeEntity(out, entityToSerialize);
    });
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream file(iFilepath);
    file << out.c_str();
}

bool SceneSerializer::deserialize(const std::string& iFilepath) {
    YAML::Node data;
    try {
        data = YAML::LoadFile(iFilepath);
    } catch (const YAML::ParserException& e) {
        PM_CORE_ERROR("Failed to deserialize scene '{0}'\n\t{1}", iFilepath, e.what());
        return false;
    }

    if (!data["Scene"]) {
        return false;
    }

    auto&& sceneName = data["Scene"].as<std::string>();
    PM_CORE_TRACE("Deserializing scene '{0}'", sceneName);

    auto&& entities = data["Entities"];
    if (entities) {
        for (auto&& entity : entities) {
            auto&& uuid = entity["Entity"].as<uint64_t>();

            Entity deserializedEntity = _scene->createEntityWithUUID(uuid);

            deserializeTagComponent(entity, deserializedEntity);
            deserializeTransformComponent(entity, deserializedEntity);
            deserializeSpriteRendererComponent(entity, deserializedEntity);
            deserializeCircleRendererComponent(entity, deserializedEntity);
            deserializeCameraComponent(entity, deserializedEntity);
            deserializeNativeScriptComponent(entity, deserializedEntity);
            deserializeRigidBody2DComponent(entity, deserializedEntity);
            deserializeBoxCollider2DComponent(entity, deserializedEntity);
            deserializeCircleCollider2DComponent(entity, deserializedEntity);

            PM_CORE_TRACE("\tDeserialized Entity '{0}' (id: {1})", deserializedEntity.tag(), uuid);
        }
    }

    return true;
}

void SceneSerializer::serializeRuntime(const std::string& iFilepath) {
    // Not implemented
    PM_CORE_ASSERT(false);
}

bool SceneSerializer::deserializeRuntime(const std::string& iFilepath) {
    // Not implemented
    PM_CORE_ASSERT(false);
    return false;
}
}
