#pragma once

#include <functional>

#include "PepperMint/Core/Base.h"
#include "PepperMint/Debug/Instrumentor.h"

namespace PepperMint {

/*
 * Events in PepperMint are currently blocking, meaning when an event occurs it immediately gets
 * dispatched and must be dealt with right then and there. For the future, a better strategy might be
 * to buffer events in an event bus and process them during the "event" part of the update stage
 */

// Event Type
enum class EventType {
    NONE = 0,
    WINDOW_CLOSE,
    WINDOW_RESIZE,
    WINDOW_FOCUS,
    WINDOW_LOST_FOCUS,
    WINDOW_MOVED,
    APP_TICK,
    APP_UPDATE,
    APP_RENDER,
    KEY_PRESSED,
    KEY_RELEASED,
    KEY_TYPED,
    MOUSE_BUTTON_PRESSED,
    MOUSE_BUTTON_RELEASED,
    MOUSE_MOVE,
    MOUSE_SCROLL
};

#define EVENT_TYPE(type)                                                 \
    static EventType StaticType() { return EventType::type; }            \
    EventType        eventType() const override { return StaticType(); } \
    const char*      name() const override { return #type; }

// Event Category
enum class EventCategory { NONE = 0, APPLICATION = BIT(0), INPUT = BIT(1), KEYBOARD = BIT(2), MOUSE = BIT(3), MOUSE_BUTTON = BIT(4) };

EventCategory  operator|(EventCategory lhs, EventCategory rhs);
EventCategory  operator&(EventCategory lhs, EventCategory rhs);
EventCategory  operator^(EventCategory lhs, EventCategory rhs);
EventCategory  operator~(EventCategory rhs);
EventCategory& operator|=(EventCategory& lhs, EventCategory rhs);
EventCategory& operator&=(EventCategory& lhs, EventCategory rhs);
EventCategory& operator^=(EventCategory& lhs, EventCategory rhs);

#define EVENT_CATEGORY(category) \
    EventCategory categoryFlags() const override { return (category); }

class Event {
  public:
    Event()          = default;
    virtual ~Event() = default;

    virtual EventType     eventType() const     = 0;
    virtual const char*   name() const          = 0;
    virtual EventCategory categoryFlags() const = 0;

    virtual std::string toString() const { return name(); }

    bool isInCategory(EventCategory iCategory) { return static_cast<bool>(categoryFlags() & iCategory); }

    void setHandled(bool iHandled) { _handled = iHandled; }
    bool handled() const { return _handled; }

  private:
    bool _handled = false;
};

inline std::ostream& operator<<(std::ostream& out, const Event& iEvent) { return out << iEvent.toString(); }

class EventDispatcher {
  public:
    EventDispatcher(Event& iEvent) : _event(iEvent) {}
    ~EventDispatcher() = default;

    template <typename T, typename F>
    bool dispatch(const F& func) {
        if (_event.eventType() == T::StaticType()) {
            _event.setHandled(func(static_cast<T&>(_event)));
            return true;
        }

        return false;
    }

  private:
    Event& _event;
};
}
