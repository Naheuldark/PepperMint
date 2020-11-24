#include "StatisticsPanel.h"

#include <imgui/imgui.h>

#include <PepperMint.h>

namespace PepperMint {

void StatisticsPanel::onImGuiRender() {
    ImGui::Begin("Statistics");
    {
        auto&& stats = Renderer2D::Stats();
        ImGui::Text("Renderer2D:");
        ImGui::Text("\tDraw Calls: %d", stats.drawCalls);
        ImGui::Text("\tQuads: %d", stats.quadCount);
        ImGui::Text("\tVertices: %d", stats.totalVertexCount());
        ImGui::Text("\tIndices: %d", stats.totalIndexCount());
    }
    ImGui::End();
}
}
