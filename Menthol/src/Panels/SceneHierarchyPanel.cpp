#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

#include <PepperMint/Scene/Components.h>

namespace PepperMint {

void SceneHierarchyPanel::onImGuiRender() {
    ImGui::Begin("Scene Hierarchy");
    {
        _context->_registry.each([&](auto&& entity) { drawEntityNode({entity, _context.get()}); });
    }
    ImGui::End();
}

void SceneHierarchyPanel::drawEntityNode(Entity iEntity) {
    auto&&             tag    = iEntity.get<TagComponent>().tag;

	ImGuiTreeNodeFlags flags  = ((_selectionContext == iEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool               opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)iEntity, flags, tag.c_str());

	if (ImGui::IsItemClicked()) {
        _selectionContext = iEntity;
	}

	if (opened) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        bool               opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
        if (opened) {
            ImGui::TreePop();
		}
        ImGui::TreePop();
	}
}
}
