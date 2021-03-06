#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <ImGuizmo.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <PepperMint/Math/Math.h>
#include <PepperMint/Scene/SceneSerializer.h>
#include <PepperMint/Utils/PlatformUtils.h>

namespace PepperMint {

// Constants
const float kMIN_PANEL_WIDTH = 370.0f;

void EditorLayer::onAttach() {
    PM_PROFILE_FUNCTION();

    _activeScene = CreateRef<Scene>();

    FrameBufferProperties properties;
    properties.width       = 1280;
    properties.height      = 720;
    properties.attachments = {
        FrameBufferTextureFormat::RGBA8,       // Color
        FrameBufferTextureFormat::RED_INTEGER, // Entity Id
        FrameBufferTextureFormat::DEPTH        // Depth
    };
    _frameBuffer = FrameBuffer::Create(properties);

    _viewportPanel.editorCamera() = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
    _viewportPanel.setFrameBuffer(_frameBuffer);
    _viewportPanel.setActiveScene(_activeScene);
    _sceneHierarchyPanel.setContext(_activeScene);
}

void EditorLayer::onDetach() { PM_PROFILE_FUNCTION(); }

void EditorLayer::onUpdate(Timestep iTimestep) {
    PM_PROFILE_FUNCTION();

    // Resize
    FrameBufferProperties spec         = _frameBuffer->properties();
    auto&&                viewportSize = _viewportPanel.viewportSize();
    if (viewportSize.x > 0.0f && viewportSize.y > 0.0f && // zero sized framebuffer is invalid
        (spec.width != viewportSize.x || spec.height != viewportSize.y)) {
        _frameBuffer->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        _viewportPanel.editorCamera().setViewportSize(viewportSize.x, -viewportSize.y);
        _activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
    }

    // Update
    if (!_playing) {
        _viewportPanel.editorCamera().onUpdate(iTimestep);
    }

    // Statistics
    Renderer2D::ResetStats();

    // Render
    {
        PM_PROFILE_SCOPE("Renderer Preparation");

        _frameBuffer->bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

        // Clear Entity Id attachment to -1
        _frameBuffer->clearAttachment(1, -1);
    }

    {
        PM_PROFILE_SCOPE("Renderer Draw");

        if (_playing) {
            _activeScene->onUpdateRuntime(iTimestep);
        } else {
            _activeScene->onUpdateEditor(iTimestep, _viewportPanel.editorCamera());
        }

        _viewportPanel.onUpdate(iTimestep);

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
            if (ImGui::MenuItem("New", "Ctrl+N")) {
                newScene();
            }

            if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                openScene();
            }

            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                saveScene();
            }

            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
                saveSceneAs();
            }

            if (ImGui::MenuItem("Exit")) {
                Application::Get().close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Scene Hierarchy Panel
    _sceneHierarchyPanel.setHoveredEntity(_viewportPanel.hoveredEntity());
    _sceneHierarchyPanel.onImGuiRender();

    // Properties Panel
    _propertiesPanel.setSelectedEntity(_sceneHierarchyPanel.selectedEntity());
    _propertiesPanel.onImGuiRender();

    // Statistics Panel
    _statisticsPanel.setHoveredEntity(_viewportPanel.hoveredEntity());
    _statisticsPanel.onImGuiRender();

    // Viewport
    _viewportPanel.setSelectedEntity(_sceneHierarchyPanel.selectedEntity());
    _viewportPanel.setEditorMode(!_playing);
    _viewportPanel.onImGuiRender();

    ImGui::End();
}

void EditorLayer::onEvent(Event& iEvent) {
    _viewportPanel.editorCamera().onEvent(iEvent);

    EventDispatcher dispatcher(iEvent);
    dispatcher.dispatch<KeyPressedEvent>(PM_BIND_EVENT_FN(EditorLayer::onKeyPressed));
}

bool EditorLayer::onKeyPressed(KeyPressedEvent& iEvent) {
    // Shorcuts
    if (iEvent.repeatCount() > 0) {
        return false;
    }

    bool control = Input::IsKeyPressed(Key::LEFT_CONTROL) || Input::IsKeyPressed(Key::RIGHT_CONTROL);
    bool shift   = Input::IsKeyPressed(Key::LEFT_SHIFT) || Input::IsKeyPressed(Key::RIGHT_SHIFT);

    switch (iEvent.keyCode()) {
        case Key::N: {
            if (control) {
                newScene();
            }
            break;
        }
        case Key::O: {
            if (control) {
                openScene();
            }
            break;
        }
        case Key::S: {
            if (control) {
                if (shift) {
                    saveSceneAs();
                } else {
                    saveScene();
                }
            }
            break;
        }

        // Switch between Runtime and Editor cameras
        case Key::P:
            _playing = !_playing;
            break;

        default:
            break;
    }

    _viewportPanel.onKeyPressed(iEvent);

    return true;
}

void EditorLayer::newScene() {
    _activeScene = CreateRef<Scene>();

    auto&& viewportsize = _viewportPanel.viewportSize();
    _activeScene->onViewportResize((uint32_t)viewportsize.x, (uint32_t)viewportsize.y);

    _viewportPanel.setActiveScene(_activeScene);
    _sceneHierarchyPanel.setContext(_activeScene);
    _statisticsPanel.setCurrentFile("");
}

void EditorLayer::openScene() {
    auto&& filepath = FileDialogs::OpenFile("PepperMint Scene (*.pm)\0*.pm\0");
    if (!filepath.empty()) {
        newScene();
        SceneSerializer(_activeScene).deserialize(filepath);
        _statisticsPanel.setCurrentFile(filepath);
    }
}

void EditorLayer::saveScene() {
    if (_statisticsPanel.currentFile().empty()) {
        saveSceneAs();
    } else {
        SceneSerializer(_activeScene).serialize(_statisticsPanel.currentFile());
    }
}

void EditorLayer::saveSceneAs() {
    auto&& filepath = FileDialogs::SaveFile("PepperMint Scene (*.pm)\0*.pm\0");
    if (!filepath.empty()) {
        SceneSerializer(_activeScene).serialize(filepath);
        _statisticsPanel.setCurrentFile(filepath);
    }
}
}
