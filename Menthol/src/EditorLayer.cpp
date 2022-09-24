#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <ImGuizmo.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <PepperMint/Scripting/ScriptEngine.h>

#include "EditorLayer.h"
#include "EditorUtils.h"

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace Menthol {

// Constants
const float                 kPADDING         = 8.0f;
const float                 kTHUMBNAIL_SIZE  = 80.0f;
const float                 kMIN_PANEL_WIDTH = 370.0f;
const std::filesystem::path kASSET_PATH      = "assets";

void EditorLayer::onAttach() {
    PM_PROFILE_FUNCTION();

    _currentDirectory = kASSET_PATH;

    _playIcon     = PepperMint::Texture2D::Create("resources/icons/Editor/PlayButton.png");
    _stopIcon     = PepperMint::Texture2D::Create("resources/icons/Editor/StopButton.png");
    _simulateIcon = PepperMint::Texture2D::Create("resources/icons/Editor/SimulateButton.png");

    _directoryIcon = PepperMint::Texture2D::Create("resources/icons/ContentBrowser/DirectoryIcon.png");
    _fileIcon      = PepperMint::Texture2D::Create("resources/icons/ContentBrowser/FileIcon.png");

    PepperMint::FrameBufferProperties properties;
    properties.width       = (uint32_t)PepperMint::Window::sHighDPIScaleFactor * 1280;
    properties.height      = (uint32_t)PepperMint::Window::sHighDPIScaleFactor * 720;
    properties.attachments = {
        PepperMint::FrameBufferTextureFormat::RGBA8,       // Color
        PepperMint::FrameBufferTextureFormat::RED_INTEGER, // Entity Id
        PepperMint::FrameBufferTextureFormat::DEPTH        // Depth
    };
    _frameBuffer = PepperMint::FrameBuffer::Create(properties);

    _editorScene = PepperMint::CreateRef<PepperMint::Scene>("Editor");
    _activeScene = _editorScene;

    auto&& commandLineArgs = PepperMint::Application::Get().specification().commandLineArgs;
    if (commandLineArgs.count > 1) {
        auto&&                      sceneFilePath = commandLineArgs[1];
        PepperMint::SceneSerializer serializer(_activeScene);
        serializer.deserialize(sceneFilePath);
        _scenePath = sceneFilePath;
    }

    _editorCamera = PepperMint::EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

    PepperMint::Renderer2D::SetLineWidth(4.0f);
}

void EditorLayer::onDetach() { PM_PROFILE_FUNCTION(); }

void EditorLayer::onUpdate(PepperMint::Timestep iTimestep) {
    PM_PROFILE_FUNCTION();

    // Resize
    PepperMint::FrameBufferProperties spec = _frameBuffer->properties();
    if (_viewportSize.x > 0.0f && _viewportSize.y > 0.0f && // zero sized framebuffer is invalid
        (spec.width != _viewportSize.x || spec.height != _viewportSize.y)) {
        _frameBuffer->resize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
        _editorCamera.setViewportSize(_viewportSize.x, _viewportSize.y);
        _activeScene->onViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);
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
                _editorCamera.onUpdate(iTimestep);
                _activeScene->onUpdateEditor(iTimestep, _editorCamera);
                break;
            }
            case SceneState::PLAY: {
                _activeScene->onUpdateRuntime(iTimestep);
                break;
            }
            case SceneState::SIMULATE: {
                _editorCamera.onUpdate(iTimestep);
                _activeScene->onUpdateSimulate(iTimestep, _editorCamera);
                break;
            }
        }

        // Mouse picking
        //	(0,0) is the bottom left corner
        //	(_viewportSize.x, _viewportSize.y) is the top right corner
        auto&& [mx, my] = ImGui::GetMousePos();
        mx -= _viewportBounds[0].x;
        my -= _viewportBounds[0].y;
        auto&& viewportSize = _viewportBounds[1] - _viewportBounds[0];
        my                  = viewportSize.y - my;
        int mouseX          = (int)mx;
        int mouseY          = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
            int pixelData  = _frameBuffer->readPixel(1, mouseX, mouseY);
            _hoveredEntity = (pixelData == -1) ? PepperMint::Entity() : PepperMint::Entity((entt::entity)pixelData, _activeScene.get());
        }
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

        UI_Toolbar();
        UI_Statistics();
        UI_Settings();
        UI_Properties();
        UI_SceneHierarchy();
        UI_ContentBrowser();

        // Viewport
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::Begin("Viewport");
        {
            // Bounds (with/without tab)
            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset    = ImGui::GetWindowPos();
            _viewportBounds[0]     = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
            _viewportBounds[1]     = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

            _viewportFocused = ImGui::IsWindowFocused();
            _viewportHovered = ImGui::IsWindowHovered();
            PepperMint::Application::Get().imguiLayer()->setBlockEvents(!_viewportFocused && !_viewportHovered);

            // Size
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            _viewportSize            = {viewportPanelSize.x, viewportPanelSize.y};

            // Viewport rendered image
            auto&& textureId = _frameBuffer->colorAttachmentRendererId();
            ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2{_viewportSize.x, _viewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

            // Drag & Drop
            if (ImGui::BeginDragDropTarget()) {
                if (auto&& payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                    const wchar_t* path = (const wchar_t*)payload->Data;
                    openScene(std::filesystem::path(kASSET_PATH) / path);
                }
                ImGui::EndDragDropTarget();
            }

            // Gizmos
            if (_editorMode && _selectedEntity && _gizmoType != -1) {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();

                ImGuizmo::SetRect(_viewportBounds[0].x,
                                  _viewportBounds[0].y,
                                  _viewportBounds[1].x - _viewportBounds[0].x,
                                  _viewportBounds[1].y - _viewportBounds[0].y);

                // Editor Camera
                auto&& cameraProjection = _editorCamera.projection();
                auto&& cameraView       = _editorCamera.viewMatrix();

                // Entity transform
                auto&&    transformComponent = _selectedEntity.get<PepperMint::TransformComponent>();
                glm::mat4 transform          = transformComponent.transform();

                // Snapping (0.5m for translation/scale - 45 degrees for rotation)
                bool  snap      = PepperMint::Input::IsKeyPressed(PepperMint::Key::LEFT_CONTROL);
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
                    PepperMint::Math::decompose(transform, translation, rotation, scale);

                    auto&& deltaRotation           = rotation - transformComponent.rotation;
                    transformComponent.translation = translation;
                    transformComponent.rotation += deltaRotation;
                    transformComponent.scale = scale;
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
    ImGui::End();
}

void EditorLayer::UI_Toolbar() {
    auto&& colors        = ImGui::GetStyle().Colors;
    auto&& buttonHovered = colors[ImGuiCol_ButtonHovered];
    auto&& buttonActive  = colors[ImGuiCol_ButtonActive];

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

    ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        bool toolbarEnabled = (bool)_activeScene;

        ImVec4 tintColor = ImVec4(1, 1, 1, 1);
        if (!toolbarEnabled) {
            tintColor.w = 0.5f;
        }

        float size = ImGui::GetWindowHeight() - 4.0f;
        {
            PepperMint::Ref<PepperMint::Texture2D> icon =
                (_sceneState == SceneState::EDIT || _sceneState == SceneState::SIMULATE) ? _playIcon : _stopIcon;
            ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
            if (ImGui::ImageButton(
                    (ImTextureID)icon->rendererId(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) &&
                toolbarEnabled) {
                if ((_sceneState == SceneState::EDIT) || (_sceneState == SceneState::SIMULATE)) {
                    onScenePlay();
                } else if (_sceneState == SceneState::PLAY) {
                    onSceneStop();
                }
            }
        }
        ImGui::SameLine();
        {
            PepperMint::Ref<PepperMint::Texture2D> icon =
                (_sceneState == SceneState::EDIT || _sceneState == SceneState::PLAY) ? _simulateIcon : _stopIcon;
            if (ImGui::ImageButton(
                    (ImTextureID)icon->rendererId(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) &&
                toolbarEnabled) {
                if ((_sceneState == SceneState::EDIT) || (_sceneState == SceneState::PLAY)) {
                    onSceneSimulate();
                } else if (_sceneState == SceneState::SIMULATE) {
                    onSceneStop();
                }
            }
        }
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
    }
    ImGui::End();
}

void EditorLayer::UI_Statistics() {
    ImGui::Begin("Statistics");
    {
        std::string currentFile("Current File: ");
        if (_scenePath.empty()) {
            currentFile += "New File";
        } else {
            currentFile += _scenePath.string();
        }
        ImGui::Text(currentFile.c_str());

        ImGui::Separator();

        std::string hoveredEntity("None");
        if (_hoveredEntity) {
            hoveredEntity = _hoveredEntity.get<PepperMint::TagComponent>().tag;
        }
        ImGui::Text("Hovered Entity: %s", hoveredEntity.c_str());

        ImGui::Separator();

        auto&& stats = PepperMint::Renderer2D::Stats();
        ImGui::Text("Renderer2D:");
        ImGui::Text("\t\tDraw Calls: %d", stats.drawCalls);
        ImGui::Text("\t\tQuads: %d", stats.quadCount);
        ImGui::Text("\t\tVertices: %d", stats.totalVertexCount());
        ImGui::Text("\t\tIndices: %d", stats.totalIndexCount());
    }
    ImGui::End();
}

void EditorLayer::UI_Settings() {
    ImGui::Begin("Settings");
    { ImGui::Checkbox("Show physics colliders", &_showPhysicsCollider); }
    ImGui::End();
}

void EditorLayer::UI_Properties() {
    ImGui::Begin("Properties");
    {
        if (_selectedEntity) {
            drawComponents(_selectedEntity);
        }
    }
    ImGui::End();
}

void EditorLayer::UI_SceneHierarchy() {
    auto&& drawEntityNode = [&](PepperMint::Entity iEntity) {
        auto&& tag = iEntity.get<PepperMint::TagComponent>().tag;

        ImGuiTreeNodeFlags commonFlags  = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        ImGuiTreeNodeFlags selectedFlag = ((_selectedEntity == iEntity) ? ImGuiTreeNodeFlags_Selected : 0);

        bool isOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)iEntity, commonFlags | selectedFlag, tag.c_str());

        if (ImGui::IsItemClicked()) {
            _selectedEntity = iEntity;
        }

        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete Entity")) {
                entityDeleted = true;
            }
            ImGui::EndPopup();
        }

        if (isOpened) {
            if (ImGui::TreeNodeEx((void*)9817239, commonFlags, tag.c_str())) {
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (entityDeleted) {
            _activeScene->destroyEntity(iEntity);
            if (_selectedEntity == iEntity) {
                _selectedEntity = {};
            }
        }
    };

    ImGui::Begin("Scene Hierarchy");
    {
        _activeScene->forEachEntity([&](auto&& entity) { drawEntityNode({entity, _activeScene.get()}); });

        // Left-click on blank space
        if (PepperMint::Input::IsMouseButtonPressed(PepperMint::Mouse::BUTTON_LEFT) && ImGui::IsWindowHovered()) {
            _selectedEntity = {};
        }

        // Right-click on blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                _selectedEntity = _activeScene->createEntity("Empty Entity");
            } else if (ImGui::MenuItem("Create Camera")) {
                _selectedEntity = _activeScene->createEntity("Camera");
                _selectedEntity.add<PepperMint::CameraComponent>();
            } else if (ImGui::MenuItem("Create Sprite")) {
                _selectedEntity = _activeScene->createEntity("Sprite");
                _selectedEntity.add<PepperMint::SpriteRendererComponent>();
            } else if (ImGui::MenuItem("Create Circle")) {
                _selectedEntity = _activeScene->createEntity("Circle");
                _selectedEntity.add<PepperMint::CircleRendererComponent>();
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void EditorLayer::UI_ContentBrowser() {
    ImGui::Begin("Content Browser");
    {
        if (_currentDirectory != std::filesystem::path(kASSET_PATH)) {
            if (ImGui::Button("<-")) {
                _currentDirectory = _currentDirectory.parent_path();
            }
        }

        float cellSize = PepperMint::Window::sHighDPIScaleFactor * kTHUMBNAIL_SIZE + PepperMint::Window::sHighDPIScaleFactor * kPADDING;

        float panelWidth  = ImGui::GetContentRegionAvail().x;
        int   columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1) {
            columnCount = 1;
        }

        ImGui::Columns(columnCount, 0, false);

        for (auto&& directoryEntry : std::filesystem::directory_iterator(_currentDirectory)) {
            auto&& path           = directoryEntry.path();
            auto&& filenameString = path.filename().string();

            ImGui::PushID(filenameString.c_str());

            PepperMint::Ref<PepperMint::Texture2D> icon = directoryEntry.is_directory() ? _directoryIcon : _fileIcon;

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::ImageButton((ImTextureID)icon->rendererId(),
                               {PepperMint::Window::sHighDPIScaleFactor * kTHUMBNAIL_SIZE, PepperMint::Window::sHighDPIScaleFactor * kTHUMBNAIL_SIZE},
                               {0, 1},
                               {1, 0});
            if (ImGui::BeginDragDropSource()) {
                auto&&         relativePath = std::filesystem::relative(path, kASSET_PATH);
                const wchar_t* itemPath     = relativePath.c_str();

                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
                ImGui::EndDragDropSource();
            }
            ImGui::PopStyleColor();

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                if (directoryEntry.is_directory()) {
                    _currentDirectory /= path.filename();
                }
            }
            ImGui::TextWrapped(filenameString.c_str());

            ImGui::NextColumn();

            ImGui::PopID();
        }

        ImGui::Columns(1);
    }
    ImGui::End();
}

void EditorLayer::onOverlayRender() {
    if (_sceneState == SceneState::PLAY) {
        auto&& camera = _activeScene->primaryCameraEntity();
        if (!camera)
            return;

        PepperMint::Renderer2D::BeginScene(camera.get<PepperMint::CameraComponent>().camera,
                                           camera.get<PepperMint::TransformComponent>().transform());
    } else {
        PepperMint::Renderer2D::BeginScene(_editorCamera);
    }

    if (_showPhysicsCollider) {
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

    // Selected Entity
    {
        if (_selectedEntity) {
            auto&& transformComponent = _selectedEntity.get<PepperMint::TransformComponent>();
            PepperMint::Renderer2D::DrawRect(transformComponent.transform(), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
        }
    }

    PepperMint::Renderer2D::EndScene();
}

void EditorLayer::onEvent(PepperMint::Event& iEvent) {
    if (_sceneState == SceneState::EDIT && _viewportHovered) {
        _editorCamera.onEvent(iEvent);
    }

    PepperMint::EventDispatcher dispatcher(iEvent);
    dispatcher.dispatch<PepperMint::KeyPressedEvent>(PM_BIND_EVENT_FN(EditorLayer::onKeyPressed));
    dispatcher.dispatch<PepperMint::MouseButtonPressedEvent>(PM_BIND_EVENT_FN(EditorLayer::onMouseButtonPressed));
}

bool EditorLayer::onKeyPressed(PepperMint::KeyPressedEvent& iEvent) {
    // Shorcuts
    if (iEvent.isRepeat()) {
        return false;
    }

    bool control = PepperMint::Input::IsKeyPressed(PepperMint::Key::LEFT_CONTROL) || PepperMint::Input::IsKeyPressed(PepperMint::Key::RIGHT_CONTROL);
    bool shift   = PepperMint::Input::IsKeyPressed(PepperMint::Key::LEFT_SHIFT) || PepperMint::Input::IsKeyPressed(PepperMint::Key::RIGHT_SHIFT);

    switch (iEvent.keyCode()) {
        // Scene commands
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
            if (!control && ((_sceneState == SceneState::EDIT) || (_sceneState == SceneState::SIMULATE))) {
                onScenePlay();
            } else if (control && ((_sceneState == SceneState::EDIT) || (_sceneState == SceneState::PLAY))) {
                onSceneSimulate();
            } else if ((_sceneState == SceneState::PLAY) || (_sceneState == SceneState::SIMULATE)) {
                onSceneStop();
            }
            break;
        }

        // Gizmos
        case PepperMint::Key::Q: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = -1;
            }
            break;
        }
        case PepperMint::Key::W: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            }
            break;
        }
        case PepperMint::Key::E: {
            if (!ImGuizmo::IsUsing()) {
                _gizmoType = ImGuizmo::OPERATION::ROTATE;
            }
            break;
        }
        case PepperMint::Key::R: {
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

bool EditorLayer::onMouseButtonPressed(PepperMint::MouseButtonPressedEvent& iEvent) {
    if (iEvent.mouseButton() == PepperMint::Mouse::BUTTON_LEFT) {
        if (_viewportHovered && !ImGuizmo::IsOver() && !PepperMint::Input::IsKeyPressed(PepperMint::Key::LEFT_ALT)) {
            _selectedEntity = _hoveredEntity;
        }
    }

    return true;
}

void EditorLayer::onScenePlay() {
    if (_sceneState == SceneState::SIMULATE) {
        onSceneStop();
    }

    _sceneState = SceneState::PLAY;

    // Save potential selection
    PepperMint::UUID selectedEntityUUID = _selectedEntity ? _selectedEntity.uuid() : PepperMint::UUID();

    // Make a copy of the editor scene
    _activeScene = PepperMint::Scene::Copy(_editorScene);
    _activeScene->setName("Runtime");

    // Start runtime and restore selection
    _activeScene->onRuntimeStart();
    _selectedEntity = (selectedEntityUUID != 0) ? _activeScene->getEntityByUUID(selectedEntityUUID) : PepperMint::Entity();
}

void EditorLayer::onSceneSimulate() {
    if (_sceneState == SceneState::PLAY) {
        onSceneStop();
    }

    _sceneState = SceneState::SIMULATE;

    // Save potential selection
    PepperMint::UUID selectedEntityUUID = _selectedEntity ? _selectedEntity.uuid() : PepperMint::UUID();

    // Make a copy of the editor scene
    _activeScene = PepperMint::Scene::Copy(_editorScene);
    _activeScene->setName("Simulate");

    // Start simulation and restore selection
    _activeScene->onSimulateStart();
    _selectedEntity = (selectedEntityUUID != 0) ? _activeScene->getEntityByUUID(selectedEntityUUID) : PepperMint::Entity();
}

void EditorLayer::onSceneStop() {
    PM_CORE_ASSERT((_sceneState == SceneState::PLAY) || (_sceneState == SceneState::SIMULATE));

    if (_sceneState == SceneState::PLAY) {
        _activeScene->onRuntimeStop();
    } else if (_sceneState == SceneState::SIMULATE) {
        _activeScene->onSimulateStop();
    }

    _sceneState = SceneState::EDIT;

    // Save potential selection
    PepperMint::UUID selectedEntityUUID = _selectedEntity ? _selectedEntity.uuid() : PepperMint::UUID();

    // Switch active scene
    _activeScene    = _editorScene;
    _selectedEntity = (selectedEntityUUID != 0) ? _activeScene->getEntityByUUID(selectedEntityUUID) : PepperMint::Entity();
}

void EditorLayer::duplicateSelectedEntity() {
    if (_sceneState != SceneState::EDIT) {
        return;
    }

    if (_selectedEntity) {
        _editorScene->duplicateEntity(_selectedEntity);
    }
}

void EditorLayer::newScene() {
    _editorScene = PepperMint::CreateRef<PepperMint::Scene>("Editor");
    _activeScene = _editorScene;

    _activeScene->onViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);

    _selectedEntity = {};
    _scenePath      = std::filesystem::path();
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

        _activeScene->onViewportResize((uint32_t)_viewportSize.x, (uint32_t)_viewportSize.y);

        _selectedEntity = {};
        _scenePath      = iPath;
    }
}

void EditorLayer::saveScene() {
    if (_scenePath.empty()) {
        saveSceneAs();
    } else {
        PepperMint::SceneSerializer(_activeScene).serialize(_scenePath.string());
    }
}

void EditorLayer::saveSceneAs() {
    auto&& filepath = PepperMint::FileDialogs::SaveFile("PepperMint Scene (*.pm)\0*.pm\0");
    if (!filepath.empty()) {
        PepperMint::SceneSerializer(_activeScene).serialize(filepath);
        _scenePath = filepath;
    }
}
}
