#include "SceneHierarchyPanel.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <PepperMint/Scene/Components.h>

namespace PepperMint {

void SceneHierarchyPanel::onImGuiRender() {
    ImGui::Begin("Scene Hierarchy");
    {
        _context->_registry.each([&](auto&& entity) { drawEntityNode({entity, _context.get()}); });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            _selectionContext = {};
        }
    }
    ImGui::End();

    ImGui::Begin("Properties");
    {
        if (_selectionContext) {
            drawComponents(_selectionContext);
        }
    }
    ImGui::End();
}

void SceneHierarchyPanel::drawEntityNode(Entity iEntity) {
    auto&& tag = iEntity.get<TagComponent>().tag;

    ImGuiTreeNodeFlags flags  = ((_selectionContext == iEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool               opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)iEntity, flags, tag.c_str());

    if (ImGui::IsItemClicked()) {
        _selectionContext = iEntity;
    }

    if (opened) {
        ImGuiTreeNodeFlags flags  = ImGuiTreeNodeFlags_OpenOnArrow;
        bool               opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
        if (opened) {
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
}

void SceneHierarchyPanel::drawComponents(Entity iEntity) {
    if (iEntity.has<TagComponent>()) {
        auto&& tag = iEntity.get<TagComponent>().tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), tag.c_str());
        if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
            tag = std::string(buffer);
        }
    }

	if (iEntity.has<TransformComponent>()) {
        if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
            auto&& transform = iEntity.get<TransformComponent>().transform;
            ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

			ImGui::TreePop();
		}
	}
}

}
