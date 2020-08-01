#include "pmpch.h"
#include "PepperMint/Scene/Scene.h"

#include "PepperMint/Scene/Components.h"
#include "PepperMint/Renderer/Renderer2D.h"

namespace PepperMint {

static void DoMath(const glm::mat4& iTransform) {}
static void OnTransformConstruct(entt::registry& iRegistry, entt::entity iEntity) {}

Scene::Scene() {
#if ENTT_EXAMPLE_CODE
	entt::entity entity = _registry.create();

	_registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
	_registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

	if (_registry.has<TransformComponent>(entity)) {
		TransformComponent& transform = _registry.get<TransformComponent>(entity);
	}

	auto view = _registry.view<TransformComponent>();
	for (auto&& entity : view) {
		TransformComponent& transform = view.get<TransformComponent>(entity);
	}

	auto group = _registry.group<TransformComponent>(entt::get<MeshComponent>);
	for (auto entity : group) {
		auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
	}
#endif // ENTT_EXAMPLE_CODE
}

entt::entity Scene::createEntity() {
	return _registry.create();
}

void Scene::onUpdate(Timestep iTimestep) {
	auto&& group = _registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
	for (auto&& entity : group) {
		auto&& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
		Renderer2D::DrawQuad(transform, 1.0f, nullptr, sprite.color);
	}
}
}