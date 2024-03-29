#include "pmpch.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "PepperMint/Scene/Components.h"
#include "PepperMint/Scene/Entity.h"
#include "PepperMint/Scene/SceneSerializer.h"
#include "PepperMint/Scripting/ScriptEngine.h"

#define READ_COMPONENT(node, element, type) \
    if (node) {                             \
        element = node.as<type>();          \
    }

#define READ_COMPONENT_ENUM(node, element, type, cast) \
    if (node) {                                        \
        element = (cast)node.as<type>();               \
    }

#define READ_SCRIPT_FIELD(fieldType, type)                      \
    case ScriptFieldType::fieldType: {                          \
        fieldInstance.setValue(scriptField["Data"].as<type>()); \
        break;                                                  \
    }

#define WRITE_SCRIPT_FIELD(fieldType, type) \
    case ScriptFieldType::fieldType: {      \
        out << scriptField.value<type>();   \
        break;                              \
    }

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

template <>
struct convert<PepperMint::UUID> {
    static Node encode(const PepperMint::UUID& rhs) {
        Node node;
        node.push_back((uint64_t)rhs);
        return node;
    }

    static bool decode(const Node& node, PepperMint::UUID& rhs) {
        rhs = node.as<uint64_t>();
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
}

namespace {

// Tag Component
void serializeTagComponent(YAML::Emitter& out, const TagComponent& iComponent) {
    out << YAML::Key << "TagComponent";
    out << YAML::BeginMap;
    { out << YAML::Key << "Tag" << YAML::Value << iComponent.tag; }
    out << YAML::EndMap;
}

void deserializeTagComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    if (auto&& serializedComponent = iSerializedEntity["TagComponent"]) {
        auto&& tagComponent = ioDeserializedEntity.addOrReplace<TagComponent>();

        READ_COMPONENT(serializedComponent["Tag"], tagComponent.tag, std::string);
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
    if (auto&& serializedComponent = iSerializedEntity["TransformComponent"]) {
        auto&& transformComponent = ioDeserializedEntity.addOrReplace<TransformComponent>();

        READ_COMPONENT(serializedComponent["Translation"], transformComponent.translation, glm::vec3);
        READ_COMPONENT(serializedComponent["Rotation"], transformComponent.rotation, glm::vec3);
        READ_COMPONENT(serializedComponent["Scale"], transformComponent.scale, glm::vec3);
    }
}

// Sprite Renderer Component
void serializeSpriteRendererComponent(YAML::Emitter& out, const SpriteRendererComponent& iComponent) {
    out << YAML::Key << "SpriteRendererComponent";
    out << YAML::BeginMap;
    {
        out << YAML::Key << "Color" << YAML::Value << iComponent.color;
        if (iComponent.texture) {
            out << YAML::Key << "TexturePath" << YAML::Value << iComponent.texture->path();
        }
        out << YAML::Key << "TilingFactor" << YAML::Value << iComponent.tilingFactor;
    }
    out << YAML::EndMap;
}

void deserializeSpriteRendererComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    if (auto&& serializedComponent = iSerializedEntity["SpriteRendererComponent"]) {
        auto&& spriteRendererComponent = ioDeserializedEntity.addOrReplace<SpriteRendererComponent>();

        READ_COMPONENT(serializedComponent["Color"], spriteRendererComponent.color, glm::vec4);
        READ_COMPONENT(serializedComponent["TilingFactor"], spriteRendererComponent.tilingFactor, float);

        if (serializedComponent["TexturePath"]) {
            spriteRendererComponent.texture = Texture2D::Create(serializedComponent["TexturePath"].as<std::string>());
        }
    }
}

// Circle Renderer Component
void serializeCircleRendererComponent(YAML::Emitter& out, const CircleRendererComponent& iComponent) {
    out << YAML::Key << "CircleRendererComponent";
    out << YAML::BeginMap;
    {
        out << YAML::Key << "Color" << YAML::Value << iComponent.color;
        out << YAML::Key << "Thickness" << YAML::Value << iComponent.thickness;
        out << YAML::Key << "Fade" << YAML::Value << iComponent.fade;
    }
    out << YAML::EndMap;
}

void deserializeCircleRendererComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    if (auto&& serializedComponent = iSerializedEntity["CircleRendererComponent"]) {
        auto&& circleRendererComponent = ioDeserializedEntity.addOrReplace<CircleRendererComponent>();

        READ_COMPONENT(serializedComponent["Color"], circleRendererComponent.color, glm::vec4);
        READ_COMPONENT(serializedComponent["Thickness"], circleRendererComponent.thickness, float);
        READ_COMPONENT(serializedComponent["Fade"], circleRendererComponent.fade, float);
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
    if (auto&& serializedComponent = iSerializedEntity["CameraComponent"]) {
        auto&& cameraComponent = ioDeserializedEntity.addOrReplace<CameraComponent>();

        READ_COMPONENT(serializedComponent["Primary"], cameraComponent.primary, bool);
        READ_COMPONENT(serializedComponent["FixedAspectRatio"], cameraComponent.fixedAspectRatio, bool);

        if (auto&& camera = serializedComponent["Camera"]) {
            cameraComponent.camera.setProjectionType((SceneCamera::ProjectionType)camera["ProjectionType"].as<int>());

            cameraComponent.camera.setPerspectiveVerticalFOV(camera["PerspectiveFOV"].as<float>());
            cameraComponent.camera.setPerspectiveNearClip(camera["PerspectiveNear"].as<float>());
            cameraComponent.camera.setPerspectiveFarClip(camera["PerspectiveFar"].as<float>());

            cameraComponent.camera.setOrthographicSize(camera["OrthographicSize"].as<float>());
            cameraComponent.camera.setOrthographicNearClip(camera["OrthographicNear"].as<float>());
            cameraComponent.camera.setOrthographicFarClip(camera["OrthographicFar"].as<float>());
        }
    }
}

// Script Component
void serializeScriptComponent(YAML::Emitter& out, const ScriptComponent& iComponent, const Entity& iEntity) {
    out << YAML::Key << "ScriptComponent";
    out << YAML::BeginMap;
    {
        out << YAML::Key << "ClassName" << YAML::Value << iComponent.className;

        // Fields
        auto&& entityClass = ScriptEngine::GetEntityClass(iComponent.className);
        auto&& fields      = entityClass->fields();
        if (!fields.empty()) {
            out << YAML::Key << "ScriptFields" << YAML::Value;

            auto&& entityFields = ScriptEngine::GetScriptFieldMap(iEntity);
            out << YAML::BeginSeq;
            {
                for (auto&& [name, field] : fields) {
                    if (entityFields.find(name) == entityFields.end()) {
                        continue;
                    }

                    auto&& scriptField = entityFields.at(name);

                    out << YAML::BeginMap;
                    {
                        out << YAML::Key << "Name" << YAML::Value << name;
                        out << YAML::Key << "Type" << YAML::Value << Utils::scriptFieldTypeToString(field.type);
                        out << YAML::Key << "Data" << YAML::Value;
                        switch (field.type) {
                            WRITE_SCRIPT_FIELD(FLOAT, float);
                            WRITE_SCRIPT_FIELD(DOUBLE, double);
                            WRITE_SCRIPT_FIELD(BOOL, bool);
                            WRITE_SCRIPT_FIELD(CHAR, char);
                            WRITE_SCRIPT_FIELD(BYTE, int8_t);
                            WRITE_SCRIPT_FIELD(SHORT, int16_t);
                            WRITE_SCRIPT_FIELD(INT, int32_t);
                            WRITE_SCRIPT_FIELD(LONG, int64_t);
                            WRITE_SCRIPT_FIELD(USHORT, uint16_t);
                            WRITE_SCRIPT_FIELD(UINT, uint32_t);
                            WRITE_SCRIPT_FIELD(ULONG, uint64_t);
                            WRITE_SCRIPT_FIELD(VECTOR2, glm::vec2);
                            WRITE_SCRIPT_FIELD(VECTOR3, glm::vec3);
                            WRITE_SCRIPT_FIELD(VECTOR4, glm::vec4);
                            WRITE_SCRIPT_FIELD(ENTITY, UUID);
                        }
                    }
                    out << YAML::EndMap;
                }
            }
            out << YAML::EndSeq;
        }
    }
    out << YAML::EndMap;
}

void deserializeScriptComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    if (auto&& serializedComponent = iSerializedEntity["ScriptComponent"]) {
        auto&& scriptComponent = ioDeserializedEntity.addOrReplace<ScriptComponent>();

        READ_COMPONENT(serializedComponent["ClassName"], scriptComponent.className, std::string);

        auto&& scriptFields = serializedComponent["ScriptFields"];
        if (scriptFields) {
            auto&& entityClass = ScriptEngine::GetEntityClass(scriptComponent.className);
            PM_CORE_ASSERT(entityClass);
            auto&& fields       = entityClass->fields();
            auto&& entityFields = ScriptEngine::GetScriptFieldMap(ioDeserializedEntity);

            for (auto&& scriptField : scriptFields) {
                std::string     name;
                ScriptFieldType type;

                if (scriptField["Name"]) {
                    name = scriptField["Name"].as<std::string>();
                }
                if (scriptField["Type"]) {
                    type = Utils::scriptFieldTypeFromString(scriptField["Type"].as<std::string>());
                }

                auto&& fieldInstance = entityFields[name];
                PM_CORE_ASSERT(fields.find(name) != fields.end());
                fieldInstance.field = fields.at(name);

                switch (type) {
                    READ_SCRIPT_FIELD(FLOAT, float);
                    READ_SCRIPT_FIELD(DOUBLE, double);
                    READ_SCRIPT_FIELD(BOOL, bool);
                    READ_SCRIPT_FIELD(CHAR, char);
                    READ_SCRIPT_FIELD(BYTE, int8_t);
                    READ_SCRIPT_FIELD(SHORT, int16_t);
                    READ_SCRIPT_FIELD(INT, int32_t);
                    READ_SCRIPT_FIELD(LONG, int64_t);
                    READ_SCRIPT_FIELD(USHORT, uint16_t);
                    READ_SCRIPT_FIELD(UINT, uint32_t);
                    READ_SCRIPT_FIELD(ULONG, uint64_t);
                    READ_SCRIPT_FIELD(VECTOR2, glm::vec2);
                    READ_SCRIPT_FIELD(VECTOR3, glm::vec3);
                    READ_SCRIPT_FIELD(VECTOR4, glm::vec4);
                    READ_SCRIPT_FIELD(ENTITY, UUID);
                }
            }
        }
    }
}

// Rigid Body 2D Component
void serializeRigidBody2DComponent(YAML::Emitter& out, const RigidBody2DComponent& iComponent) {
    out << YAML::Key << "RigidBody2DComponent";
    out << YAML::BeginMap;
    {
        out << YAML::Key << "BodyType" << YAML::Value << (int)iComponent.type;
        out << YAML::Key << "FixedRotation" << YAML::Value << iComponent.fixedRotation;
    }
    out << YAML::EndMap;
}

void deserializeRigidBody2DComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    if (auto&& serializedComponent = iSerializedEntity["RigidBody2DComponent"]) {
        auto&& rigidBody2DComponent = ioDeserializedEntity.addOrReplace<RigidBody2DComponent>();

        READ_COMPONENT_ENUM(serializedComponent["BodyType"], rigidBody2DComponent.type, int, RigidBody2DComponent::BodyType);
        READ_COMPONENT(serializedComponent["FixedRotation"], rigidBody2DComponent.fixedRotation, bool);
    }
}

// Box Collider 2D Component
void serializeBoxCollider2DComponent(YAML::Emitter& out, const BoxCollider2DComponent& iComponent) {
    out << YAML::Key << "BoxCollider2DComponent";
    out << YAML::BeginMap;
    {
        out << YAML::Key << "Offset" << YAML::Value << iComponent.offset;
        out << YAML::Key << "Size" << YAML::Value << iComponent.size;
        out << YAML::Key << "Density" << YAML::Value << iComponent.density;
        out << YAML::Key << "Friction" << YAML::Value << iComponent.friction;
        out << YAML::Key << "Restitution" << YAML::Value << iComponent.restitution;
        out << YAML::Key << "RestitutionThreshold" << YAML::Value << iComponent.restitutionThreshold;
    }
    out << YAML::EndMap;
}

void deserializeBoxCollider2DComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    if (auto&& serializedComponent = iSerializedEntity["BoxCollider2DComponent"]) {
        auto&& boxCollider2DComponent = ioDeserializedEntity.addOrReplace<BoxCollider2DComponent>();

        READ_COMPONENT(serializedComponent["Offset"], boxCollider2DComponent.offset, glm::vec2);
        READ_COMPONENT(serializedComponent["Size"], boxCollider2DComponent.size, glm::vec2);
        READ_COMPONENT(serializedComponent["Density"], boxCollider2DComponent.density, float);
        READ_COMPONENT(serializedComponent["Friction"], boxCollider2DComponent.friction, float);
        READ_COMPONENT(serializedComponent["Restitution"], boxCollider2DComponent.restitution, float);
        READ_COMPONENT(serializedComponent["RestitutionThreshold"], boxCollider2DComponent.restitutionThreshold, float);
    }
}

// Circle Collider 2D Component
void serializeCircleCollider2DComponent(YAML::Emitter& out, const CircleCollider2DComponent& iComponent) {
    out << YAML::Key << "CircleCollider2DComponent";
    out << YAML::BeginMap;
    {
        out << YAML::Key << "Offset" << YAML::Value << iComponent.offset;
        out << YAML::Key << "Radius" << YAML::Value << iComponent.radius;
        out << YAML::Key << "Density" << YAML::Value << iComponent.density;
        out << YAML::Key << "Friction" << YAML::Value << iComponent.friction;
        out << YAML::Key << "Restitution" << YAML::Value << iComponent.restitution;
        out << YAML::Key << "RestitutionThreshold" << YAML::Value << iComponent.restitutionThreshold;
    }
    out << YAML::EndMap;
}

void deserializeCircleCollider2DComponent(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
    if (auto&& serializedComponent = iSerializedEntity["CircleCollider2DComponent"]) {
        auto&& circleCollider2DComponent = ioDeserializedEntity.addOrReplace<CircleCollider2DComponent>();

        READ_COMPONENT(serializedComponent["Offset"], circleCollider2DComponent.offset, glm::vec2);
        READ_COMPONENT(serializedComponent["Radius"], circleCollider2DComponent.radius, float);
        READ_COMPONENT(serializedComponent["Density"], circleCollider2DComponent.density, float);
        READ_COMPONENT(serializedComponent["Friction"], circleCollider2DComponent.friction, float);
        READ_COMPONENT(serializedComponent["Restitution"], circleCollider2DComponent.restitution, float);
        READ_COMPONENT(serializedComponent["RestitutionThreshold"], circleCollider2DComponent.restitutionThreshold, float);
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
        if (iEntityToSerialize.has<ScriptComponent>()) {
            serializeScriptComponent(out, iEntityToSerialize.get<ScriptComponent>(), iEntityToSerialize);
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
    PM_CORE_TRACE("Serializing scene '{0}'", _scene->name());

    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << _scene->name();
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
    _scene->forEachEntity([&](auto&& entity) {
        Entity entityToSerialize = {entity, _scene.get()};
        if (!entityToSerialize) {
            return;
        }

        serializeEntity(out, entityToSerialize);

        PM_CORE_TRACE("\tSerialized Entity '{0}' (id: {1})", entityToSerialize.tag(), entityToSerialize.uuid());
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
            deserializeScriptComponent(entity, deserializedEntity);
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
