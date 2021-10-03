#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

namespace PepperMint {

extern const std::filesystem::path xASSET_PATH = "assets";

const float kPADDING        = 8.0f;
const float kTHUMBNAIL_SIZE = 80.0f;

ContentBrowserPanel::ContentBrowserPanel() : _currentDirectory(xASSET_PATH) {
    _directoryIcon = Texture2D::Create("resources/icons/ContentBrowser/DirectoryIcon.png");
    _fileIcon      = Texture2D::Create("resources/icons/ContentBrowser/FileIcon.png");
}

void ContentBrowserPanel::onImGuiRender() {
    ImGui::Begin("Content Browser");
    {
        if (_currentDirectory != std::filesystem::path(xASSET_PATH)) {
            if (ImGui::Button("<-")) {
                _currentDirectory = _currentDirectory.parent_path();
            }
        }

        float cellSize = Window::sHighDPIScaleFactor * kTHUMBNAIL_SIZE + Window::sHighDPIScaleFactor * kPADDING;

        float panelWidth  = ImGui::GetContentRegionAvail().x;
        int   columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1) {
            columnCount = 1;
        }

        ImGui::Columns(columnCount, 0, false);

        for (auto&& directoryEntry : std::filesystem::directory_iterator(_currentDirectory)) {
            auto&& path           = directoryEntry.path();
            auto&& relativePath   = std::filesystem::relative(path, xASSET_PATH);
            auto&& filenameString = relativePath.filename().string();

            ImGui::PushID(filenameString.c_str());

            Ref<Texture2D> icon = directoryEntry.is_directory() ? _directoryIcon : _fileIcon;

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::ImageButton((ImTextureID)icon->rendererId(),
                               {Window::sHighDPIScaleFactor * kTHUMBNAIL_SIZE, Window::sHighDPIScaleFactor * kTHUMBNAIL_SIZE},
                               {0, 1},
                               {1, 0});
            if (ImGui::BeginDragDropSource()) {
                const wchar_t* itemPath = relativePath.c_str();

                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
                ImGui::EndDragDropSource();
            }
            ImGui::PopStyleColor();

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                if (directoryEntry.is_directory()) {
                    _currentDirectory /= path.filename();
                }
            }
            ImGui::TextWrapped(filenameString.c_str());

            ImGui::NextColumn();

            ImGui::PopID();
        }

        ImGui::Columns(1);
    }
    ImGui::End();
}
}
