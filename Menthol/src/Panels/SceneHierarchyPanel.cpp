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

    if (iEntity.has<CameraComponent>()) {
        if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
            auto&& cameraComponent = iEntity.get<CameraComponent>();
            auto&& camera          = cameraComponent.camera;

            ImGui::Checkbox("Primary", &cameraComponent.primary);

            const char* projectionTypeString[]      = {"Perspective", "Orthographic"};
            const char* currentProjectionTypeString = projectionTypeString[(int)camera.projectionType()];
            if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
                for (size_t i = 0; i < 2; ++i) {
                    bool isSelected = (currentProjectionTypeString == projectionTypeString[i]);
                    if (ImGui::Selectable(projectionTypeString[i], isSelected)) {
                        currentProjectionTypeString = projectionTypeString[i];
                        camera.setProjectionType((SceneCamera::ProjectionType)i);
                    }

                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            if (camera.projectionType() == SceneCamera::ProjectionType::PERSPECTIVE) {
                float verticalFOV = glm::degrees(camera.perspectiveVerticalFOC());
                if (ImGui::DragFloat("Vertical FOV", &verticalFOV)) {
                    camera.setPerspectiveVerticalFOC(glm::radians(verticalFOV));
                }

                float nearClip = camera.perspectiveNearClip();
                if (ImGui::DragFloat("Near", &nearClip)) {
                    camera.setPerspectiveNearClip(nearClip);
                }

                float farClip = camera.perspectiveFarClip();
                if (ImGui::DragFloat("Far", &farClip)) {
                    camera.setPerspectiveFarClip(farClip);
                }

            } else if (camera.projectionType() == SceneCamera::ProjectionType::ORTHOGRAPHIC) {
                float size = camera.orthographicSize();
                if (ImGui::DragFloat("Size", &size)) {
                    camera.setOrthographicSize(size);
                }

                float nearClip = camera.orthographicNearClip();
                if (ImGui::DragFloat("Near", &nearClip)) {
                    camera.setOrthographicNearClip(nearClip);
                }

                float farClip = camera.orthographicFarClip();
                if (ImGui::DragFloat("Far", &farClip)) {
                    camera.setOrthographicFarClip(farClip);
                }

                ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio);

            } else {
                PM_CORE_ERROR("Unknown Camera ProjectionType!")
            }

            ImGui::TreePop();
        }
    }
}

}
