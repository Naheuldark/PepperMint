#pragma once

#include "PepperMint/Input.h"

namespace PepperMint {

class WindowsInput : public Input {
protected:
	virtual bool isKeyPressedImpl(int iKeyCode) override;

	virtual bool isMouseButtonPressedImpl(int iButton) override;
	virtual std::pair<float, float> getMousePositionImpl() override;
	virtual float getMouseXImpl() override;
	virtual float getMouseYImpl() override;
};
}