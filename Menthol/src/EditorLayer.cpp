#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <ImGuizmo.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Menthol {

// Constants
const float kMIN_PANEL_WIDTH = 370.0f;

void EditorLayer::onAttach() {
    PM_PROFILE_FUNCTION();

    _editorScene = PepperMint::CreateRef<PepperMint::Scene>("Editor");
    _activeScene = _editorScene;

    auto&& commandLineArgs = PepperMint::Application::Get().commandLineArgs();
    if (commandLineArgs.count > 1) {
        auto&&                      sceneFilePath = commandLineArgs[1];
        PepperMint::SceneSerializer serializer(_activeScene);
        serializer.deserialize(sceneFilePath);
        _statisticsPanel.setCurrentFile(sceneFilePath);
    }

    PepperMint::FrameBufferProperties properties;
    properties.width       = PepperMint::Window::sHighDPIScaleFactor * 1280;
    properties.height      = PepperMint::Window::sHighDPIScaleFactor * 720;
    properties.attachments = {
        PepperMint::FrameBufferTextureFormat::RGBA8,       // Color
        PepperMint::FrameBufferTextureFormat::RED_INTEGER, // Entity Id
        PepperMint::FrameBufferTextureFormat::DEPTH        // Depth
    };
    _frameBuffer = PepperMint::FrameBuffer::Create(properties);

    _viewportPanel.editorCamera() = PepperMint::EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
    _viewportPanel.setFrameBuffer(_frameBuffer);
    _viewportPanel.setActiveScene(_activeScene);
    _sceneHierarchyPanel.setContext(_activeScene);
}

void EditorLayer::onDetach() { PM_PROFILE_FUNCTION(); }

void EditorLayer::onUpdate(PepperMint::Timestep iTimestep) {
    PM_PROFILE_FUNCTION();

    // Resize
    PepperMint::FrameBufferProperties spec         = _frameBuffer->properties();
    auto&&                            viewportSize = _viewportPanel.viewportSize();
    if (viewportSize.x > 0.0f && viewportSize.y > 0.0f && // zero sized framebuffer is invalid
        (spec.width != viewportSize.x || spec.height != viewportSize.y)) {
        _frameBuffer->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
        _viewportPanel.editorCamera().setViewportSize(viewportSize.x, viewportSize.y);
        _activeScene->onViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
    }

    PepperMint::Renderer2D::ResetStats();

    _frameBuffer->bind();

    {
        PM_PROFILE_SCOPE("Renderer Preparation");

        PepperMint::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        PepperMint::RenderCommand::Clear();

        // Clear Entity Id attachment to -1
        _frameBuffer->clearAttachment(1, -1);
    }

    {
        PM_PROFILE_SCOPE("Renderer Draw");

        switch (_sceneState) {
            case SceneState::EDIT: {
                _viewportPanel.editorCamera().onUpdate(iTimestep);

                _activeScene->onUpdateEditor(iTimestep, _viewportPanel.editorCamera());
                break;
            }
            case SceneState::PLAY: {
                _activeScene->onUpdateRuntime(iTimestep);
                break;
            }
        }

        _viewportPanel.onUpdate(iTimestep);
    }

    onOverlayRender();

    _frameBuffer->unbind();
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
        style.WindowMinSize.x = PepperMint::Window::sHighDPIScaleFactor * kMIN_PANEL_WIDTH;

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
                    PepperMint::Application::Get().close();
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
        _viewportPanel.setEditorMode(_sceneState == SceneState::EDIT);
        _viewportPanel.onImGuiRender();

        // Toolbar
        _toolbarPanel.setSceneState(_sceneState);
        _toolbarPanel.onImGuiRender();

        if ((_sceneState == SceneState::EDIT) && _toolbarPanel.playButtonClicked()) {
            onScenePlay();
        } else if ((_sceneState == SceneState::PLAY) && _toolbarPanel.playButtonClicked()) {
            onSceneStop();
        }

        // Content Browser Panel
        _contentBrowserPanel.onImGuiRender();

        if (!_viewportPanel.sceneToOpen().empty()) {
            openScene(_viewportPanel.sceneToOpen());
            _viewportPanel.sceneToOpen().clear();
        }

        // Settings Panel
        _settingsPanel.onImGuiRender();
    }
    ImGui::End();
}

void EditorLayer::onOverlayRender() {
    if (_sceneState == SceneState::PLAY) {
        auto&& camera = _activeScene->primaryCameraEntity();
        PepperMint::Renderer2D::BeginScene(camera.get<PepperMint::CameraComponent>().camera,
                                           camera.get<PepperMint::TransformComponent>().transform());
    } else {
        PepperMint::Renderer2D::BeginScene(_viewportPanel.editorCamera());
    }

    if (_settingsPanel.showPhysicsCollider()) {
        // Box Colliders
        {
            auto&& view = _activeScene->getAllEntitiesWith<PepperMint::TransformComponent, PepperMint::BoxCollider2DComponent>();
            for (auto&& entity : view) {
                auto [transformComponent, boxColliderComponent] =
                    view.get<PepperMint::TransformComponent, PepperMint::BoxCollider2DComponent>(entity);

                auto&& translation = transformComponent.translation + glm::vec3(boxColliderComponent.offset, 0.001f);
                auto&& scale       = transformComponent.scale * glm::vec3(boxColliderComponent.size * 2.0f, 1.0f);
                auto&& transform   = glm::translate(glm::mat4(1.0f), translation) *
                                   glm::rotate(glm::mat4(1.0f), transformComponent.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
                                   glm::scale(glm::mat4(1.0f), scale);

                PepperMint::Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
            }
        }

        // Circle Colliders
        {
            auto&& view = _activeScene->getAllEntitiesWith<PepperMint::TransformComponent, PepperMint::CircleCollider2DComponent>();
            for (auto&& entity : view) {
                auto [transformComponent, circleColliderComponent] =
                    view.get<PepperMint::TransformComponent, PepperMint::CircleCollider2DComponent>(entity);

                auto&& translation = transformComponent.translation + glm::vec3(circleColliderComponent.offset, 0.001f);
                auto&& scale       = transformComponent.scale * glm::vec3(circleColliderComponent.radius * 2.0f);
                auto&& transform   = glm::translate(glm::mat4(1.0f), translation) * glm::scale(glm::mat4(1.0f), scale);

                PepperMint::Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.01f);
            }
        }
    }

    PepperMint::Renderer2D::EndScene();
}

void EditorLayer::onEvent(PepperMint::Event& iEvent) {
    if ((_sceneState == SceneState::EDIT) && _viewportPanel.viewportHovered()) {
        _viewportPanel.editorCamera().onEvent(iEvent);
    }

    PepperMint::EventDispatcher dispatcher(iEvent);
    dispatcher.dispatch<PepperMint::KeyPressedEvent>(PM_BIND_EVENT_FN(EditorLayer::onKeyPressed));
    dispatcher.dispatch<PepperMint::MouseButtonPressedEvent>(PM_BIND_EVENT_FN(EditorLayer::onMouseButtonPressed));
}

bool EditorLayer::onKeyPressed(PepperMint::KeyPressedEvent& iEvent) {
    // Shorcuts
    if (iEvent.repeatCount() > 0) {
        return false;
    }

    bool control = PepperMint::Input::IsKeyPressed(PepperMint::Key::LEFT_CONTROL) || PepperMint::Input::IsKeyPressed(PepperMint::Key::RIGHT_CONTROL);
    bool shift   = PepperMint::Input::IsKeyPressed(PepperMint::Key::LEFT_SHIFT) || PepperMint::Input::IsKeyPressed(PepperMint::Key::RIGHT_SHIFT);

    switch (iEvent.keyCode()) {
        case PepperMint::Key::D: {
            if (control) {
                duplicateSelectedEntity();
            }
            break;
        }

        case PepperMint::Key::N: {
            if (control) {
                newScene();
            }
            break;
        }
        case PepperMint::Key::O: {
            if (control) {
                openScene();
            }
            break;
        }
        case PepperMint::Key::S: {
            if (control) {
                if (shift) {
                    saveSceneAs();
                } else {
                    saveScene();
                }
            }
            break;
        }

        // Switch between Runtime and Editor scene
        case PepperMint::Key::P: {
            if (_sceneState == SceneState::EDIT) {
                onScenePlay();
            } else if (_sceneState == SceneState::PLAY) {
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
    _sceneState = SceneState::PLAY;

    // Make a copy of the editor scene
    _runtimeScene = PepperMint::Scene::Copy(_editorScene);
    _runtimeScene->setName("Runtime");

    // Switch active scene
    _activeScene = _runtimeScene;
    _activeScene->onRuntimeStart();
}

void EditorLayer::onSceneStop() {
    _sceneState = SceneState::EDIT;

    // Switch active scene
    _activeScene  = _editorScene;
    _runtimeScene = nullptr;
    _activeScene->onRuntimeStop();
}

bool EditorLayer::onMouseButtonPressed(PepperMint::MouseButtonPressedEvent& iEvent) {
    _viewportPanel.onMouseButtonPressed(iEvent);

    return true;
}

void EditorLayer::duplicateSelectedEntity() {
    if (_sceneState != SceneState::EDIT) {
        return;
    }

    PepperMint::Entity selectedEntity = _sceneHierarchyPanel.selectedEntity();
    if (selectedEntity) {
        _editorScene->duplicateEntity(selectedEntity);
    }
}

void EditorLayer::newScene() {
    _editorScene = PepperMint::CreateRef<PepperMint::Scene>("Editor");
    _activeScene = _editorScene;

    auto&& viewportsize = _viewportPanel.viewportSize();
    _activeScene->onViewportResize((uint32_t)viewportsize.x, (uint32_t)viewportsize.y);

    _viewportPanel.setActiveScene(_activeScene);
    _sceneHierarchyPanel.setContext(_activeScene);
    _statisticsPanel.setCurrentFile("");
}

void EditorLayer::openScene() {
    auto&& filepath = PepperMint::FileDialogs::OpenFile("PepperMint Scene (*.pm)\0*.pm\0");
    if (!filepath.empty()) {
        openScene(filepath);
    }
}

void EditorLayer::openScene(const std::filesystem::path& iPath) {
    if (_sceneState != SceneState::EDIT) {
        onSceneStop();
    }

    if (iPath.extension().string() != ".pm") {
        PM_CORE_WARN("Could not load {0} - not a scene file", iPath.filename().string());
        return;
    }

    _editorScene = PepperMint::CreateRef<PepperMint::Scene>("Editor");
    PepperMint::SceneSerializer serializer(_editorScene);
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
        PepperMint::SceneSerializer(_activeScene).serialize(_statisticsPanel.currentFile());
    }
}

void EditorLayer::saveSceneAs() {
    auto&& filepath = PepperMint::FileDialogs::SaveFile("PepperMint Scene (*.pm)\0*.pm\0");
    if (!filepath.empty()) {
        PepperMint::SceneSerializer(_activeScene).serialize(filepath);
        _statisticsPanel.setCurrentFile(filepath);
    }
}
}
