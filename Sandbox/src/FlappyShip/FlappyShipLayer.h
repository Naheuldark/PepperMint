#pragma once

#include <PepperMint.h>

#include "Level.h"

#include <imgui/imgui.h>


class FlappyShipLayer : public PepperMint::Layer {
public:
	FlappyShipLayer();
	~FlappyShipLayer() override = default;

	void onAttach() override;
	void onDetach() override;
	void onUpdate(PepperMint::Timestep iTimestep) override;
	void onImGuiRender() override;
	void onEvent(PepperMint::Event& iEvent) override;

private:
	void updateCamera(uint32_t iWidth, uint32_t iHeight);

	bool onWindowResize(PepperMint::WindowResizeEvent& iEvent);
	bool onMouseButtonPressed(PepperMint::MouseButtonPressedEvent& iEvent);

private:
	PepperMint::Scope<PepperMint::OrthographicCamera> _camera;
	Level _level;
	ImFont* _font;
	
	bool _blink = false;
	float _time = 0.0f;

	enum class GameState : int {
		PLAY = 0,
		MENU = 1,
		GAME_OVER = 2
	};
	GameState _state = GameState::MENU;
};
