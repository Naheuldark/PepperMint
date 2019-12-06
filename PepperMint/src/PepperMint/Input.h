#pragma once

#include "PepperMint/Core.h"

#include <utility>

namespace PepperMint {

class PM_API Input {
public:
	Input() = default;
	~Input() = default;

	inline static bool IsKeyPressed(int iKeyCode) { return kInstance->isKeyPressedImpl(iKeyCode); }

	inline static bool IsMouseButtonPressed(int iButton) { return kInstance->isMouseButtonPressedImpl(iButton); }
	inline static std::pair<float, float> GetMousePosition() { return kInstance->getMousePositionImpl(); }
	inline static float GetMouseX() { return kInstance->getMouseXImpl(); }
	inline static float GetMouseY() { return kInstance->getMouseYImpl(); }

protected:
	virtual bool isKeyPressedImpl(int iKeyCode) = 0;

	virtual bool isMouseButtonPressedImpl(int iButton) = 0;
	virtual std::pair<float, float> getMousePositionImpl() = 0;
	virtual float getMouseXImpl() = 0;
	virtual float getMouseYImpl() = 0;

private:
	static Input* kInstance;
};
}