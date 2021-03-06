#include "PropertiesPanel.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace PepperMint {

namespace {

void drawFloatControl(const std::string& iLabel, float& ioValue, float iResetValue, const ImVec2& iButtonSize, const ImVec4& iColor, ImFont* iFont) {
    ImGui::PushStyleColor(ImGuiCol_Button, iColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{iColor.x + 0.1f, iColor.y + 0.1f, iColor.z + 0.1f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, iColor);
    ImGui::PushFont(iFont);
    if (ImGui::Button(iLabel.c_str(), iButtonSize)) {
        ioValue = iResetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat(std::string("##" + iLabel).c_str(), &ioValue, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
}

template <typename UIFunc>
void drawTwoColumnsWithLabel(const std::string& iLabel, float iColumnWidth, UIFunc UIFunction) {
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, iColumnWidth);

    ImGui::Text(iLabel.c_str());
    ImGui::NextColumn();

    UIFunction();
    ImGui::Columns(1);
}

void drawVec3Control(const std::string& iLabel, glm::vec3& ioValues, float iResetValue = 0.0f, float iMin = 0.0f, float iColumnWidth = 100.0f) {
    ImGui::PushID(iLabel.c_str());

    drawTwoColumnsWithLabel(iLabel, iColumnWidth, [&]() {
        // Display X, Y, Z buttons
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

        float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};
        auto&& boldFont   = ImGui::GetIO().Fonts->Fonts[(int)FontStyle::BOLD];

        drawFloatControl("X", ioValues.x, iResetValue, buttonSize, ImVec4{0.8f, 0.1f, 0.15f, 1.0f}, boldFont);
        ImGui::SameLine();
        drawFloatControl("Y", ioValues.y, iResetValue, buttonSize, ImVec4{0.2f, 0.7f, 0.2f, 1.0f}, boldFont);
        ImGui::SameLine();
        drawFloatControl("Z", ioValues.z, iResetValue, buttonSize, ImVec4{0.1f, 0.25f, 0.8f, 1.0f}, boldFont);

        ImGui::PopStyleVar();
    });

    ImGui::PopID();
}

template <typename Component, typename UIFunc>
void drawComponent(const std::string& iName, Entity iEntity, const ImGuiTreeNodeFlags iFlags, bool iRemovable, UIFunc UIFunction) {
    if (iEntity.has<Component>()) {
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool isOpened = ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), iFlags, iName.c_str());
        ImGui::PopStyleVar();

        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

        if (ImGui::Button("...", ImVec2{lineHeight, lineHeight})) {
            ImGui::OpenPopup("##ComponentSettings");
        }

        bool removeComponent = false;
        if (ImGui::BeginPopup("##ComponentSettings")) {
            if (iRemovable && ImGui::MenuItem("Remove Component")) {
                removeComponent = true;
            }
            ImGui::EndPopup();
        }

        if (isOpened) {
            UIFunction(iEntity.get<Component>());
            ImGui::TreePop();
        }

        if (removeComponent) {
            iEntity.remove<Component>();
        }
    }
}

void drawComponents(Entity ioSelectedEntity) {
    // Display Component Tag
    if (ioSelectedEntity.has<TagComponent>()) {
        auto&& tag = ioSelectedEntity.get<TagComponent>().tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(tag.c_str()));
        if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
            tag = std::string(buffer);
        }
    }

    ImGui::SameLine();

    // Display "Add Component" button next to the Tag
    ImGui::PushItemWidth(-1);

    if (ImGui::Button("Add Component")) {
        ImGui::OpenPopup("##AddComponent");
    }

    if (ImGui::BeginPopup("##AddComponent")) {
        if (!ioSelectedEntity.has<CameraComponent>() && ImGui::MenuItem("Camera")) {
            ioSelectedEntity.add<CameraComponent>();
            ImGui::CloseCurrentPopup();
        }

        if (!ioSelectedEntity.has<SpriteRendererComponent>() && ImGui::MenuItem("Sprite Renderer")) {
            ioSelectedEntity.add<SpriteRendererComponent>();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::PopItemWidth();

    // Display all Components
    const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                                     ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    drawComponent<TransformComponent>("Tranform", ioSelectedEntity, flags, false, [](auto&& transformComponent) {
        // Translation controls
        drawVec3Control("Translation", transformComponent.translation);

        // Rotation controls (cast in degress for the UI)
        auto&& rotation = glm::degrees(transformComponent.rotation);
        drawVec3Control("Rotation", rotation);
        transformComponent.rotation = glm::radians(rotation);

        // Scale controls
        drawVec3Control("Scale", transformComponent.scale, 1.0f);
    });

    drawComponent<CameraComponent>("Camera", ioSelectedEntity, flags, true, [](auto&& cameraComponent) {
        auto&& camera = cameraComponent.camera;

        const float columnWidth = 130.0f;

        drawTwoColumnsWithLabel("Primary", columnWidth, [&]() { ImGui::Checkbox("##Primary", &cameraComponent.primary); });

        drawTwoColumnsWithLabel("Projection", columnWidth, [&]() {
            const char* projectionTypeString[]      = {"Perspective", "Orthographic"};
            const char* currentProjectionTypeString = projectionTypeString[(int)camera.projectionType()];
            if (ImGui::BeginCombo("##Projection", currentProjectionTypeString)) {
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
        });

        if (camera.projectionType() == SceneCamera::ProjectionType::PERSPECTIVE) {
            drawTwoColumnsWithLabel("Vertical FOV", columnWidth, [&]() {
                float verticalFOV = glm::degrees(camera.perspectiveVerticalFOV());
                if (ImGui::DragFloat("##VerticalFOV", &verticalFOV)) {
                    camera.setPerspectiveVerticalFOV(glm::radians(verticalFOV));
                }
            });

            drawTwoColumnsWithLabel("Near", columnWidth, [&]() {
                float nearClip = camera.perspectiveNearClip();
                if (ImGui::DragFloat("##Near", &nearClip)) {
                    camera.setPerspectiveNearClip(nearClip);
                }
            });

            drawTwoColumnsWithLabel("Far", columnWidth, [&]() {
                float farClip = camera.perspectiveFarClip();
                if (ImGui::DragFloat("##Far", &farClip)) {
                    camera.setPerspectiveFarClip(farClip);
                }
            });

        } else if (camera.projectionType() == SceneCamera::ProjectionType::ORTHOGRAPHIC) {
            drawTwoColumnsWithLabel("Size", columnWidth, [&]() {
                float size = camera.orthographicSize();
                if (ImGui::DragFloat("##Size", &size)) {
                    camera.setOrthographicSize(size);
                }
            });

            drawTwoColumnsWithLabel("Near", columnWidth, [&]() {
                float nearClip = camera.orthographicNearClip();
                if (ImGui::DragFloat("##Near", &nearClip)) {
                    camera.setOrthographicNearClip(nearClip);
                }
            });

            drawTwoColumnsWithLabel("Far", columnWidth, [&]() {
                float farClip = camera.orthographicFarClip();
                if (ImGui::DragFloat("##Far", &farClip)) {
                    camera.setOrthographicFarClip(farClip);
                }
            });

            drawTwoColumnsWithLabel(
                "Fixed Aspect Ratio", columnWidth, [&]() { ImGui::Checkbox("##FixedAspectRatio", &cameraComponent.fixedAspectRatio); });

        } else {
            PM_CORE_ERROR("Unknown Camera ProjectionType!")
        }
    });

    drawComponent<SpriteRendererComponent>("Sprite Renderer", ioSelectedEntity, flags, true, [](auto&& spriteComponent) {
        drawTwoColumnsWithLabel("Color", 100.0f, [&]() { ImGui::ColorEdit4("##Color", glm::value_ptr(spriteComponent.color)); });
    });
}
}

void PropertiesPanel::onImGuiRender() {
    ImGui::Begin("Properties");
    {
        if (_selectedEntity) {
            drawComponents(_selectedEntity);
        }
    }
    ImGui::End();
}
}
