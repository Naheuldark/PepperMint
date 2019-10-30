#pragma once

#include "Event.h"

#include <sstream>

namespace PepperMint {

class PM_API WindowCloseEvent : public Event {
public:
	WindowCloseEvent() = default;
	~WindowCloseEvent() = default;

	EVENT_CATEGORY(EVENT_CATEGORY_APPLICATION)
	EVENT_TYPE(WINDOW_CLOSE)
};

class PM_API WindowResizeEvent : public Event {
public:
	WindowResizeEvent(unsigned int iWidth, unsigned int iHeight) :
		_width(iWidth), _height(iHeight) {}
	~WindowResizeEvent() = default;

	std::string toString() const override {
		std::ostringstream ss;
		ss << "WindowResizeEvent: " << _width << ", " << _height;
		return ss.str();
	}

	EVENT_CATEGORY(EVENT_CATEGORY_APPLICATION)
	EVENT_TYPE(WINDOW_RESIZE)

private:
	unsigned int _width, _height;
};

class PM_API AppTickEvent : public Event {
public:
	AppTickEvent() = default;
	~AppTickEvent() = default;

	EVENT_CATEGORY(EVENT_CATEGORY_APPLICATION)
	EVENT_TYPE(APP_TICK)
};

class PM_API AppUpdateEvent : public Event {
public:
	AppUpdateEvent() = default;
	~AppUpdateEvent() = default;

	EVENT_CATEGORY(EVENT_CATEGORY_APPLICATION)
	EVENT_TYPE(APP_UPDATE)
};

class PM_API AppRenderEvent : public Event {
public:
	AppRenderEvent() = default;
	~AppRenderEvent() = default;

	EVENT_CATEGORY(EVENT_CATEGORY_APPLICATION)
	EVENT_TYPE(APP_RENDER)
};
}