#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Sandbox2D::onAttach() {
    PM_PROFILE_FUNCTION();

    _checkerboardTexture = PepperMint::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach() { PM_PROFILE_FUNCTION(); }

void Sandbox2D::onUpdate(PepperMint::Timestep iTimestep) {
    PM_PROFILE_FUNCTION();

    // Update
    _cameraController.onUpdate(iTimestep);

    // Statistics
    PepperMint::Renderer2D::ResetStats();

    // Render
    {
        PM_PROFILE_SCOPE("Renderer Preparation");

        PepperMint::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        PepperMint::RenderCommand::Clear();
    }

    {
        PM_PROFILE_SCOPE("Renderer Draw");

        static float rotation = 0.0f;
        rotation += iTimestep * 50.0f;

        PepperMint::Renderer2D::BeginScene(_cameraController.camera());
        PepperMint::Renderer2D::DrawQuad(
            {1.0f, 0.0f}, -45.0f, {0.8f, 0.8f}, 1.0f, nullptr, _squareColor);
        PepperMint::Renderer2D::DrawQuad(
            {0.5f, -0.5f}, 0.0f, {0.5f, 0.75f}, 1.0f, nullptr, {0.8f, 0.3f, 0.8f, 1.0f});
        PepperMint::Renderer2D::DrawQuad(
            {0.0f, 0.0f, -0.1f}, 0.0f, {20.0f, 20.0f}, 10.0f, _checkerboardTexture);
        PepperMint::Renderer2D::DrawQuad(
            {-1.0f, 0.0f, 0.0f}, rotation, {1.0f, 1.0f}, 20.0f, _checkerboardTexture);
        PepperMint::Renderer2D::EndScene();

        PepperMint::Renderer2D::BeginScene(_cameraController.camera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f) {
            for (float x = -5.0f; x < 5.0f; x += 0.5f) {
                glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f};
                PepperMint::Renderer2D::DrawQuad(
                    {x, y}, 0.0f, {0.45f, 0.45f}, 1.0f, nullptr, color);
            }
        }
        PepperMint::Renderer2D::EndScene();
    }
}

void Sandbox2D::onImGuiRender() {
    PM_PROFILE_FUNCTION();

    ImGui::Begin("Settings");

    auto&& stats = PepperMint::Renderer2D::Stats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.drawCalls);
    ImGui::Text("Quads: %d", stats.quadCount);
    ImGui::Text("Vertices: %d", stats.totalVertexCount());
    ImGui::Text("Indices: %d", stats.totalIndexCount());

    ImGui::ColorEdit4("Square Color", glm::value_ptr(_squareColor));

    ImGui::End();
}

void Sandbox2D::onEvent(PepperMint::Event& iEvent) { _cameraController.onEvent(iEvent); }
