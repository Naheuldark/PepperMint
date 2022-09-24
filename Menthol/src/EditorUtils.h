#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <PepperMint.h>

namespace Menthol {

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

void drawVec3Control(const std::string& iLabel,
                     glm::vec3&         ioValues,
                     float              iResetValue  = 0.0f,
                     float              iMin         = 0.0f,
                     float              iColumnWidth = PepperMint::Window::sHighDPIScaleFactor * 100.0f) {
    ImGui::PushID(iLabel.c_str());

    drawTwoColumnsWithLabel(iLabel, iColumnWidth, [&]() {
        // Display X, Y, Z buttons
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

        float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};
        auto&& boldFont   = ImGui::GetIO().Fonts->Fonts[(int)PepperMint::FontStyle::BOLD];

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
void drawComponent(const std::string& iName, PepperMint::Entity iEntity, const ImGuiTreeNodeFlags iFlags, bool iRemovable, UIFunc UIFunction) {
    if (iEntity.has<Component>()) {
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool isOpened = ImGui::TreeNodeEx((void*)typeid(Component).hash_code(), iFlags, iName.c_str());
        ImGui::PopStyleVar();

        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

        bool allowPopup = iRemovable;

        if (ImGui::Button((allowPopup ? "..." : ""), ImVec2{lineHeight, lineHeight})) {
            ImGui::OpenPopup("##ComponentSettings");
        }

        bool removeComponent = false;
        if (allowPopup && ImGui::BeginPopup("##ComponentSettings")) {
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

template <typename Component>
void displayAddComponentEntry(PepperMint::Entity ioSelectedEntity, const std::string& iEntryName) {
    if (!ioSelectedEntity.has<Component>() && ImGui::MenuItem(iEntryName.c_str())) {
        ioSelectedEntity.add<Component>();
        ImGui::CloseCurrentPopup();
    }
}

void drawComponents(PepperMint::Entity ioSelectedEntity) {
    // Display Component Tag
    if (ioSelectedEntity.has<PepperMint::TagComponent>()) {
        auto&& tag = ioSelectedEntity.get<PepperMint::TagComponent>().tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strncpy_s(buffer, sizeof(buffer), tag.c_str(), tag.length());
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
        displayAddComponentEntry<PepperMint::CameraComponent>(ioSelectedEntity, "Camera");
        displayAddComponentEntry<PepperMint::ScriptComponent>(ioSelectedEntity, "Script");
        displayAddComponentEntry<PepperMint::SpriteRendererComponent>(ioSelectedEntity, "Sprite Renderer");
        displayAddComponentEntry<PepperMint::CircleRendererComponent>(ioSelectedEntity, "Circle Renderer");
        displayAddComponentEntry<PepperMint::RigidBody2DComponent>(ioSelectedEntity, "Rigid Body");
        displayAddComponentEntry<PepperMint::BoxCollider2DComponent>(ioSelectedEntity, "Box Collider");
        displayAddComponentEntry<PepperMint::CircleCollider2DComponent>(ioSelectedEntity, "Circle Collider");

        ImGui::EndPopup();
    }

    ImGui::PopItemWidth();

    // Display all Components
    const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                                     ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    drawComponent<PepperMint::TransformComponent>("Tranform", ioSelectedEntity, flags, false, [](auto&& transformComponent) {
        // Translation controls
        drawVec3Control("Translation", transformComponent.translation);

        // Rotation controls (cast in degress for the UI)
        auto&& rotation = glm::degrees(transformComponent.rotation);
        drawVec3Control("Rotation", rotation);
        transformComponent.rotation = glm::radians(rotation);

        // Scale controls
        drawVec3Control("Scale", transformComponent.scale, 1.0f);
    });

    drawComponent<PepperMint::CameraComponent>("Camera", ioSelectedEntity, flags, true, [](auto&& cameraComponent) {
        auto&& camera = cameraComponent.camera;

        const float columnWidth = PepperMint::Window::sHighDPIScaleFactor * 130.0f;

        drawTwoColumnsWithLabel("Primary", columnWidth, [&]() { ImGui::Checkbox("##Primary", &cameraComponent.primary); });

        drawTwoColumnsWithLabel("Projection", columnWidth, [&]() {
            const char* projectionTypeString[]      = {"Perspective", "Orthographic"};
            const char* currentProjectionTypeString = projectionTypeString[(int)camera.projectionType()];
            if (ImGui::BeginCombo("##Projection", currentProjectionTypeString)) {
                for (size_t i = 0; i < 2; ++i) {
                    bool isSelected = (currentProjectionTypeString == projectionTypeString[i]);
                    if (ImGui::Selectable(projectionTypeString[i], isSelected)) {
                        currentProjectionTypeString = projectionTypeString[i];
                        camera.setProjectionType((PepperMint::SceneCamera::ProjectionType)i);
                    }

                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
        });

        if (camera.projectionType() == PepperMint::SceneCamera::ProjectionType::PERSPECTIVE) {
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

        } else if (camera.projectionType() == PepperMint::SceneCamera::ProjectionType::ORTHOGRAPHIC) {
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

    drawComponent<PepperMint::ScriptComponent>("Script", ioSelectedEntity, flags, true, [&](auto&& scriptComponent) {
        bool scriptClassExists = PepperMint::ScriptEngine::EntityClassExists(scriptComponent.className);

        static char buffer[64];
        strcpy(buffer, scriptComponent.className.c_str());

        if (!scriptClassExists) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));
        }

        // Class name
        if (ImGui::InputText("Class", buffer, sizeof(buffer))) {
            scriptComponent.className = buffer;
        }

        // Fields
        if (auto&& scriptInstance = PepperMint::ScriptEngine::GetEntityScriptInstance(ioSelectedEntity.uuid())) {
            auto&& fields = scriptInstance->scriptClass()->fields();
            for (auto&& [name, field] : fields) {
                switch (field.type) {
                    case PepperMint::ScriptFieldType::FLOAT: {
                        float data = scriptInstance->fieldValue<float>(name);
                        if (ImGui::DragFloat(name.c_str(), &data)) {
                            scriptInstance->setFieldValue(name, data);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        if (!scriptClassExists) {
            ImGui::PopStyleColor();
        }
    });

    drawComponent<PepperMint::SpriteRendererComponent>("Sprite Renderer", ioSelectedEntity, flags, true, [](auto&& spriteComponent) {
        auto&& columnWidth = PepperMint::Window::sHighDPIScaleFactor * 100.0f;

        drawTwoColumnsWithLabel("Color", columnWidth, [&]() { ImGui::ColorEdit4("##Color", glm::value_ptr(spriteComponent.color)); });
        drawTwoColumnsWithLabel("Texture", columnWidth, [&]() {
            ImGui::Button("##Texture", ImVec2(100.0f, 0.0f));
            if (ImGui::BeginDragDropTarget()) {
                if (auto&& payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                    const wchar_t*        path        = (const wchar_t*)payload->Data;
                    std::filesystem::path texturePath = std::filesystem::path(kASSET_PATH) / path;

                    auto&& texture = PepperMint::Texture2D::Create(texturePath.string());
                    if (texture->isLoaded()) {
                        spriteComponent.texture = texture;
                    } else {
                        PM_WARN("Could not load texture {0}", texturePath.filename().string());
                    }
                }
                ImGui::EndDragDropTarget();
            }
        });
        drawTwoColumnsWithLabel(
            "Tiling Factor", columnWidth, [&]() { ImGui::DragFloat("##TilingFactor", &spriteComponent.tilingFactor, 0.1f, 0.0f, 100.0f); });
    });

    drawComponent<PepperMint::CircleRendererComponent>("Circle Renderer", ioSelectedEntity, flags, true, [](auto&& circleComponent) {
        auto&& columnWidth = PepperMint::Window::sHighDPIScaleFactor * 100.0f;

        drawTwoColumnsWithLabel("Color", columnWidth, [&]() { ImGui::ColorEdit4("##Color", glm::value_ptr(circleComponent.color)); });
        drawTwoColumnsWithLabel("Thickness", columnWidth, [&]() { ImGui::DragFloat("##Thickness", &circleComponent.thickness, 0.025f, 0.0f, 1.0f); });
        drawTwoColumnsWithLabel("Fade", columnWidth, [&]() { ImGui::DragFloat("##Fade", &circleComponent.fade, 0.00025f, 0.0f, 1.0f); });
    });

    drawComponent<PepperMint::RigidBody2DComponent>("Rigid Body", ioSelectedEntity, flags, true, [](auto&& rigidBodyComponent) {
        const float columnWidth = PepperMint::Window::sHighDPIScaleFactor * 130.0f;

        drawTwoColumnsWithLabel("Body Type", columnWidth, [&]() {
            const char* bodyTypeString[]      = {"Static", "Dynamic", "Kinematic"};
            const char* currentBodyTypeString = bodyTypeString[(int)rigidBodyComponent.type];
            if (ImGui::BeginCombo("##BodyType", currentBodyTypeString)) {
                for (size_t i = 0; i < 3; ++i) {
                    bool isSelected = (currentBodyTypeString == bodyTypeString[i]);
                    if (ImGui::Selectable(bodyTypeString[i], isSelected)) {
                        currentBodyTypeString   = bodyTypeString[i];
                        rigidBodyComponent.type = ((PepperMint::RigidBody2DComponent::BodyType)i);
                    }

                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
        });

        drawTwoColumnsWithLabel("Fixed Rotation", columnWidth, [&]() { ImGui::Checkbox("##FixedRotation", &rigidBodyComponent.fixedRotation); });
    });

    drawComponent<PepperMint::BoxCollider2DComponent>("Box Collider", ioSelectedEntity, flags, true, [](auto&& boxColliderComponent) {
        const float columnWidth = PepperMint::Window::sHighDPIScaleFactor * 130.0f;

        drawTwoColumnsWithLabel("Offset", columnWidth, [&]() { ImGui::DragFloat2("##Offset", glm::value_ptr(boxColliderComponent.offset)); });
        drawTwoColumnsWithLabel("Size", columnWidth, [&]() { ImGui::DragFloat2("##Size", glm::value_ptr(boxColliderComponent.size)); });
        drawTwoColumnsWithLabel("Density", columnWidth, [&]() { ImGui::DragFloat("##Density", &boxColliderComponent.density, 0.01f, 0.0f, 1.0f); });
        drawTwoColumnsWithLabel(
            "Friction", columnWidth, [&]() { ImGui::DragFloat("##Friction", &boxColliderComponent.friction, 0.01f, 0.0f, 1.0f); });
        drawTwoColumnsWithLabel(
            "Restitution", columnWidth, [&]() { ImGui::DragFloat("##Restitution", &boxColliderComponent.restitution, 0.01f, 0.0f, 1.0f); });
        drawTwoColumnsWithLabel("Restitution Threshold", columnWidth, [&]() {
            ImGui::DragFloat("##RestitutionThreshold", &boxColliderComponent.restitutionThreshold, 0.01f, 0.0f);
        });
    });

    drawComponent<PepperMint::CircleCollider2DComponent>("Circle Collider", ioSelectedEntity, flags, true, [](auto&& circleColliderComponent) {
        const float columnWidth = PepperMint::Window::sHighDPIScaleFactor * 130.0f;

        drawTwoColumnsWithLabel("Offset", columnWidth, [&]() { ImGui::DragFloat2("##Offset", glm::value_ptr(circleColliderComponent.offset)); });
        drawTwoColumnsWithLabel("Radius", columnWidth, [&]() { ImGui::DragFloat("##Radius", &circleColliderComponent.radius, 0.01f, 0.0f); });
        drawTwoColumnsWithLabel(
            "Density", columnWidth, [&]() { ImGui::DragFloat("##Density", &circleColliderComponent.density, 0.01f, 0.0f, 1.0f); });
        drawTwoColumnsWithLabel(
            "Friction", columnWidth, [&]() { ImGui::DragFloat("##Friction", &circleColliderComponent.friction, 0.01f, 0.0f, 1.0f); });
        drawTwoColumnsWithLabel(
            "Restitution", columnWidth, [&]() { ImGui::DragFloat("##Restitution", &circleColliderComponent.restitution, 0.01f, 0.0f, 1.0f); });
        drawTwoColumnsWithLabel("Restitution Threshold", columnWidth, [&]() {
            ImGui::DragFloat("##RestitutionThreshold", &circleColliderComponent.restitutionThreshold, 0.01f, 0.0f);
        });
    });
}
}
