#pragma once

#include "pmpch.h"

#include "PepperMint/Core.h"
#include "PepperMint/Events/Event.h"

namespace PepperMint {

struct WindowProperties {
	std::string title;
	unsigned int width, height;

	WindowProperties(const std::string& iTitle = "PepperMint Engine",
					 unsigned int iWidth = 1280,
					 unsigned int iHeight = 720) :
		title(iTitle), width(iWidth), height(iHeight) {}
};

/*
 * Interface representing a desktop system based window
 */
class PM_API Window {
public:
	using EventCallbackFn = std::function<void(Event&)>;

	virtual ~Window() = default;

	virtual void onUpdate() = 0;

	virtual unsigned int width() const = 0;
	virtual unsigned int height() const = 0;

	// Window attributes
	virtual void setEventCallback(const EventCallbackFn& iCallback) = 0;
	virtual void setVSync(bool iEnabled) = 0;
	virtual bool isVSync() const = 0;

	static Window* Create(const WindowProperties& iProperties = WindowProperties());
};

}