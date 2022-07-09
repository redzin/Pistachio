#include "pch.h"
#include "RenderCommand.h"

#include "PistacioPlatform/OpenGL/RendererAPI_OpenGL.h"

namespace Pistacio
{
  RendererAPI* RenderCommand::rendererAPI = new RendererAPI_OpenGL;
}
