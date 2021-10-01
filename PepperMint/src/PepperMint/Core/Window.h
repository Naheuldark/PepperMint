#pragma once

#include "PepperMint/Core/Base.h"
#include "PepperMint/Events/Event.h"

namespace PepperMint {

struct WindowProperties {
    std::string title;
    uint32_t    width, height;

    WindowProperties(const std::string& iTitle  = "PepperMint Engine",
                     uint32_t           iWidth  = 3500, // TODO Update with screen resolution (old 1600x900)
                     uint32_t           iHeight = 1800)
        : title(iTitle), width(iWidth), height(iHeight) {}
};

/*
 * Interface representing a desktop system based window
 */
class Window {
  public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() = default;

    virtual void onUpdate() = 0;

    virtual uint32_t width() const  = 0;
    virtual uint32_t height() const = 0;

    // Window attributes
    virtual void setEventCallback(const EventCallbackFn& iCallback) = 0;
    virtual void setVSync(bool iEnabled)                            = 0;
    virtual bool isVSync() const                                    = 0;

    // Get the native Window pointer
    virtual void* nativeWindow() const = 0;

    static Scope<Window> Create(const WindowProperties& iProperties = WindowProperties());
};

}
