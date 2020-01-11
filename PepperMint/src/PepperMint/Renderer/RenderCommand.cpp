#include "pmpch.h"
#include "PepperMint/Renderer/RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace PepperMint {

Scope<RendererAPI> RenderCommand::sRendererAPI = RendererAPI::Create();
}