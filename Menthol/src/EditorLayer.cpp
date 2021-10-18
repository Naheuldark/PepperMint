#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <ImGuizmo.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace PepperMint {

// Constants
const float kMIN_PANEL_WIDTH = 370.0f;

void EditorLayer::onAttach() {
    PM_PROFILE_FUNCTION();

    _editorScene = CreateRef<Scene>("Editor");
    _activeScene = _editorScene;

    auto&& commandLineArgs = Application::Get().commandLineArgs();
    if (commandLineArgs.count > 1) {
        auto&&          sceneFilePath = commandLineArgs[1];
        SceneSerializer serializer(_activeScene);
        serializer.deserialize(sceneFilePath);
        _statisticsPanel.setCurrentFile(sceneFilePath);
    }

    FrameBufferProperties properties;
    properties.width       = Window::sHighDPIScaleFactor * 1280;
    properties.height      = Window::sHighDPIScaleFactor * 720;
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
        _viewportPanel.editorCamera().setViewportSize(viewportSize.x, viewportSize.y);
        _activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
    }

    // Update
    if ((_toolbarPanel.sceneState() == SceneState::EDIT) && _viewportPanel.viewportHovered()) {
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

        switch (_toolbarPanel.sceneState()) {
            case SceneState::EDIT: {
                if (_viewportPanel.viewportHovered()) {
                    _viewportPanel.editorCamera().onUpdate(iTimestep);
                }

                _activeScene->onUpdateEditor(iTimestep, _viewportPanel.editorCamera());
                break;
            }
            case SceneState::PLAY: {
                _activeScene->onUpdateRuntime(iTimestep);
                break;
            }
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
    {
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
        style.WindowMinSize.x = Window::sHighDPIScaleFactor * kMIN_PANEL_WIDTH;

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
        _sceneHierarchyPanel.setSelectedEntity(_viewportPanel.selectedEntity());
        _sceneHierarchyPanel.onImGuiRender();

        // Properties Panel
        _propertiesPanel.setSelectedEntity(_sceneHierarchyPanel.selectedEntity());
        _propertiesPanel.onImGuiRender();

        // Statistics Panel
        _statisticsPanel.setHoveredEntity(_viewportPanel.hoveredEntity());
        _statisticsPanel.onImGuiRender();

        // Viewport
        _viewportPanel.setSelectedEntity(_sceneHierarchyPanel.selectedEntity());
        _viewportPanel.setEditorMode(_toolbarPanel.sceneState() == SceneState::EDIT);
        _viewportPanel.onImGuiRender();

        // Toolbar
        SceneState oldState = _toolbarPanel.sceneState();

        _toolbarPanel.onImGuiRender();

        if ((oldState == SceneState::EDIT) && (_toolbarPanel.sceneState() == SceneState::PLAY)) {
            onScenePlay();
        } else if ((oldState == SceneState::PLAY) && (_toolbarPanel.sceneState() == SceneState::EDIT)) {
            onSceneStop();
        }

        // Content Browser Panel
        _contentBrowserPanel.onImGuiRender();

        if (!_viewportPanel.sceneToOpen().empty()) {
            openScene(_viewportPanel.sceneToOpen());
            _viewportPanel.sceneToOpen().clear();
        }
    }
    ImGui::End();
}

void EditorLayer::onEvent(Event& iEvent) {
    if ((_toolbarPanel.sceneState() == SceneState::EDIT) && _viewportPanel.viewportHovered()) {
        _viewportPanel.editorCamera().onEvent(iEvent);
    }

    EventDispatcher dispatcher(iEvent);
    dispatcher.dispatch<KeyPressedEvent>(PM_BIND_EVENT_FN(EditorLayer::onKeyPressed));
    dispatcher.dispatch<MouseButtonPressedEvent>(PM_BIND_EVENT_FN(EditorLayer::onMouseButtonPressed));
}

bool EditorLayer::onKeyPressed(KeyPressedEvent& iEvent) {
    // Shorcuts
    if (iEvent.repeatCount() > 0) {
        return false;
    }

    bool control = Input::IsKeyPressed(Key::LEFT_CONTROL) || Input::IsKeyPressed(Key::RIGHT_CONTROL);
    bool shift   = Input::IsKeyPressed(Key::LEFT_SHIFT) || Input::IsKeyPressed(Key::RIGHT_SHIFT);

    switch (iEvent.keyCode()) {
        case Key::D: {
            if (control) {
                duplicateSelectedEntity();
            }
            break;
        }

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
        case Key::P: {
            if (_toolbarPanel.sceneState() == SceneState::EDIT) {
                onScenePlay();
            } else if (_toolbarPanel.sceneState() == SceneState::PLAY) {
                onSceneStop();
            }
            break;
        }

        default:
            break;
    }

    _viewportPanel.onKeyPressed(iEvent);

    return true;
}

void EditorLayer::onScenePlay() {
    _toolbarPanel.setSceneState(SceneState::PLAY);

    // Make a copy of the editor scene
    _runtimeScene = Scene::Copy(_editorScene);
    _runtimeScene->setName("Runtime");

    _activeScene = _runtimeScene;
    _activeScene->onRuntimeStart();
}

void EditorLayer::onSceneStop() {
    _toolbarPanel.setSceneState(SceneState::EDIT);

    _activeScene  = _editorScene;
    _runtimeScene = nullptr;
    _activeScene->onRuntimeStop();
}

bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent& iEvent) {
    _viewportPanel.onMouseButtonPressed(iEvent);

    return true;
}

void EditorLayer::duplicateSelectedEntity() {
    if (_toolbarPanel.sceneState() != SceneState::EDIT) {
        return;
    }

    Entity selectedEntity = _sceneHierarchyPanel.selectedEntity();
    if (selectedEntity) {
        _editorScene->duplicateEntity(selectedEntity);
    }
}

void EditorLayer::newScene() {
    _editorScene = CreateRef<Scene>("Editor");
    _activeScene = _editorScene;

    auto&& viewportsize = _viewportPanel.viewportSize();
    _activeScene->onViewportResize((uint32_t)viewportsize.x, (uint32_t)viewportsize.y);

    _viewportPanel.setActiveScene(_activeScene);
    _sceneHierarchyPanel.setContext(_activeScene);
    _statisticsPanel.setCurrentFile("");
}

void EditorLayer::openScene() {
    auto&& filepath = FileDialogs::OpenFile("PepperMint Scene (*.pm)\0*.pm\0");
    if (!filepath.empty()) {
        openScene(filepath);
    }
}

void EditorLayer::openScene(const std::filesystem::path& iPath) {
    if (_toolbarPanel.sceneState() != SceneState::EDIT) {
        onSceneStop();
    }

    if (iPath.extension().string() != ".pm") {
        PM_CORE_WARN("Could not load {0} - not a scene file", iPath.filename().string());
        return;
    }

    _editorScene = CreateRef<Scene>("Editor");
    SceneSerializer serializer(_editorScene);
    if (serializer.deserialize(iPath.string())) {
        _activeScene = _editorScene;

        auto&& viewportsize = _viewportPanel.viewportSize();
        _activeScene->onViewportResize((uint32_t)viewportsize.x, (uint32_t)viewportsize.y);

        _viewportPanel.setActiveScene(_activeScene);
        _sceneHierarchyPanel.setContext(_activeScene);
        _statisticsPanel.setCurrentFile(iPath.string());
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
