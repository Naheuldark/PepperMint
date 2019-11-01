#pragma once

#include "Event.h"

namespace PepperMint {

class PM_API MouseMovedEvent : public Event {
public:
	MouseMovedEvent(float iMouseX, float iMouseY) :
		_mouseX(iMouseX), _mouseY(iMouseY) {}
	~MouseMovedEvent() = default;

	inline float x() const { return _mouseX; }
	inline float y() const { return _mouseY; }

	std::string toString() const override {
		std::ostringstream ss;
		ss << "MouseMovedEvent: " << _mouseX << ", " << _mouseY;
		return ss.str();
	}

	EVENT_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)
	EVENT_TYPE(MOUSE_MOVE)

private:
	float _mouseX, _mouseY;
};

class MouseScrolledEvent : public Event {
public:
	MouseScrolledEvent(float iXOffset, float iYOffset) :
		_xOffset(iXOffset), _yOffset(iYOffset) {}
	~MouseScrolledEvent() = default;

	inline float xOffset() const { return _xOffset; }
	inline float yOffset() const { return _yOffset; }

	std::string toString() const override {
		std::ostringstream ss;
		ss << "MouseScrolledEvent: " << _xOffset << ", " << _yOffset;
		return ss.str();
	}

	EVENT_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)
	EVENT_TYPE(MOUSE_SCROLL)

private:
	float _xOffset, _yOffset;
};

class PM_API MouseButtonEvent : public Event {
public:
	~MouseButtonEvent() = default;

	inline int mouseButton() const { return _button; }

	EVENT_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)

protected:
	MouseButtonEvent(int iButton) :
		_button(iButton) {}

	int _button;
};

class PM_API MouseButtonPressedEvent : public MouseButtonEvent {
public:
	MouseButtonPressedEvent(int iButton) : 
		MouseButtonEvent(iButton) {}
	~MouseButtonPressedEvent() = default;

	std::string toString() const override {
		std::ostringstream ss;
		ss << "MouseButtonPressedEvent: " << _button;
		return ss.str();
	}

	EVENT_TYPE(MOUSE_BUTTON_PRESSED)
};

class PM_API MouseButtonReleasedEvent : public MouseButtonEvent {
public:
	MouseButtonReleasedEvent(int iButton) :
		MouseButtonEvent(iButton) {}
	~MouseButtonReleasedEvent() = default;

	std::string toString() const override {
		std::ostringstream ss;
		ss << "MouseButtonReleasedEvent: " << _button;
		return ss.str();
	}

	EVENT_TYPE(MOUSE_BUTTON_RELEASED)
};
}