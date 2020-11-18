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
    // Update scripts
    auto&& scriptView = _registry.view<NativeScriptComponent>();
    for (auto&& entity : scriptView) {
        auto&& nativeScriptComponent = scriptView.get<NativeScriptComponent>(entity);

        if (!nativeScriptComponent.instance) {
            nativeScriptComponent.createInstanceFunc();
            nativeScriptComponent.instance->_entity = Entity(entity, this);

            if (nativeScriptComponent.onCreateFunc) {
                nativeScriptComponent.onCreateFunc(nativeScriptComponent.instance);
            }
        }

        if (nativeScriptComponent.onUpdateFunc) {
            nativeScriptComponent.onUpdateFunc(nativeScriptComponent.instance, iTimestep);
        }
    }

    // Render 2D
    Camera*    mainCamera      = nullptr;
    glm::mat4* cameraTransform = nullptr;

    auto&& renderView = _registry.view<TransformComponent, CameraComponent>();
    for (auto&& entity : renderView) {
        auto&& [transformComponent, cameraComponent] = renderView.get<TransformComponent, CameraComponent>(entity);

        if (cameraComponent.primary) {
            mainCamera      = &cameraComponent.camera;
            cameraTransform = &transformComponent.transform;
            break;
        }
    }

    if (mainCamera) {
        Renderer2D::BeginScene(mainCamera->projection(), *cameraTransform);
        {
            auto&& group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto&& entity : group) {
                auto&& [transformComponent, spriteComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::DrawQuad(transformComponent, 1.0f, nullptr, spriteComponent.color);
            }
        }
        Renderer2D::EndScene();
    }
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
}
