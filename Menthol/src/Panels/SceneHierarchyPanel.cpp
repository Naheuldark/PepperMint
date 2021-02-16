#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

#include <ImGuizmo.h>

#include <PepperMint/Core/Input.h>
#include <PepperMint/Scene/Components.h>

namespace PepperMint {

void SceneHierarchyPanel::onImGuiRender() {
    ImGui::Begin("Scene Hierarchy");
    {
        _context->_registry.each([&](auto&& entity) { drawEntityNode({entity, _context.get()}); });

        // Left-click on blank space
        if (Input::IsMouseButtonPressed(Mouse::BUTTON_LEFT) && ImGui::IsWindowHovered()) {
            _selectedEntity = {};
        }

        // Left-click with a hovered entity in the viewport
        if (Input::IsMouseButtonPressed(Mouse::BUTTON_LEFT) && _hoveredEntity && (!ImGuizmo::IsOver())) {
            _selectedEntity = _hoveredEntity;
        }

        // Right-click on blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                _context->createEntity("Empty Entity");
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void SceneHierarchyPanel::drawEntityNode(Entity iEntity) {
    auto&& tag = iEntity.get<TagComponent>().tag;

    ImGuiTreeNodeFlags commonFlags  = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    ImGuiTreeNodeFlags selectedFlag = ((_selectedEntity == iEntity) ? ImGuiTreeNodeFlags_Selected : 0);

    bool isOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)iEntity, commonFlags | selectedFlag, tag.c_str());

    if (ImGui::IsItemClicked()) {
        _selectedEntity = iEntity;
    }

    bool entityDeleted = false;
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete Entity")) {
            entityDeleted = true;
        }
        ImGui::EndPopup();
    }

    if (isOpened) {
        if (ImGui::TreeNodeEx((void*)9817239, commonFlags, tag.c_str())) {
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    if (entityDeleted) {
        _context->destroyEntity(iEntity);
        if (_selectedEntity == iEntity) {
            _selectedEntity = {};
        }
    }
}
}
