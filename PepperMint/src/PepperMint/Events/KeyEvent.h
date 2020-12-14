#pragma once

#include "PepperMint/Core/KeyCodes.h"
#include "PepperMint/Events/Event.h"

namespace PepperMint {

class KeyEvent : public Event {
  public:
    ~KeyEvent() override = default;

    KeyCode keyCode() const { return _keyCode; }

    EVENT_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT)

  protected:
    KeyEvent(const KeyCode iKeyCode) : _keyCode(iKeyCode) {}

    KeyCode _keyCode;
};

class KeyPressedEvent : public KeyEvent {
  public:
    KeyPressedEvent(const KeyCode iKeyCode, const uint16_t iRepeatCount) : KeyEvent(iKeyCode), _repeatCount(iRepeatCount) {}
    ~KeyPressedEvent() override = default;

    uint16_t repeatCount() const { return _repeatCount; }

    std::string toString() const override {
        std::ostringstream ss;
        ss << "KeyPressedEvent: " << _keyCode << " (" << _repeatCount << " repeats)";
        return ss.str();
    }

    EVENT_TYPE(KEY_PRESSED)

  private:
    uint16_t _repeatCount;
};

class KeyReleasedEvent : public KeyEvent {
  public:
    KeyReleasedEvent(const KeyCode iKeyCode) : KeyEvent(iKeyCode) {}
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
    KeyTypedEvent(const KeyCode iKeyCode) : KeyEvent(iKeyCode) {}
    ~KeyTypedEvent() override = default;

    std::string toString() const override {
        std::ostringstream ss;
        ss << "KeyTypedEvent: " << _keyCode;
        return ss.str();
    }

    EVENT_TYPE(KEY_TYPED)
};
}
