#pragma once

#include "pmpch.h"

#include "PepperMint/Core.h"

namespace PepperMint {

/*
Events in PepperMint are currently blocking, meaning when an event occurs it immediately gets dispatched and must be 
dealt with right then and there.
For the future, a better strategy might be to buffer events in an event bus and process them during the "event"
part of the update stage 
*/

enum class EventType {
	NONE = 0,
	WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED,
	APP_TICK, APP_UPDATE, APP_RENDER,
	KEY_PRESSED, KEY_RELEASED, KEY_TYPED,
	MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_MOVE, MOUSE_SCROLL
};

#define EVENT_TYPE(type) static EventType StaticType() { return EventType::##type; }\
						 virtual EventType eventType() const override { return StaticType(); }\
						 virtual const char* name() const override { return #type; }

enum EventCategory {
	EVENT_CATEGORY_NONE			= 0,
	EVENT_CATEGORY_APPLICATION	= BIT(0),
	EVENT_CATEGORY_INPUT		= BIT(1),
	EVENT_CATEGORY_KEYBOARD		= BIT(2),
	EVENT_CATEGORY_MOUSE		= BIT(3),
	EVENT_CATEGORY_MOUSE_BUTTON	= BIT(4)
};

#define EVENT_CATEGORY(category) virtual int categoryFlags() const override { return category; }

class PM_API Event {
public:
	Event() = default;
	~Event() = default;

	virtual EventType eventType() const = 0;
	virtual const char* name() const = 0;
	virtual int categoryFlags() const = 0;

	virtual std::string toString() const { return name(); }

	inline bool isInCategory(EventCategory iCategory) {
		return categoryFlags() & iCategory;
	}

	inline void setHandled(bool iHandled) { _handled = iHandled; }
	inline bool handled() const { return _handled; }

private:
	bool _handled = false;
};

inline std::ostream& operator<<(std::ostream& out, const Event& iEvent) {
	return out << iEvent.toString();
}

class EventDispatcher {
public:
	EventDispatcher(Event& iEvent) :
		_event(iEvent) {}
	~EventDispatcher() = default;

	template<typename T, typename F>
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