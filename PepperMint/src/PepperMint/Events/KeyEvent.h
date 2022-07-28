#pragma once

#include "PepperMint/Core/KeyCodes.h"
#include "PepperMint/Events/Event.h"

namespace PepperMint {

class KeyEvent : public Event {
  public:
    ~KeyEvent() override = default;

    KeyCode keyCode() const { return _keyCode; }

    EVENT_CATEGORY(EventCategory::KEYBOARD | EventCategory::INPUT)

  protected:
    KeyEvent(const KeyCode iKeyCode) : _keyCode(iKeyCode) {}

    KeyCode _keyCode;
};

class KeyPressedEvent : public KeyEvent {
  public:
    KeyPressedEvent(const KeyCode iKeyCode, bool iIsRepeat = false) : KeyEvent(iKeyCode), _isRepeat(iIsRepeat) {}
    ~KeyPressedEvent() override = default;

    bool isRepeat() const { return _isRepeat; }

    std::string toString() const override {
        std::ostringstream ss;
        ss << "KeyPressedEvent: " << _keyCode << " (repeat? " << _isRepeat << ")";
        return ss.str();
    }

    EVENT_TYPE(KEY_PRESSED)

  private:
    bool _isRepeat;
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
