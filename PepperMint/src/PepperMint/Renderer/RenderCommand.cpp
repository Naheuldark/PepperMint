#include "PepperMint/Renderer/RenderCommand.h"
#include "pmpch.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace PepperMint {

Scope<RendererAPI> RenderCommand::sRendererAPI = RendererAPI::Create();
}
