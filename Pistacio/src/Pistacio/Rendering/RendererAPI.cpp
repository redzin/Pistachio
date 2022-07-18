#include "pch.h"
#include "RendererAPI.h"

#include "PistacioPlatform/OpenGL/RendererAPI_OpenGL.h"

namespace Pistacio
{

  Scope<RendererAPI> RendererAPI::Instance = CreateScope<RendererAPI_OpenGL>();
  RendererAPI::API RendererAPI::api = RendererAPI::API::OpenGL;

}
