#include "pmpch.h"

#include "PepperMint/Renderer/Renderer2D.h"
#include "PepperMint/Scene/Components.h"
#include "PepperMint/Scene/Entity.h"
#include "PepperMint/Scene/Scene.h"

namespace PepperMint {

static void DoMath(const glm::mat4& iTransform) {}
static void OnTransformConstruct(entt::registry& iRegistry, entt::entity iEntity) {}

Entity Scene::createEntity(const std::string& iName) {
    Entity entity(_registry.create(), this);
    entity.add<TransformComponent>();
    entity.add<TagComponent>(iName);
    return entity;
}

void Scene::onUpdate(Timestep iTimestep) {
    Camera*    mainCamera      = nullptr;
    glm::mat4* cameraTransform = nullptr;

    auto&& group = _registry.view<TransformComponent, CameraComponent>();
    for (auto&& entity : group) {
        auto&& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

        if (camera.primary) {
            mainCamera      = &camera.camera;
            cameraTransform = &transform.transform;
            break;
        }
    }

    if (mainCamera) {
        Renderer2D::BeginScene(mainCamera->projection(), *cameraTransform);
        {
            auto&& group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto&& entity : group) {
                auto&& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::DrawQuad(transform, 1.0f, nullptr, sprite.color);
            }
        }
        Renderer2D::EndScene();
    }
}
}
