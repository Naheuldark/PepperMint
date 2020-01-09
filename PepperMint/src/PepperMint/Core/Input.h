#pragma once

#include <utility>

#include "Core.h"

namespace PepperMint {

class Input {
public:
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	~Input() = default;

	inline static bool IsKeyPressed(int iKeyCode) { return sInstance->isKeyPressedImpl(iKeyCode); }

	inline static bool IsMouseButtonPressed(int iButton) { return sInstance->isMouseButtonPressedImpl(iButton); }
	inline static std::pair<float, float> GetMousePosition() { return sInstance->getMousePositionImpl(); }
	inline static float GetMouseX() { return sInstance->getMouseXImpl(); }
	inline static float GetMouseY() { return sInstance->getMouseYImpl(); }

protected:
	Input() = default;

	virtual bool isKeyPressedImpl(int iKeyCode) = 0;

	virtual bool isMouseButtonPressedImpl(int iButton) = 0;
	virtual std::pair<float, float> getMousePositionImpl() = 0;
	virtual float getMouseXImpl() = 0;
	virtual float getMouseYImpl() = 0;

private:
	static Input* sInstance;
};
}