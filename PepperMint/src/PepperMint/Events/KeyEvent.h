#pragma once

#include "PepperMint/Core/Input.h"
#include "PepperMint/Events/Event.h"

namespace PepperMint {

class KeyEvent : public Event {
  public:
    ~KeyEvent() override = default;

    inline KeyCode keyCode() const { return _keyCode; }

    EVENT_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT)

  protected:
    KeyEvent(KeyCode iKeyCode) : _keyCode(iKeyCode) {}

    KeyCode _keyCode;
};

class KeyPressedEvent : public KeyEvent {
  public:
    KeyPressedEvent(KeyCode iKeyCode, int iRepeatCount)
        : KeyEvent(iKeyCode), _repeatCount(iRepeatCount) {}
    ~KeyPressedEvent() override = default;

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

class KeyReleasedEvent : public KeyEvent {
  public:
    KeyReleasedEvent(KeyCode iKeyCode) : KeyEvent(iKeyCode) {}
    ~KeyReleasedEvent() override = default;

    std::string toString() const override {
        std::ostringstream ss;
        ss << "KeyReleasedEvent: " << _keyCode;
        return ss.str();
    }

    EVENT_TYPE(KEY_RELEASED)
};

class KeyTypedEvent : public KeyEvent {
  public:
    KeyTypedEvent(KeyCode iKeyCode) : KeyEvent(iKeyCode) {}
    ~KeyTypedEvent() override = default;

    std::string toString() const override {
        std::ostringstream ss;
        ss << "KeyTypedEvent: " << _keyCode;
        return ss.str();
    }

    EVENT_TYPE(KEY_TYPED)
};
}
