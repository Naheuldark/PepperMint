#include "StatisticsPanel.h"

#include <imgui/imgui.h>

#include <PepperMint.h>

namespace PepperMint {

void StatisticsPanel::onImGuiRender(const std::string& iCurrentFile) {
    ImGui::Begin("Statistics");
    {
        std::string currentFile("Current File: ");
        if (iCurrentFile.empty()) {
            currentFile += "New File";
        } else {
            auto&& pos = (iCurrentFile.find_last_of('/') != std::string::npos ? iCurrentFile.find_last_of('/') : iCurrentFile.find_last_of('\\'));
            currentFile += iCurrentFile.substr(pos + 1);
        }
        ImGui::Text(currentFile.c_str());

        ImGui::Separator();

        auto&& stats = Renderer2D::Stats();
        ImGui::Text("Renderer2D:");
        ImGui::Text("\t\tDraw Calls: %d", stats.drawCalls);
        ImGui::Text("\t\tQuads: %d", stats.quadCount);
        ImGui::Text("\t\tVertices: %d", stats.totalVertexCount());
        ImGui::Text("\t\tIndices: %d", stats.totalIndexCount());
    }
    ImGui::End();
}
}
