#include "pmpch.h"

#include "PepperMint/Renderer/Renderer2D.h"
#include "PepperMint/Scene/Components.h"
#include "PepperMint/Scene/Entity.h"
#include "PepperMint/Scene/Scene.h"

namespace PepperMint {

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
        auto&& scriptComponent = scriptView.get<NativeScriptComponent>(entity);

        // TODO: Move to Scene onScenePlay
        if (!scriptComponent.script) {
            scriptComponent.script          = scriptComponent.instantiateScript();
            scriptComponent.script->_entity = Entity(entity, this);
            scriptComponent.script->onCreate();
        }

        scriptComponent.script->onUpdate(iTimestep);
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
        Renderer2D::BeginScene(*mainCamera, *cameraTransform);
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
