#include "SceneHierarchyPanel.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

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

    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)iEntity,
                                    ((_selectionContext == iEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow,
                                    tag.c_str());

    if (ImGui::IsItemClicked()) {
        _selectionContext = iEntity;
    }

    if (opened) {
        if (ImGui::TreeNodeEx((void*)9817239, ImGuiTreeNodeFlags_OpenOnArrow, tag.c_str())) {
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
}

namespace {

void drawVec3Control(const std::string& iLabel, glm::vec3& ioValues, float iResetValue = 0.0f, float iMin = 0.0f, float iColumnWidth = 100.0f) {
    ImGui::PushID(iLabel.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, iColumnWidth);

    // Display control label
    ImGui::Text(iLabel.c_str());
    ImGui::NextColumn();

    // Display X, Y, Z buttons
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("X", buttonSize))
            ioValues.x = iResetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &ioValues.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
    }

    ImGui::SameLine();

    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        if (ImGui::Button("Y", buttonSize))
            ioValues.y = iResetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &ioValues.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
    }

    ImGui::SameLine();

    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        if (ImGui::Button("Z", buttonSize))
            ioValues.z = iResetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &ioValues.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
    }

    ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
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
            auto&& transformComponent = iEntity.get<TransformComponent>();

            // Draw Translation controls
            drawVec3Control("Translation", transformComponent.translation);

            // Draw Rotation controls (cast in degress for the UI)
            auto&& rotation = glm::degrees(transformComponent.rotation);
            drawVec3Control("Rotation", rotation);
            transformComponent.rotation = glm::radians(rotation);

            // Draw Scale controls
            drawVec3Control("Scale", transformComponent.scale, 1.0f);

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

    if (iEntity.has<SpriteRendererComponent>()) {
        if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer")) {
            auto&& color = iEntity.get<SpriteRendererComponent>().color;
            ImGui::ColorEdit4("Color", glm::value_ptr(color));
            ImGui::TreePop();
        }
    }
}

}
