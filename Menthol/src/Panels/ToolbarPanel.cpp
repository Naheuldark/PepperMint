#include "ToolbarPanel.h"

#include <imgui/imgui.h>

namespace PepperMint {

ToolbarPanel::ToolbarPanel() {
    _playIcon = Texture2D::Create("resources/icons/Editor/PlayButton.png");
    _stopIcon = Texture2D::Create("resources/icons/Editor/StopButton.png");
}

void ToolbarPanel::onImGuiRender() {
    auto&& colors        = ImGui::GetStyle().Colors;
    auto&& buttonHovered = colors[ImGuiCol_ButtonHovered];
    auto&& buttonActive  = colors[ImGuiCol_ButtonActive];

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

    ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        float          size = ImGui::GetWindowHeight() - 4.0f;
        Ref<Texture2D> icon = _sceneState == SceneState::EDIT ? _playIcon : _stopIcon;
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
        if (ImGui::ImageButton((ImTextureID)icon->rendererId(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0)) {
            if (_sceneState == SceneState::EDIT) {
                _sceneState = SceneState::PLAY;
            } else if (_sceneState == SceneState::PLAY) {
                _sceneState = SceneState::EDIT;
            }
        }
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
    }
    ImGui::End();
}
}
