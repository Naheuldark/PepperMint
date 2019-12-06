#pragma once

#include "Event.h"

namespace PepperMint {

class PM_API KeyEvent : public Event {
public:
	~KeyEvent() = default;

	inline int keyCode() const { return _keyCode; }

	EVENT_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT)

protected:
	KeyEvent(int iKeyCode) :
		_keyCode(iKeyCode) {}

	int _keyCode;
};

class PM_API KeyPressedEvent : public KeyEvent {
public:
	KeyPressedEvent(int iKeyCode, int iRepeatCount) : 
		KeyEvent(iKeyCode), _repeatCount(iRepeatCount) {}
	~KeyPressedEvent() = default;

	inline int repeatCount() const { return _repeatCount; }

	std::string toString() const override {
		std::ostringstream ss;
		ss << "KeyPressedEvent: " << _keyCode << " (" << _repeatCount << " repeats)";
		return ss.str();
	}

	EVENT_TYPE(KEY_PRESSED)

private:
	int _repeatCount;
};

class PM_API KeyReleasedEvent : public KeyEvent {
public:
	KeyReleasedEvent(int iKeyCode) : 
		KeyEvent(iKeyCode) {}
	~KeyReleasedEvent() = default;

	std::string toString() const override {
		std::ostringstream ss;
		ss << "KeyReleasedEvent: " << _keyCode;
		return ss.str();
	}

	EVENT_TYPE(KEY_RELEASED)
};

class PM_API KeyTypedEvent : public KeyEvent {
public:
	KeyTypedEvent(int iKeyCode) :
		KeyEvent(iKeyCode) {}
	~KeyTypedEvent() = default;

	std::string toString() const override {
		std::ostringstream ss;
		ss << "KeyTypedEvent: " << _keyCode;
		return ss.str();
	}

	EVENT_TYPE(KEY_TYPED)
};
}