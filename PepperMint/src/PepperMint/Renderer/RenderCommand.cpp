#include "pmpch.h"

#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace PepperMint {

Scope<RendererAPI> RenderCommand::sRendererAPI = CreateScope<OpenGLRendererAPI>();
}