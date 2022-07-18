#include "pch.h"
#include "TextureQuadRenderPass.h"

namespace Pistacio
{
  TextureQuadRenderPass::TextureQuadRenderPass()
  {

    float vertexAttributes[5 * 4] = {
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
       1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
       1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
      -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
    };
    unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };

    // Create vertex attribute buffer
    m_VertexBuffer = VertexBuffer::Create(vertexAttributes, sizeof(vertexAttributes));
    BufferLayout vertexBufferLayout{
      { BufferDataType::Float3, "a_Position" },
      { BufferDataType::Float2, "a_TexCoord" }
    };
    m_VertexBuffer->SetLayout(vertexBufferLayout);

    //Create index buffer
    m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

    //Create vertex array
    m_VertexArray = VertexArray::Create();
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    m_VertexArray->AddVertexBuffer(m_VertexBuffer);

  }

  void TextureQuadRenderPass::Render(Ref<Texture2D> texture, Ref<Framebuffer> target) const
  {
    auto shader = Renderer::GetShaderLibrary().Get("TextureQuad");
    glm::mat4 transform(1);
    Camera fullscreenOrthoProj;
    fullscreenOrthoProj.viewMatrix = glm::mat4(1);
    fullscreenOrthoProj.projectionMatrix = glm::mat4(1);

    RendererAPI::Get()->Bind(target);
    RendererAPI::Get()->SetClearColor(glm::vec4());
    RendererAPI::Get()->SetClearDepth(1000.0f);
    RendererAPI::Get()->Clear();
    RendererAPI::Get()->Bind(shader);
    RendererAPI::Get()->Bind(m_VertexArray);
    RendererAPI::Get()->Bind(texture, 0);
    RendererAPI::Get()->DrawIndexed(m_VertexArray);
  }

}

