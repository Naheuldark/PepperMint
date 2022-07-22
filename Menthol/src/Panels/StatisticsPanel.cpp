#include "StatisticsPanel.h"

#include <imgui/imgui.h>

namespace Menthol {

void StatisticsPanel::onImGuiRender() {
    ImGui::Begin("Statistics");
    {
        std::string currentFile("Current File: ");
        if (_currentFile.empty()) {
            currentFile += "New File";
        } else {
            auto&& pos = (_currentFile.find_last_of('/') != std::string::npos ? _currentFile.find_last_of('/') : _currentFile.find_last_of('\\'));
            currentFile += _currentFile.substr(pos + 1);
        }
        ImGui::Text(currentFile.c_str());

        ImGui::Separator();

        std::string hoveredEntity("None");
        if (_hoveredEntity) {
            hoveredEntity = _hoveredEntity.get<PepperMint::TagComponent>().tag;
        }
        ImGui::Text("Hovered Entity: %s", hoveredEntity.c_str());

        ImGui::Separator();

        auto&& stats = PepperMint::Renderer2D::Stats();
        ImGui::Text("Renderer2D:");
        ImGui::Text("\t\tDraw Calls: %d", stats.drawCalls);
        ImGui::Text("\t\tQuads: %d", stats.quadCount);
        ImGui::Text("\t\tVertices: %d", stats.totalVertexCount());
        ImGui::Text("\t\tIndices: %d", stats.totalIndexCount());
    }
    ImGui::End();
}
}
