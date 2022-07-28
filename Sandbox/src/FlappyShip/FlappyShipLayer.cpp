#include "FlappyShipLayer.h"
#include "Random.h"

const float kCameraHeight = 8.0f;

FlappyShipLayer::FlappyShipLayer() : Layer("FlappyShip") {
    auto& window = PepperMint::Application::Get().window();
    updateCamera(window.width(), window.height());

    Random::Init();
}

void FlappyShipLayer::onAttach() {
    _level.init();

    ImGuiIO io = ImGui::GetIO();
    _font      = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", 120.0f);
}

void FlappyShipLayer::onDetach() {}

void FlappyShipLayer::onUpdate(PepperMint::Timestep iTimestep) {
    _time += iTimestep;

    // Blinking Menu
    if ((int)(_time * 100.0f) % 10 > 5) {
        _blink = !_blink;
    }

    // Game status
    if (_level.gameOver()) {
        _state = GameState::GAME_OVER;
    }

    // Update
    auto&& playerPos = _level.player().position();
    _camera->setPosition({playerPos.x, playerPos.y, 0.0f});

    switch (_state) {
        case GameState::PLAY:
            _level.onUpdate(iTimestep);
            break;
        default:
            break;
    }

    // Render
    PepperMint::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    PepperMint::RenderCommand::Clear();

    PepperMint::Renderer2D::BeginScene(*_camera);
    _level.onRender();
    PepperMint::Renderer2D::EndScene();
}

void FlappyShipLayer::onImGuiRender() {
    // ImGui::Begin("Settings");
    // _level.onImGuiRender();
    // ImGui::End();

    switch (_state) {
        case FlappyShipLayer::GameState::PLAY: {
            std::string playerScore = std::string("Score: ") + std::to_string(_level.player().score());
            ImGui::GetForegroundDrawList()->AddText(_font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, playerScore.c_str());
            break;
        }
        case FlappyShipLayer::GameState::MENU: {
            auto   pos    = ImGui::GetWindowPos();
            auto&& width  = PepperMint::Application::Get().window().width();
            auto&& height = PepperMint::Application::Get().window().height();

            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;

            if (_blink) {
                ImGui::GetForegroundDrawList()->AddText(_font, 120.0f, pos, 0xffffffff, "Click to play!");
            }
            break;
        }
        case FlappyShipLayer::GameState::GAME_OVER: {
            auto   pos    = ImGui::GetWindowPos();
            auto&& width  = PepperMint::Application::Get().window().width();
            auto&& height = PepperMint::Application::Get().window().height();

            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;

            if (_blink) {
                ImGui::GetForegroundDrawList()->AddText(_font, 120.0f, pos, 0xffffffff, "Click to play!");
            }

            pos.x += 200.0f;
            pos.y += 150.0f;
            std::string playerScore = std::string("Score: ") + std::to_string(_level.player().score());
            ImGui::GetForegroundDrawList()->AddText(_font, 48.0f, pos, 0xffffffff, playerScore.c_str());
            break;
        }
        default:
            break;
    }
}

void FlappyShipLayer::onEvent(PepperMint::Event& iEvent) {
    PepperMint::EventDispatcher dispatcher(iEvent);
    dispatcher.dispatch<PepperMint::WindowResizeEvent>(PM_BIND_EVENT_FN(FlappyShipLayer::onWindowResize));
    dispatcher.dispatch<PepperMint::MouseButtonPressedEvent>(PM_BIND_EVENT_FN(FlappyShipLayer::onMouseButtonPressed));
}

bool FlappyShipLayer::onWindowResize(PepperMint::WindowResizeEvent& iEvent) {
    updateCamera(iEvent.width(), iEvent.height());
    return false;
}

bool FlappyShipLayer::onMouseButtonPressed(PepperMint::MouseButtonPressedEvent& iEvent) {
    if (_state == GameState::GAME_OVER) {
        _level.reset();
    }

    _state = GameState::PLAY;

    return false;
}

void FlappyShipLayer::updateCamera(uint32_t iWidth, uint32_t iHeight) {
    float aspectRatio = (float)iWidth / (float)iHeight;

    float bottom = -kCameraHeight;
    float top    = kCameraHeight;
    float left   = bottom * aspectRatio;
    float right  = top * aspectRatio;
    _camera      = PepperMint::CreateScope<PepperMint::OrthographicCamera>(left, right, bottom, top);
}
