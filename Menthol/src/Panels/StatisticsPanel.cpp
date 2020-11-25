#include "StatisticsPanel.h"

#include <imgui/imgui.h>

#include <PepperMint.h>

namespace PepperMint {

void StatisticsPanel::onImGuiRender() {
    ImGui::Begin("Statistics");
    {
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
