#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Sandbox2D::onAttach() {
	PM_PROFILE_FUNCTION();

	_checkerboardTexture = PepperMint::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach() {
	PM_PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(PepperMint::Timestep iTimestep) {
	PM_PROFILE_FUNCTION();

	// Update
	_cameraController.onUpdate(iTimestep);

	// Statistics
	PepperMint::Renderer2D::ResetStats();

	// Render
	{
		PM_PROFILE_SCOPE("Renderer Preparation");
		PepperMint::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		PepperMint::RenderCommand::Clear();
	}

	{
		PM_PROFILE_SCOPE("Renderer Draw");

		static float rotation = 0.0f;
		rotation += iTimestep * 50.0f;

		PepperMint::Renderer2D::BeginScene(_cameraController.camera());
		PepperMint::Renderer2D::DrawQuad({ 1.0f, 0.0f }, -45.0f, { 0.8f, 0.8f }, 1.0f, nullptr, _squareColor);
		PepperMint::Renderer2D::DrawQuad({ 0.5f, -0.5f }, 0.0f, { 0.5f, 0.75f }, 1.0f, nullptr, { 0.8f, 0.3f, 0.8f, 1.0f });
		PepperMint::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, 0.0f, { 20.0f, 20.0f }, 10.0f, _checkerboardTexture);
		PepperMint::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, rotation, { 1.0f, 1.0f }, 20.0f, _checkerboardTexture);
		PepperMint::Renderer2D::EndScene();

		PepperMint::Renderer2D::BeginScene(_cameraController.camera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				PepperMint::Renderer2D::DrawQuad({ x, y }, 0.0f, { 0.45f, 0.45f }, 1.0f, nullptr, color);
			}
		}
		PepperMint::Renderer2D::EndScene();
	}
}

void Sandbox2D::onImGuiRender() {
	PM_PROFILE_FUNCTION();

	// Note: Switch this to true to enable dockspace
	static bool dockingEnabled = true;
	if (dockingEnabled) {
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, 
		// so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
			window_flags |= ImGuiWindowFlags_NoBackground;
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen) {
			ImGui::PopStyleVar(2);
		}

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) PepperMint::Application::Get().close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto&& stats = PepperMint::Renderer2D::Stats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.totalVertexCount());
		ImGui::Text("Indices: %d", stats.totalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(_squareColor));

		uint32_t textureID = _checkerboardTexture->rendererId();
		ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f });

		ImGui::End();

		ImGui::End();
	} else {
		ImGui::Begin("Settings");

		auto&& stats = PepperMint::Renderer2D::Stats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.totalVertexCount());
		ImGui::Text("Indices: %d", stats.totalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(_squareColor));

		uint32_t textureID = _checkerboardTexture->rendererId();
		ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f });

		ImGui::End();
	}
}

void Sandbox2D::onEvent(PepperMint::Event& iEvent) {
	_cameraController.onEvent(iEvent);
}
