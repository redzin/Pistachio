#pragma once
#include "pch.h"

#include "RendererAPI.h"
#include "Shader.h"
#include "Camera/Camera.h"
#include "UniformBuffer.h"
#include "Framebuffer.h"
#include "RenderPasses/TextureQuadRenderPass.h"

namespace Pistacio
{

  struct SceneData
  {
    glm::vec4 ClearColor;
    glm::mat4 ViewProjectionMatrix;
    Scope<UniformBuffer> FrameDependentUniformBuffer;
    Scope<UniformBuffer> TransformDependentUniformBuffer;
    Ref<Framebuffer> Target;
    Ref<TextureQuadRenderPass> TextureQuadRenderPass;
  };

  class Renderer
  {
  public:
    static void Init();
    static void BeginScene(Camera camera, Ref<Framebuffer> target, glm::vec4 clearColor = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f));
    static void EndScene();
    static void Present(Ref<Framebuffer> presentTarget);
    static void Submit(Ref<Shader> shader, glm::mat4 transform, const Ref<VertexArray> vertexArray, const Ref<Texture2D> texture);
    static void SetViewport(int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height);
    static ShaderLibrary& GetShaderLibrary();

  private:
    static ShaderLibrary ShaderLibrary;
    static SceneData SceneData;
  };

}

