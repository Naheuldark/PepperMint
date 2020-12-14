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

    _checkerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

    FrameBufferProperties frameBufferProperties;
    frameBufferProperties.width  = 1280;
    frameBufferProperties.height = 720;
    _frameBuffer                 = FrameBuffer::Create(frameBufferProperties);

    _activeScene  = CreateRef<Scene>();
    _editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0
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
#endif

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
        _editorCamera.setViewportSize(_viewportSize.x, -_viewportSize.y);
        _activeScene->onViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
    }

    // Update
    if (_viewportFocused) {
        _cameraController.onUpdate(iTimestep);
    }
    _editorCamera.onUpdate(iTimestep);

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

        _activeScene->onUpdateEditor(iTimestep, _editorCamera);

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
    _sceneHierarchyPanel.onImGuiRender();

    // Statistics Panel
    _statisticsPanel.onImGuiRender(_currentFile);

    // Viewport
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");
    {
        _viewportFocused = ImGui::IsWindowFocused();
        _viewportHovered = ImGui::IsWindowHovered();
        Application::Get().imguiLayer()->setBlockEvents(!_viewportFocused && !_viewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        _viewportSize            = {viewportPanelSize.x, viewportPanelSize.y};

        uint64_t textureID = _frameBuffer->colorAttachmentRendererId();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{_viewportSize.x, _viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

        // Gizmos
        Entity selectedEntity = _sceneHierarchyPanel.selectionContext();
        if (selectedEntity && _gizmoType != -1) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            float windowWidth  = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            // Runtime Camera
            // auto&& primaryCamera    = _activeScene->primaryCameraEntity();
            // auto&& cameraProjection = primaryCamera.get<CameraComponent>().camera.projection();
            // auto&& cameraView       = glm::inverse(primaryCamera.get<TransformComponent>().transform());

            // Editor Camera
            auto&& cameraProjection = _editorCamera.projection();
            auto&& cameraView       = _editorCamera.viewMatrix();

            // Entity transform
            auto&&    transformComponent = selectedEntity.get<TransformComponent>();
            glm::mat4 transform          = transformComponent.transform();

            // Snapping (0.5m for translation/scale - 45 degrees for rotation)
            bool  snap      = Input::IsKeyPressed(Key::LEFT_CONTROL);
            float snapValue = 0.5f;
            if (_gizmoType == ImGuizmo::OPERATION::ROTATE) {
                snapValue = 45.0f;
            }

            float snapValues[3] = {snapValue, snapValue, snapValue};

            ImGuizmo::Manipulate(glm::value_ptr(cameraView),
                                 glm::value_ptr(cameraProjection),
                                 (ImGuizmo::OPERATION)_gizmoType,
                                 ImGuizmo::LOCAL,
                                 glm::value_ptr(transform),
                                 nullptr,
                                 snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing()) {
                glm::vec3 translation, rotation, scale;
                Math::decompose(transform, translation, rotation, scale);

                auto&& deltaRotation           = rotation - transformComponent.rotation;
                transformComponent.translation = translation;
                transformComponent.rotation += deltaRotation;
                transformComponent.scale = scale;
            }
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

void EditorLayer::onEvent(Event& iEvent) {
    _cameraController.onEvent(iEvent);
    _editorCamera.onEvent(iEvent);

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

        // Gizmos
        case Key::Q: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = -1;
            }
            break;
        }
        case Key::W: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            break;
        }
        case Key::E: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            break;
        }
        case Key::R: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = ImGuizmo::OPERATION::SCALE;
            }
            break;
        }

        default:
            break;
    }

    return true;
}

void EditorLayer::newScene() {
    _activeScene = CreateRef<Scene>();
    _activeScene->onViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
    _sceneHierarchyPanel.setContext(_activeScene);
    _currentFile = "";
}

void EditorLayer::openScene() {
    auto&& filepath = FileDialogs::OpenFile("PepperMint Scene (*.pm)\0*.pm\0");
    if (!filepath.empty()) {
        newScene();
        SceneSerializer(_activeScene).deserialize(filepath);
        _currentFile = filepath;
    }
}

void EditorLayer::saveScene() {
    if (_currentFile.empty()) {
        saveSceneAs();
    } else {
        SceneSerializer(_activeScene).serialize(_currentFile);
    }
}

void EditorLayer::saveSceneAs() {
    auto&& filepath = FileDialogs::SaveFile("PepperMint Scene (*.pm)\0*.pm\0");
    if (!filepath.empty()) {
        SceneSerializer(_activeScene).serialize(filepath);
        _currentFile = filepath;
    }
}
}
