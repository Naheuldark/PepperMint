#pragma once

namespace PepperMint {

class GraphicsContext {
  public:
    GraphicsContext()          = default;
    virtual ~GraphicsContext() = default;

    virtual void init()        = 0;
    virtual void swapBuffers() = 0;

    static Scope<GraphicsContext> Create(void* iWindow);
};
}
