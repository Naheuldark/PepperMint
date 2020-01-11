#pragma once

#include "PepperMint/Core/Input.h"

namespace PepperMint {

class WindowsInput : public Input {
protected:
	bool isKeyPressedImpl(KeyCode iKeyCode) override;

	bool isMouseButtonPressedImpl(MouseCode iButton) override;
	std::pair<float, float> getMousePositionImpl() override;
	float getMouseXImpl() override;
	float getMouseYImpl() override;
};
}