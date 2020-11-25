#include "pmpch.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "PepperMint/Scene/Components.h"
#include "PepperMint/Scene/Entity.h"
#include "PepperMint/Scene/SceneSerializer.h"

namespace PepperMint {

namespace {

template <typename Component>
void serializeComponent(YAML::Emitter& out, Entity iEntityToSerialize) {
    if (iEntityToSerialize.has<Component>()) {
        iEntityToSerialize.get<Component>().serialize(out);
    }
}

void serializeEntity(YAML::Emitter& out, Entity iEntityToSerialize) {
    out << YAML::BeginMap;
    {
        out << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // TODO: Entity ID goes here
        serializeComponent<TagComponent>(out, iEntityToSerialize);
        serializeComponent<TransformComponent>(out, iEntityToSerialize);
        serializeComponent<SpriteRendererComponent>(out, iEntityToSerialize);
        serializeComponent<CameraComponent>(out, iEntityToSerialize);
        serializeComponent<NativeScriptComponent>(out, iEntityToSerialize);
    }
    out << YAML::EndMap;
}
}

void SceneSerializer::serialize(const std::string& iFilepath) {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << "Untitled"; // TODO
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
    std::ifstream     file(iFilepath);
    std::stringstream ss;
    ss << file.rdbuf();

    YAML::Node data = YAML::Load(ss.str());
    if (!data["Scene"]) {
        return false;
    }

    auto&& sceneName = data["Scene"].as<std::string>();
    PM_CORE_TRACE("Deserializing scene '{0}'", sceneName);

    auto&& entities = data["Entities"];
    if (entities) {
        for (auto&& entity : entities) {
            Entity deserializedEntity = _scene->createEntity();

            auto&& uuid = entity["Entity"].as<uint64_t>(); // TODO

            TagComponent::Deserialize(entity, deserializedEntity);
            TransformComponent::Deserialize(entity, deserializedEntity);
            SpriteRendererComponent::Deserialize(entity, deserializedEntity);
            CameraComponent::Deserialize(entity, deserializedEntity);
            NativeScriptComponent::Deserialize(entity, deserializedEntity);

            PM_CORE_TRACE("\tDeserialized Entity '{0}' (id: {1})", deserializedEntity.get<TagComponent>().tag, uuid);
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
