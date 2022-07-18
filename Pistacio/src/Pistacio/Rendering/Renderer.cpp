#include "pch.h"
#include "Renderer.h"
#include "RenderPasses/TextureQuadRenderPass.h"


namespace Pistacio
{
  SceneData Renderer::SceneData{};
  ShaderLibrary Renderer::ShaderLibrary;

  void Renderer::Init()
  {
    RendererAPI::Get()->Enable({ RendererAPI::Capability::DepthTest, RendererAPI::Capability::Blend });
    RendererAPI::Get()->SetDepthFunc(RendererAPI::DepthFunction::LEqual);
    RendererAPI::Get()->DepthMask(true);
    RendererAPI::Get()->SetBlendFunction(RendererAPI::BlendFunction::SrcAlpha, RendererAPI::BlendFunction::OneMinusSrcAlpha);
    
    SceneData.TextureQuadRenderPass = CreateRef<TextureQuadRenderPass>();
    SceneData.FrameDependentUniformBuffer.reset(UniformBuffer::Create(4 * 4 * 4, 0));
    SceneData.TransformDependentUniformBuffer.reset(UniformBuffer::Create(4 * 4 * 4, 1));
  }

  void Renderer::BeginScene(Camera camera, Ref<Framebuffer> target, glm::vec4 clearColor)
  {
    SceneData.ClearColor = clearColor;
    SceneData.ViewProjectionMatrix = camera.projectionMatrix * camera.viewMatrix;
    SceneData.Target = target;
    
    RendererAPI::Get()->Bind(SceneData.Target);
    RendererAPI::Get()->SetClearColor(SceneData.ClearColor);
    RendererAPI::Get()->SetClearDepth(1000.0f);
    RendererAPI::Get()->Clear();

  }

  void Renderer::EndScene()
  {
  }

  void Pistacio::Renderer::Present(Ref<Framebuffer> presentTarget)
  {
    SceneData.TextureQuadRenderPass->Render(SceneData.Target->GetColorAttachment(), presentTarget);
  }

  void Renderer::Submit(Ref<Shader> shader, glm::mat4 transform, const Ref<VertexArray> vertexArray, const Ref<Texture2D> texture)
  {
    RendererAPI::Get()->Bind(shader);
    SceneData.FrameDependentUniformBuffer->SetData(&SceneData.ViewProjectionMatrix, 4 * 4 * 4, 0);
    SceneData.TransformDependentUniformBuffer->SetData(&transform, 4 * 4 * 4, 0);
    RendererAPI::Get()->Bind(vertexArray);
    RendererAPI::Get()->Bind(texture, 0);
    RendererAPI::Get()->DrawIndexed(vertexArray);

  }

  void Pistacio::Renderer::SetViewport(int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height)
  {
    RendererAPI::Get()->SetViewport(xoffset, yoffset, width, height);
  }

  ShaderLibrary& Renderer::GetShaderLibrary()
  {
    return ShaderLibrary;
  }

}