#include "pmpch.h"

#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace PepperMint {

RendererAPI* RenderCommand::sRendererAPI = new OpenGLRendererAPI;
}