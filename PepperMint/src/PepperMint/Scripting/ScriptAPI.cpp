#include "pmpch.h"

#include <box2d/b2_body.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

#include "PepperMint/Core/Input.h"
#include "PepperMint/Core/KeyCodes.h"
#include "PepperMint/Core/UUID.h"
#include "PepperMint/Scene/Components.h"
#include "PepperMint/Scene/Entity.h"
#include "PepperMint/Scripting/ScriptAPI.h"
#include "PepperMint/Scripting/ScriptEngine.h"

#define PM_ADD_INTERNAL_CALL(method) mono_add_internal_call("PepperMint.InternalCalls::" #method, method)

namespace PepperMint {

namespace {

const std::string kNAMESPACE = "PepperMint";

// Store the has functions for entities
static std::unordered_map<MonoType*, std::function<bool(Entity)>> sEntityHasComponentFunctions;

template <typename... Component>
void registerComponent() {
    (
        []() {
            std::string_view typeName        = typeid(Component).name();
            size_t           pos             = typeName.find_last_of(':');
            std::string_view structName      = typeName.substr(pos + 1);
            auto&&           managedTypeName = fmt::format("{}.{}", kNAMESPACE, structName);

            auto&& managedType = mono_reflection_type_from_name(managedTypeName.data(), ScriptEngine::GetCoreAssemblyImage());
            if (!managedType) {
                PM_CORE_ERROR("Could not find component type {}", managedTypeName);
                return;
            }
            sEntityHasComponentFunctions[managedType] = [](Entity entity) { return entity.has<Component>(); };
        }(),
        ...);
}

template <typename... Component>
void registerComponent(ComponentGroup<Component...>) {
    registerComponent<Component...>();
}

#pragma region Log

void Log_MessageInt(MonoString* iMessage, int iValue) {
    auto&&      cstr = mono_string_to_utf8(iMessage);
    std::string str(cstr);
    mono_free(cstr);
    PM_CORE_TRACE("{0}, {1}", str, iValue);
}

void Log_Vector(glm::vec3* iVector, glm::vec3* oResult) {
    PM_CORE_TRACE("Vector: {0}", *iVector);
    *oResult = glm::normalize(*iVector);
}

float Log_VectorDot(glm::vec3* iVector) {
    PM_CORE_TRACE("Vector: {0}", *iVector);
    return glm::dot(*iVector, *iVector);
}

#pragma endregion

#pragma region Entity

bool Entity_HasComponent(UUID iEntityUUID, MonoReflectionType* iComponentType) {
    auto&& scene = ScriptEngine::GetSceneContext();
    PM_CORE_ASSERT(scene);
    auto&& entity = scene->getEntityByUUID(iEntityUUID);
    PM_CORE_ASSERT(entity);

    auto&& managedType = mono_reflection_type_get_type(iComponentType);
    PM_CORE_ASSERT(sEntityHasComponentFunctions.find(managedType) != sEntityHasComponentFunctions.end());

    return sEntityHasComponentFunctions.at(managedType)(entity);
}

#pragma endregion

#pragma region TransformComponent

void TransformComponent_GetTranslation(UUID iEntityUUID, glm::vec3* oTranslation) {
    auto&& scene = ScriptEngine::GetSceneContext();
    PM_CORE_ASSERT(scene);
    auto&& entity = scene->getEntityByUUID(iEntityUUID);
    PM_CORE_ASSERT(entity);

    *oTranslation = entity.get<TransformComponent>().translation;
}

void TransformComponent_SetTranslation(UUID iEntityUUID, glm::vec3* iTranslation) {
    auto&& scene = ScriptEngine::GetSceneContext();
    PM_CORE_ASSERT(scene);
    auto&& entity = scene->getEntityByUUID(iEntityUUID);
    PM_CORE_ASSERT(entity);

    entity.get<TransformComponent>().translation = *iTranslation;
}

#pragma endregion

#pragma region RigidBody2DComponent

void RigidBody2DComponent_ApplyLinearImpulse(UUID iEntityUUID, glm::vec2* iImpulse, glm::vec2* iPoint, bool iWake) {
    auto&& scene = ScriptEngine::GetSceneContext();
    PM_CORE_ASSERT(scene);
    auto&& entity = scene->getEntityByUUID(iEntityUUID);
    PM_CORE_ASSERT(entity);

    auto&& body = (b2Body*)entity.get<RigidBody2DComponent>().runtimeBody;
    body->ApplyLinearImpulse(b2Vec2(iImpulse->x, iImpulse->y), b2Vec2(iPoint->x, iPoint->y), iWake);
}

void RigidBody2DComponent_ApplyLinearImpulseToCenter(UUID iEntityUUID, glm::vec2* iImpulse, bool iWake) {
    auto&& scene = ScriptEngine::GetSceneContext();
    PM_CORE_ASSERT(scene);
    auto&& entity = scene->getEntityByUUID(iEntityUUID);
    PM_CORE_ASSERT(entity);

    auto&& body = (b2Body*)entity.get<RigidBody2DComponent>().runtimeBody;
    body->ApplyLinearImpulseToCenter(b2Vec2(iImpulse->x, iImpulse->y), iWake);
}

#pragma endregion

#pragma region Input

bool Input_IsKeyDown(KeyCode iKeyCode) { return Input::IsKeyPressed(iKeyCode); }

#pragma endregion
}

void ScriptAPI::RegisterComponents() { registerComponent(AllComponents{}); }

void ScriptAPI::RegisterFunctions() {
    PM_ADD_INTERNAL_CALL(Log_MessageInt);
    PM_ADD_INTERNAL_CALL(Log_Vector);
    PM_ADD_INTERNAL_CALL(Log_VectorDot);

    PM_ADD_INTERNAL_CALL(Entity_HasComponent);

    PM_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
    PM_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

    PM_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulse);
    PM_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulseToCenter);

    PM_ADD_INTERNAL_CALL(Input_IsKeyDown);
}
}
