#include "pmpch.h"

#include "PepperMint/Scene/Components.h"

namespace YAML {

template <>
struct convert<glm::vec3> {
    static Node encode(const glm::vec3& rhs) {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
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

// Tag Component
void TagComponent::serialize(YAML::Emitter& out) const {
    out << YAML::Key << "TagComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Tag" << YAML::Value << tag;
    out << YAML::EndMap;
}

void TagComponent::Deserialize(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
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
glm::mat4 TransformComponent::transform() const {
    auto&& translate = glm::translate(glm::mat4(1.0f), translation);
    auto&& rotate    = glm::rotate(glm::mat4(1.0f), rotation.x, {1, 0, 0}) * glm::rotate(glm::mat4(1.0f), rotation.y, {0, 1, 0}) *
                    glm::rotate(glm::mat4(1.0f), rotation.z, {0, 0, 1});
    auto&& rescale = glm::scale(glm::mat4(1.0f), scale);

    return translate * rotate * rescale;
}

void TransformComponent::serialize(YAML::Emitter& out) const {
    out << YAML::Key << "TransformComponent";
    out << YAML::BeginMap;
    {
        out << YAML::Key << "Translation" << YAML::Value << translation;
        out << YAML::Key << "Rotation" << YAML::Value << rotation;
        out << YAML::Key << "Scale" << YAML::Value << scale;
    }
    out << YAML::EndMap;
}

void TransformComponent::Deserialize(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
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
void SpriteRendererComponent::serialize(YAML::Emitter& out) const {
    out << YAML::Key << "SpriteRendererComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Color" << YAML::Value << color;
    out << YAML::EndMap;
}

void SpriteRendererComponent::Deserialize(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
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

// Camera Component
void CameraComponent::serialize(YAML::Emitter& out) const {
    out << YAML::Key << "CameraComponent";
    out << YAML::BeginMap;
    {
        out << YAML::Key << "Camera" << YAML::Value;
        out << YAML::BeginMap;
        {
            out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.projectionType();
            out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.perspectiveVerticalFOV();
            out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.perspectiveNearClip();
            out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.perspectiveFarClip();
            out << YAML::Key << "OrthographicSize" << YAML::Value << camera.orthographicSize();
            out << YAML::Key << "OrthographicNear" << YAML::Value << camera.orthographicNearClip();
            out << YAML::Key << "OrthographicFar" << YAML::Value << camera.orthographicFarClip();
        }
        out << YAML::EndMap;
        out << YAML::Key << "Primary" << YAML::Value << primary;
        out << YAML::Key << "FixedAspectRatio" << YAML::Value << fixedAspectRatio;
    }
    out << YAML::EndMap;
}

void CameraComponent::Deserialize(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {
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
void NativeScriptComponent::serialize(YAML::Emitter& out) const {
    out << YAML::Key << "NativeScriptComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Script" << YAML::Value << "Script"; // TODO
    out << YAML::EndMap;
}

void NativeScriptComponent::Deserialize(const YAML::Node& iSerializedEntity, Entity ioDeserializedEntity) {}
}
