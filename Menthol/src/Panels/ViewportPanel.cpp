#include "ViewportPanel.h"

#include <imgui/imgui.h>

#include <ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>

namespace Menthol {

// External variables
extern const std::filesystem::path xASSET_PATH; // defined in ContentBrowserPanel.cpp

void ViewportPanel::onUpdate(PepperMint::Timestep iTimestep) {
    // Mouse picking
    //	(0,0) is the bottom left corner
    //	(_viewportSize.x, _viewportSize.y) is the top right corner
    auto&& [mx, my] = ImGui::GetMousePos();
    mx -= _viewportBounds[0].x;
    my -= _viewportBounds[0].y;
    auto&& viewportSize = _viewportBounds[1] - _viewportBounds[0];
    my                  = viewportSize.y - my;
    int mouseX          = (int)mx;
    int mouseY          = (int)my;

    if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
        int pixelData  = _frameBuffer->readPixel(1, mouseX, mouseY);
        _hoveredEntity = (pixelData == -1) ? PepperMint::Entity() : PepperMint::Entity((entt::entity)pixelData, _activeScene.get());
    }
}

void ViewportPanel::onImGuiRender() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");
    {
        // Bounds (with/without tab)
        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset    = ImGui::GetWindowPos();
        _viewportBounds[0]     = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
        _viewportBounds[1]     = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

        // Focus
        _viewportFocused = ImGui::IsWindowFocused();
        _viewportHovered = ImGui::IsWindowHovered();
        PepperMint::Application::Get().imguiLayer()->setBlockEvents(!_viewportFocused && !_viewportHovered);

        // Size
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _viewportSize            = {viewportPanelSize.x, viewportPanelSize.y};

        // Viewport rendered image
        auto&& textureId = _frameBuffer->colorAttachmentRendererId();
        ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2{_viewportSize.x, _viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

        // Drag & Drop
        if (ImGui::BeginDragDropTarget()) {
            if (auto&& payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const wchar_t* path = (const wchar_t*)payload->Data;
                _sceneToOpen        = std::filesystem::path(xASSET_PATH) / path;
            }
            ImGui::EndDragDropTarget();
        }

        // Gizmos
        if (_editorMode && _selectedEntity && _gizmoType != -1) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(
                _viewportBounds[0].x, _viewportBounds[0].y, _viewportBounds[1].x - _viewportBounds[0].x, _viewportBounds[1].y - _viewportBounds[0].y);

            // Editor Camera
            auto&& cameraProjection = _editorCamera.projection();
            auto&& cameraView       = _editorCamera.viewMatrix();

            // Entity transform
            auto&&    transformComponent = _selectedEntity.get<PepperMint::TransformComponent>();
            glm::mat4 transform          = transformComponent.transform();

            // Snapping (0.5m for translation/scale - 45 degrees for rotation)
            bool  snap      = PepperMint::Input::IsKeyPressed(PepperMint::Key::LEFT_CONTROL);
            float snapValue = 0.5f;
            if (_gizmoType == ImGuizmo::OPERATION::ROTATE) {
                snapValue = 45.0f;
            }

            float snapValues[3] = {snapValue, snapValue, snapValue};

            ImGuizmo::Manipulate(glm::value_ptr(cameraView),
                                 glm::value_ptr(cameraProjection),
                                 (ImGuizmo::OPERATION)_gizmoType,
                                 ImGuizmo::LOCAL,
                                 glm::value_ptr(transform),
                                 nullptr,
                                 snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing()) {
                glm::vec3 translation, rotation, scale;
                PepperMint::Math::decompose(transform, translation, rotation, scale);

                auto&& deltaRotation           = rotation - transformComponent.rotation;
                transformComponent.translation = translation;
                transformComponent.rotation += deltaRotation;
                transformComponent.scale = scale;
            }
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

bool ViewportPanel::onKeyPressed(PepperMint::KeyPressedEvent& iEvent) {
    switch (iEvent.keyCode()) {
        // Gizmos
        case PepperMint::Key::Q: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = -1;
            }
            break;
        }
        case PepperMint::Key::W: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            break;
        }
        case PepperMint::Key::E: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            break;
        }
        case PepperMint::Key::R: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = ImGuizmo::OPERATION::SCALE;
            }
            break;
        }

        default:
            break;
    }

    return true;
}

bool ViewportPanel::onMouseButtonPressed(PepperMint::MouseButtonPressedEvent& iEvent) {
    if (iEvent.mouseButton() == PepperMint::Mouse::BUTTON_LEFT) {
        if (_viewportHovered && (!ImGuizmo::IsOver()) && (!PepperMint::Input::IsKeyPressed(PepperMint::Key::LEFT_ALT))) {
            _selectedEntity = _hoveredEntity;
        }
    }

    return true;
}
}
