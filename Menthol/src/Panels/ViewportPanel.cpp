#include "ViewportPanel.h"

#include <imgui/imgui.h>

#include <ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>

#include <PepperMint/Core/Application.h>
#include <PepperMint/Math/Math.h>

namespace PepperMint {

void ViewportPanel::onUpdate(Timestep iTimestep) {
    // Mouse picking
    //	(0,0) is the bottom left corner
    //	(_viewportSize.x, _viewportSize.y) is the top right corner
    auto&& [mx, my] = ImGui::GetMousePos();
    mx -= _viewportBounds[0].x;
    my -= _viewportBounds[0].y;
    my         = _viewportSize.y - my;
    int mouseX = (int)mx;
    int mouseY = (int)my;

    if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)_viewportSize.x && mouseY < (int)_viewportSize.y) {
        int pixelData  = _frameBuffer->readPixel(1, mouseX, mouseY);
        _hoveredEntity = (pixelData == -1) ? Entity() : Entity((entt::entity)pixelData, _activeScene.get());
    }
}

void ViewportPanel::onImGuiRender() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");
    {
        auto&& viewportOffset = ImGui::GetCursorPos(); // Includes tab bar

        _viewportFocused = ImGui::IsWindowFocused();
        _viewportHovered = ImGui::IsWindowHovered();
        Application::Get().imguiLayer()->setBlockEvents(!_viewportFocused && !_viewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _viewportSize            = {viewportPanelSize.x, viewportPanelSize.y};

        auto&& textureId = _frameBuffer->colorAttachmentRendererId();
        ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2{_viewportSize.x, _viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

        // Bounds
        auto&& windowSize  = ImGui::GetWindowSize();
        auto&& windowPos   = ImGui::GetWindowPos();
        _viewportBounds[0] = {windowPos.x + viewportOffset.x, windowPos.y + viewportOffset.y};                 // Top left corner
        _viewportBounds[1] = {_viewportBounds[0].x + _viewportSize.x, _viewportBounds[0].y + _viewportSize.y}; // Bottom right corner

        // Gizmos
        if (_editorMode && _selectedEntity && _gizmoType != -1) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            float windowWidth  = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            // Editor Camera
            auto&& cameraProjection = _editorCamera.projection();
            auto&& cameraView       = _editorCamera.viewMatrix();

            // Entity transform
            auto&&    transformComponent = _selectedEntity.get<TransformComponent>();
            glm::mat4 transform          = transformComponent.transform();

            // Snapping (0.5m for translation/scale - 45 degrees for rotation)
            bool  snap      = Input::IsKeyPressed(Key::LEFT_CONTROL);
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
                Math::decompose(transform, translation, rotation, scale);

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

bool ViewportPanel::onKeyPressed(KeyPressedEvent& iEvent) {
    switch (iEvent.keyCode()) {
        // Gizmos
        case Key::Q: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = -1;
            }
            break;
        }
        case Key::W: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            break;
        }
        case Key::E: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            break;
        }
        case Key::R: {
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
}
