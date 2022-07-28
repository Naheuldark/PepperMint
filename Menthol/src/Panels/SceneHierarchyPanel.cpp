#include <imgui/imgui.h>

#include <ImGuizmo.h>

#include "SceneHierarchyPanel.h"

namespace Menthol {

void SceneHierarchyPanel::onImGuiRender() {
    ImGui::Begin("Scene Hierarchy");
    {
        _context->forEachEntity([&](auto&& entity) { drawEntityNode({entity, _context.get()}); });

        // Left-click on blank space
        if (PepperMint::Input::IsMouseButtonPressed(PepperMint::Mouse::BUTTON_LEFT) && ImGui::IsWindowHovered()) {
            _selectedEntity = {};
        }

        // Right-click on blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                _selectedEntity = _context->createEntity("Empty Entity");
            } else if (ImGui::MenuItem("Create Camera")) {
                _selectedEntity = _context->createEntity("Camera");
                _selectedEntity.add<PepperMint::CameraComponent>();
            } else if (ImGui::MenuItem("Create Sprite")) {
                _selectedEntity = _context->createEntity("Sprite");
                _selectedEntity.add<PepperMint::SpriteRendererComponent>();
            } else if (ImGui::MenuItem("Create Circle")) {
                _selectedEntity = _context->createEntity("Circle");
                _selectedEntity.add<PepperMint::CircleRendererComponent>();
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void SceneHierarchyPanel::drawEntityNode(PepperMint::Entity iEntity) {
    auto&& tag = iEntity.get<PepperMint::TagComponent>().tag;

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
