#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace PepperMint {

// Constants
const float kMIN_PANEL_WIDTH = 370.0f;

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

    auto&& redSquare = _activeScene->createEntity("Red Square");
    redSquare.add<SpriteRendererComponent>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});

    _mainCamera = _activeScene->createEntity("Camera A");
    _mainCamera.add<CameraComponent>();

    _secondCamera                 = _activeScene->createEntity("Camera B");
    auto&& secondCameraComponent  = _secondCamera.add<CameraComponent>();
    secondCameraComponent.primary = false;

    class CameraController : public ScriptableEntity {
      public:
        void onCreate() override {
            auto&& translation = get<TransformComponent>().translation;
            translation.x      = rand() % 10 - 5.0f;
        }

        void onUpdate(Timestep iTimestep) override {
            auto&& translation = get<TransformComponent>().translation;
            float  speed       = 5.0f;

            if (Input::IsKeyPressed(Key::A)) {
                translation.x -= speed * iTimestep;
            } else if (Input::IsKeyPressed(Key::D)) {
                translation.x += speed * iTimestep;
            }

            if (Input::IsKeyPressed(Key::S)) {
                translation.y -= speed * iTimestep;
            } else if (Input::IsKeyPressed(Key::W)) {
                translation.y += speed * iTimestep;
            }
        }
    };
    _mainCamera.add<NativeScriptComponent>().bind<CameraController>();
    _secondCamera.add<NativeScriptComponent>().bind<CameraController>();

    _sceneHierarchyPanel.setContext(_activeScene);
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

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Menthol Application", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen) {
        ImGui::PopStyleVar(2);
    }

    ///////////////
    // DockSpace //
    ///////////////
    ImGuiIO&    io    = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();

    float minWindowSizeX  = style.WindowMinSize.x;
    style.WindowMinSize.x = kMIN_PANEL_WIDTH;

    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWindowSizeX;

    // Menu Bar
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                Application::Get().close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Scene Hierarchy Panel
    _sceneHierarchyPanel.onImGuiRender();

    // Statistics Panel
    _statisticsPanel.onImGuiRender();

    // Viewport
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");
    {
        _viewportFocused = ImGui::IsWindowFocused();
        _viewportHovered = ImGui::IsWindowHovered();
        Application::Get().imguiLayer()->setBlockEvents(!_viewportFocused || !_viewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _viewportSize            = {viewportPanelSize.x, viewportPanelSize.y};

        uint64_t textureID = _frameBuffer->colorAttachmentRendererId();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{_viewportSize.x, _viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
    }
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

void EditorLayer::onEvent(Event& iEvent) { _cameraController.onEvent(iEvent); }
}
