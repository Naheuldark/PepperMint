#pragma once

#include "Player.h"

class Level {
public:
	Level() = default;
	~Level() = default;

	void init();

	void onUpdate(PepperMint::Timestep iTimestep);
	void onRender();
	void onImGuiRender();

private:
	Player _player;
};
