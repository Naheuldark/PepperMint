#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace PepperMint;

namespace Menthol {

void EditorLayer::onAttach() {
    PM_PROFILE_FUNCTION();

    _checkerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

    FrameBufferProperties frameBufferProperties;
    frameBufferProperties.width  = 1280;
    frameBufferProperties.height = 720;
    _frameBuffer                 = FrameBuffer::Create(frameBufferProperties);

    _activeScene = CreateRef<Scene>();

    _squareEntity = _activeScene->createEntity("Green Square");
    _squareEntity.add<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

    _mainCamera = _activeScene->createEntity("Camera");
    _mainCamera.add<CameraComponent>();

    _secondCamera                 = _activeScene->createEntity("Clip-Space");
    auto&& secondCameraComponent  = _secondCamera.add<CameraComponent>();
    secondCameraComponent.primary = false;
}

void EditorLayer::onDetach() { PM_PROFILE_FUNCTION(); }

void EditorLayer::onUpdate(Timestep iTimestep) {
    PM_PROFILE_FUNCTION();

    // Resize
    FrameBufferProperties spec = _frameBuffer->properties();
    if (_viewportSize.x > 0.0f && _viewportSize.y > 0.0f && // zero sized framebuffer is invalid
        (spec.width != _viewportSize.x || spec.height != _viewportSize.y)) {
        _frameBuffer->resize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
        _cameraController.onResize(_viewportSize.x, _viewportSize.y);
        _activeScene->onViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
    }

    // Update
    if (_viewportFocused) {
        _cameraController.onUpdate(iTimestep);
    }

    // Statistics
    Renderer2D::ResetStats();

    // Render
    {
        PM_PROFILE_SCOPE("Renderer Preparation");

        _frameBuffer->bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();
    }

    {
        PM_PROFILE_SCOPE("Renderer Draw");

        _activeScene->onUpdate(iTimestep);

        _frameBuffer->unbind();
    }
}

void EditorLayer::onImGuiRender() {
    PM_PROFILE_FUNCTION();

    static bool               dockspaceOpen             = true;
    static bool               opt_fullscreen_persistant = true;
    bool                      opt_fullscreen            = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags           = ImGuiDockNodeFlags_None;

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

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and
    // handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking,
    // otherwise any change of dockspace/settings would lead to windows being stuck in limbo and
    // never being visible.
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
            // Disabling fullscreen would allow the window to be moved to the front of other
            // windows, which we can't undo at the moment without finer window depth/z control.
            // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
            if (ImGui::MenuItem("Exit")) {
                Application::Get().close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Begin("Settings");
    {
        auto&& stats = Renderer2D::Stats();
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.totalVertexCount());
        ImGui::Text("Indices: %d", stats.totalIndexCount());

        if (_squareEntity) {
            ImGui::Separator();

            auto&& tag = _squareEntity.get<TagComponent>().tag;
            ImGui::Text("%s", tag.c_str());

            auto&& squareColor = _squareEntity.get<SpriteRendererComponent>().color;
            ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));

            ImGui::Separator();
        }

        ImGui::DragFloat3("Camera Transform", glm::value_ptr(_mainCamera.get<TransformComponent>().transform[3]));

        if (ImGui::Checkbox("Camera A", &_primaryCamera)) {
            _mainCamera.get<CameraComponent>().primary   = _primaryCamera;
            _secondCamera.get<CameraComponent>().primary = !_primaryCamera;
        }

        ImGui::Separator();

        auto&& secondCamera = _secondCamera.get<CameraComponent>().camera;
        float  orthoSize    = secondCamera.orthographicSize();
        if (ImGui::DragFloat("Second Camera Orthographic Size", &orthoSize)) {
            secondCamera.setOrthographicSize(orthoSize);
        }
    }
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");
    {
        _viewportFocused = ImGui::IsWindowFocused();
        _viewportHovered = ImGui::IsWindowHovered();
        Application::Get().imguiLayer()->setBlockEvents(!_viewportFocused || !_viewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _viewportSize            = {viewportPanelSize.x, viewportPanelSize.y};

        uint32_t textureID = _frameBuffer->colorAttachmentRendererId();
        ImGui::Image((void*)textureID, ImVec2{_viewportSize.x, _viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
    }
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

void EditorLayer::onEvent(Event& iEvent) { _cameraController.onEvent(iEvent); }

}
