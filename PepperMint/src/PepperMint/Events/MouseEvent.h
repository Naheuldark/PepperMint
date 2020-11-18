#pragma once

#include "PepperMint/Core/MouseCodes.h"
#include "PepperMint/Events/Event.h"

namespace PepperMint {

class MouseMovedEvent : public Event {
  public:
    MouseMovedEvent(const float iMouseX, const float iMouseY) : _mouseX(iMouseX), _mouseY(iMouseY) {}
    ~MouseMovedEvent() override = default;

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
    MouseScrolledEvent(const float iXOffset, const float iYOffset) : _xOffset(iXOffset), _yOffset(iYOffset) {}
    ~MouseScrolledEvent() override = default;

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

class MouseButtonEvent : public Event {
  public:
    ~MouseButtonEvent() override = default;

    inline MouseCode mouseButton() const { return _button; }

    EVENT_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)

  protected:
    MouseButtonEvent(const MouseCode iButton) : _button(iButton) {}

    MouseCode _button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
  public:
    MouseButtonPressedEvent(const MouseCode iButton) : MouseButtonEvent(iButton) {}
    ~MouseButtonPressedEvent() override = default;

    std::string toString() const override {
        std::ostringstream ss;
        ss << "MouseButtonPressedEvent: " << _button;
        return ss.str();
    }

    EVENT_TYPE(MOUSE_BUTTON_PRESSED)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
  public:
    MouseButtonReleasedEvent(const MouseCode iButton) : MouseButtonEvent(iButton) {}
    ~MouseButtonReleasedEvent() override = default;

    std::string toString() const override {
        std::ostringstream ss;
        ss << "MouseButtonReleasedEvent: " << _button;
        return ss.str();
    }

    EVENT_TYPE(MOUSE_BUTTON_RELEASED)
};
}
