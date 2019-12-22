#pragma once

#include "PepperMint/Input.h"

namespace PepperMint {

class WindowsInput : public Input {
protected:
	bool isKeyPressedImpl(int iKeyCode) override;

	bool isMouseButtonPressedImpl(int iButton) override;
	std::pair<float, float> getMousePositionImpl() override;
	float getMouseXImpl() override;
	float getMouseYImpl() override;
};
}