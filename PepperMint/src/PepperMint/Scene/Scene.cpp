#include "pmpch.h"

#include "PepperMint/Renderer/Renderer2D.h"
#include "PepperMint/Scene/Components.h"
#include "PepperMint/Scene/Entity.h"
#include "PepperMint/Scene/Scene.h"
#include "PepperMint/Scene/ScriptableEntity.h"

// Box2D
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_world.h>

namespace PepperMint {

namespace {

b2BodyType rigidBody2DTypeToBox2DBodyType(RigidBody2DComponent::BodyType type) {
    switch (type) {
        case RigidBody2DComponent::BodyType::STATIC:
            return b2_staticBody;
        case RigidBody2DComponent::BodyType::DYNAMIC:
            return b2_dynamicBody;
        case RigidBody2DComponent::BodyType::KINEMATIC:
            return b2_kinematicBody;
    }

    PM_CORE_ASSERT(false, "Unknown body type");
    return b2_staticBody;
}

template <typename... Component>
void copyComponent(entt::registry& oDestination, const entt::registry& iSource, const std::unordered_map<UUID, entt::entity>& iEnttMap) {
    (
        [&]() {
            auto&& allView = iSource.view<Component>();
            for (auto&& sourceEntity : allView) {
                auto&& sourceComponent   = iSource.get<Component>(sourceEntity);
                auto&& destinationEntity = iEnttMap.at(iSource.get<IdComponent>(sourceEntity).uuid);
                oDestination.emplace_or_replace<Component>(destinationEntity, sourceComponent);
            }
        }(),
        ...);
}

template <typename... Component>
void copyComponent(ComponentGroup<Component...>,
                   entt::registry&                               oDestination,
                   const entt::registry&                         iSource,
                   const std::unordered_map<UUID, entt::entity>& iEnttMap) {
    copyComponent<Component...>(oDestination, iSource, iEnttMap);
}

template <typename... Component>
void copyComponentIfExists(Entity oDestination, Entity iSource) {
    (
        [&]() {
            if (iSource.has<Component>()) {
                oDestination.addOrReplace<Component>(iSource.get<Component>());
            }
        }(),
        ...);
}

template <typename... Component>
void copyComponentIfExists(ComponentGroup<Component...>, Entity oDestination, Entity iSource) {
    copyComponentIfExists<Component...>(oDestination, iSource);
}
}

Ref<Scene> Scene::Copy(const Ref<Scene>& iOther) {
    Ref<Scene> newScene = CreateRef<Scene>();

    newScene->_viewportWidth  = iOther->_viewportWidth;
    newScene->_viewportHeight = iOther->_viewportHeight;

    std::unordered_map<UUID, entt::entity> enttMap;

    // Create entities in new scene
    auto&& sourceSceneRegistry = iOther->_registry;
    auto&& destSceneRegistry   = newScene->_registry;

    auto&& idView = sourceSceneRegistry.view<IdComponent>();
    for (auto&& entity : idView) {
        auto&& uuid = sourceSceneRegistry.get<IdComponent>(entity).uuid;
        auto&& tag  = sourceSceneRegistry.get<TagComponent>(entity).tag;

        // PM_CORE_TRACE("Entity copied {0} (id {1})", tag, uuid);

        Entity newEntity = newScene->createEntityWithUUID(uuid, tag);
        enttMap.emplace(uuid, newEntity);
    }

    // Copy components (except Id and Tag components)
    copyComponent(AllComponents{}, destSceneRegistry, sourceSceneRegistry, enttMap);

    return newScene;
}

Entity Scene::createEntity(const std::string& iName) { return createEntityWithUUID(UUID(), iName); }

Entity Scene::createEntityWithUUID(const UUID& iUUID, const std::string& iName) {
    Entity entity(_registry.create(), this);
    entity.add<IdComponent>(iUUID);
    entity.add<TagComponent>(iName);
    entity.add<TransformComponent>();
    return entity;
}

void Scene::destroyEntity(Entity iEntity) { _registry.destroy(iEntity); }

void Scene::onRuntimeStart() {
    _physicsWorld = new b2World({0.0f, -9.8f});

    auto&& physicsView = _registry.view<RigidBody2DComponent>();
    for (auto&& entity : physicsView) {
        Entity tmp       = {entity, this};
        auto&& transform = tmp.get<TransformComponent>();
        auto&& rigidBody = tmp.get<RigidBody2DComponent>();

        b2BodyDef bodyDef;
        bodyDef.type = rigidBody2DTypeToBox2DBodyType(rigidBody.type);
        bodyDef.position.Set(transform.translation.x, transform.translation.y);
        bodyDef.angle = transform.rotation.z;

        b2Body* body = _physicsWorld->CreateBody(&bodyDef);
        body->SetFixedRotation(rigidBody.fixedRotation);
        rigidBody.runtimeBody = body;

        if (tmp.has<BoxCollider2DComponent>()) {
            auto&& boxCollider = tmp.get<BoxCollider2DComponent>();

            b2PolygonShape boxShape;
            boxShape.SetAsBox(boxCollider.size.x * transform.scale.x, boxCollider.size.y * transform.scale.y);

            b2FixtureDef fixtureDef;
            fixtureDef.shape                = &boxShape;
            fixtureDef.density              = boxCollider.density;
            fixtureDef.friction             = boxCollider.friction;
            fixtureDef.restitution          = boxCollider.restitution;
            fixtureDef.restitutionThreshold = boxCollider.restitutionThreshold;
            body->CreateFixture(&fixtureDef);
        }
    }
}

void Scene::onRuntimeStop() {
    delete _physicsWorld;
    _physicsWorld = nullptr;
}

void Scene::onUpdateRuntime(Timestep iTimestep) {
    // Update scripts
    auto&& scriptView = _registry.view<NativeScriptComponent>();
    for (auto&& entity : scriptView) {
        auto&& scriptComponent = scriptView.get<NativeScriptComponent>(entity);

        // TODO: Move to Scene onScenePlay
        if (!scriptComponent.script) {
            scriptComponent.script = scriptComponent.instantiateScript();
            PM_CORE_ASSERT(scriptComponent.script, "Error while instantiating script!");
            scriptComponent.script->_entity = Entity(entity, this);
            scriptComponent.script->onCreate();
        }

        scriptComponent.script->onUpdate(iTimestep);
    }

    // Physics
    if (_physicsWorld) {
        const int32_t velocityIterations = 6;
        const int32_t positionIterations = 2;
        _physicsWorld->Step(iTimestep, velocityIterations, positionIterations);

        auto&& physicsView = _registry.view<RigidBody2DComponent>();
        for (auto&& entity : physicsView) {
            Entity tmp       = {entity, this};
            auto&& transform = tmp.get<TransformComponent>();

            auto&& body     = (b2Body*)tmp.get<RigidBody2DComponent>().runtimeBody;
            auto&& position = body->GetPosition();
            auto&& rotation = body->GetAngle();

            transform.translation.x = position.x;
            transform.translation.y = position.y;
            transform.rotation.z    = rotation;
        }
    }

    // Render 2D
    Camera*   mainCamera = nullptr;
    glm::mat4 cameraTransform;

    auto&& renderView = _registry.view<TransformComponent, CameraComponent>();
    for (auto&& entity : renderView) {
        auto&& [transformComponent, cameraComponent] = renderView.get<TransformComponent, CameraComponent>(entity);

        if (cameraComponent.primary) {
            mainCamera      = &cameraComponent.camera;
            cameraTransform = transformComponent.transform();
            break;
        }
    }

    if (mainCamera) {
        Renderer2D::BeginScene(*mainCamera, cameraTransform);
        {
            // Draw Sprites
            {
                auto&& group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
                for (auto&& entity : group) {
                    auto&& [transformComponent, spriteComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                    Renderer2D::DrawSprite(transformComponent, spriteComponent, (int)entity);
                }
            }

            // Draw Circles
            {
                auto&& view = _registry.view<TransformComponent, CircleRendererComponent>();
                for (auto&& entity : view) {
                    auto&& [transformComponent, circleComponent] = view.get<TransformComponent, CircleRendererComponent>(entity);
                    Renderer2D::DrawCircle(transformComponent, circleComponent, (int)entity);
                }
            }
        }
        Renderer2D::EndScene();
    }
}

void Scene::onUpdateEditor(Timestep iTimestep, EditorCamera& iCamera) {
    Renderer2D::BeginScene(iCamera);
    {
        // Draw Sprites
        {
            auto&& group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto&& entity : group) {
                auto&& [transformComponent, spriteComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::DrawSprite(transformComponent, spriteComponent, (int)entity);
            }
        }

        // Draw Circles
        {
            auto&& view = _registry.view<TransformComponent, CircleRendererComponent>();
            for (auto&& entity : view) {
                auto&& [transformComponent, circleComponent] = view.get<TransformComponent, CircleRendererComponent>(entity);
                Renderer2D::DrawCircle(transformComponent, circleComponent, (int)entity);
            }
        }
    }
    Renderer2D::EndScene();
}

void Scene::onViewportResize(uint32_t iWidth, uint32_t iHeight) {
    _viewportWidth  = iWidth;
    _viewportHeight = iHeight;

    // Resize all non-fixedAspectRatio cameras
    auto&& view = _registry.view<CameraComponent>();
    for (auto&& entity : view) {
        auto&& cameraComponent = view.get<CameraComponent>(entity);
        if (!cameraComponent.fixedAspectRatio) {
            cameraComponent.camera.setViewportSize(iWidth, iHeight);
        }
    }
}

Entity Scene::primaryCameraEntity() {
    auto&& view = _registry.view<CameraComponent>();
    for (auto&& entity : view) {
        auto&& cameraComponent = view.get<CameraComponent>(entity);
        if (cameraComponent.primary) {
            return Entity(entity, this);
        }
    }
    return {};
}

void Scene::duplicateEntity(Entity iEntity) {
    Entity newEntity = createEntity(iEntity.tag());

    copyComponentIfExists(AllComponents{}, newEntity, iEntity);
}

///////////////////////////////////////
// Behavior for each added component //
///////////////////////////////////////
template <typename Component>
void Scene::onAddComponent(Entity iEntity, Component& ioComponent) {
    static_assert(false);
}

template <>
void Scene::onAddComponent<IdComponent>(Entity iEntity, IdComponent& ioComponent) {}

template <>
void Scene::onAddComponent<TagComponent>(Entity iEntity, TagComponent& ioComponent) {}

template <>
void Scene::onAddComponent<TransformComponent>(Entity iEntity, TransformComponent& ioComponent) {}

template <>
void Scene::onAddComponent<SpriteRendererComponent>(Entity iEntity, SpriteRendererComponent& ioComponent) {}

template <>
void Scene::onAddComponent<CircleRendererComponent>(Entity iEntity, CircleRendererComponent& ioComponent) {}

template <>
void Scene::onAddComponent<CameraComponent>(Entity iEntity, CameraComponent& ioComponent) {
    if (_viewportWidth > 0 && _viewportHeight > 0) {
        ioComponent.camera.setViewportSize(_viewportWidth, _viewportHeight);
    }
}

template <>
void Scene::onAddComponent<NativeScriptComponent>(Entity iEntity, NativeScriptComponent& ioComponent) {}

template <>
void Scene::onAddComponent<RigidBody2DComponent>(Entity iEntity, RigidBody2DComponent& ioComponent) {}

template <>
void Scene::onAddComponent<BoxCollider2DComponent>(Entity iEntity, BoxCollider2DComponent& ioComponent) {}

}
