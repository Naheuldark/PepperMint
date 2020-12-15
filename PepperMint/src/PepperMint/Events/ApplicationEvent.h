#pragma once

#include "PepperMint/Events/Event.h"

namespace PepperMint {

class WindowCloseEvent : public Event {
  public:
    WindowCloseEvent()           = default;
    ~WindowCloseEvent() override = default;

    EVENT_CATEGORY(EventCategory::APPLICATION)
    EVENT_TYPE(WINDOW_CLOSE)
};

class WindowResizeEvent : public Event {
  public:
    WindowResizeEvent(unsigned int iWidth, unsigned int iHeight) : _width(iWidth), _height(iHeight) {}
    ~WindowResizeEvent() override = default;

    std::string toString() const override {
        std::ostringstream ss;
        ss << "WindowResizeEvent: " << _width << ", " << _height;
        return ss.str();
    }

    unsigned int width() const { return _width; }
    unsigned int height() const { return _height; }

    EVENT_CATEGORY(EventCategory::APPLICATION)
    EVENT_TYPE(WINDOW_RESIZE)

  private:
    unsigned int _width, _height;
};

class AppTickEvent : public Event {
  public:
    AppTickEvent()           = default;
    ~AppTickEvent() override = default;

    EVENT_CATEGORY(EventCategory::APPLICATION)
    EVENT_TYPE(APP_TICK)
};

class AppUpdateEvent : public Event {
  public:
    AppUpdateEvent()           = default;
    ~AppUpdateEvent() override = default;

    EVENT_CATEGORY(EventCategory::APPLICATION)
    EVENT_TYPE(APP_UPDATE)
};

class AppRenderEvent : public Event {
  public:
    AppRenderEvent()           = default;
    ~AppRenderEvent() override = default;

    EVENT_CATEGORY(EventCategory::APPLICATION)
    EVENT_TYPE(APP_RENDER)
};
}
