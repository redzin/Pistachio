#include <Pistacio.h>
#include "Pistacio/Core/EntryPoint.h"
#include "Pistacio/Rendering/ImGui/ImGuiRenderer.h"
#include "Pistacio/Rendering/Swapchain.h"
#include "Pistacio/Rendering/Shader.h"
#include "Pistacio/Rendering/Buffer.h"
#include "Pistacio/Rendering/VertexArray.h"
#include "Pistacio/Rendering/Renderer.h"
#include "Pistacio/Rendering/UniformBuffer.h"
#include "Pistacio/Rendering/PerspectiveCamera.h"

namespace Pistacio
{
  class Sandbox : public Pistacio::Application
  {
  public:
    Sandbox() : Application("Sandbox") {}
    Sandbox(std::string name, uint32_t width, uint32_t height) : Application(name, width, height, false) {}
    ~Sandbox() {}
  };

  struct NewClearColorEvent
  {
    glm::vec4 clear_color = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);
  };

  class SampleImGuiLayer : public Pistacio::Layer
  {
  public:
    SampleImGuiLayer() : eventLibrary(Pistacio::Application::Get()->GetEventLibrary()) { }
    ~SampleImGuiLayer() = default;

    const char* GetName() const { return "SimpleImGuiLayer"; }

    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 old_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    EventLibrary& eventLibrary;

    void RenderImGuiDockspace()
    {

      ImGuiDockNodeFlags dockspace_flags
        = ImGuiDockNodeFlags_NoDockingInCentralNode
        | ImGuiDockNodeFlags_PassthruCentralNode;

      // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
      // because it would be confusing to have two docking targets within each others.
      ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
      const ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->WorkPos);
      ImGui::SetNextWindowSize(viewport->WorkSize);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    
      // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
      // and handle the pass-thru hole, so we ask Begin() to not render a background.
      if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

      // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
      // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
      // all active windows docked into it will lose their parent and become undocked.
      // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
      // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

      bool open = true;
      ImGui::Begin("Dockspace", &open, window_flags);
      ImGui::PopStyleVar();

      ImGui::PopStyleVar(2);

      // Submit the DockSpace
      ImGuiIO& io = ImGui::GetIO();
      if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
      {
        ImGuiID dockspace_id = ImGui::GetID("Dockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
      }

      if (ImGui::BeginMenuBar())
      {
        if (ImGui::BeginMenu("Options"))
        {
          if (ImGui::MenuItem("Play", NULL, false, &open != NULL))
          {
            //Enter play mode
          };
          ImGui::Separator();
          if (ImGui::MenuItem("Close", NULL, false, &open != NULL))
            Application::Get()->GetEventLibrary().Publish<WindowCloseEvent>(std::make_shared<WindowCloseEvent>());
          ImGui::EndMenu();
        }
        //ImGuiRenderer::HelpMarker("Helper text!");

        ImGui::EndMenuBar();
      }

      ImGui::End();
    }

    void OnGuiRender() override
    {

      RenderImGuiDockspace();

      //ImGui::ShowDemoWindow(&open);

      bool open = true;
      const char* windowName = "Pistacio Controls Panel";

      // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
      {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin(windowName);                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("The Pistacio editor.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        ImVec4 old_clear_color = clear_color;
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (old_clear_color.x != clear_color.x || old_clear_color.y != clear_color.y || old_clear_color.z != clear_color.z || old_clear_color.w != clear_color.w)
        {
          old_clear_color = clear_color;
          std::shared_ptr<NewClearColorEvent> e = std::shared_ptr<NewClearColorEvent>(new NewClearColorEvent{ glm::vec4(clear_color.x, clear_color.y, clear_color.z, clear_color.w) });
          eventLibrary.Publish(e);
        }

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
          counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
      }

    }

    void OnAttach() override
    {
      PSTC_INFO("SampleImGuiLayer attached!");
    }

    void OnDetach() override
    {
      PSTC_INFO("SampleImGuiLayer detached!");
    }

  };

  class SampleTriangleLayer : public Pistacio::Layer
  {
  public:
    glm::vec4 clear_color{ 0.45f, 0.55f, 0.60f, 1.00f };

    float vertexAttributes[6 * 3] = {
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
       0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    unsigned int indices[3] = { 0, 1 ,2 };


    std::shared_ptr<UniformBuffer> uniformBuffer;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer> indexBuffer;
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<PerspectiveCamera> camera;

    const char* GetName() const { return "SimpleImGuiLayer"; }

    std::chrono::steady_clock::time_point start;

    void OnAttach() override
    {
      Pistacio::Application::Get()->GetEventLibrary().Subscribe<NewClearColorEvent>([this](NewClearColorEvent& e) { clear_color = e.clear_color; return true; });

      std::string vertexSrc =
        R"(
#version 430 core

layout(std140, binding = 0) uniform CameraData
{
  mat4 view;
  mat4 projection;
};

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;

layout(location = 0) out vec3 out_color;

void main()
{
  out_color = a_Color;
  gl_Position = projection * view * vec4(a_Position, 1.0);
}
)";

      std::string fragmentSrc =
        R"(
#version 430 core

layout(location = 0) in vec3 in_color;

out vec4 out_color;

void main()
{
  out_color = vec4((in_color), 1.0);
}
)";

      shader.reset(Shader::Create(vertexSrc, fragmentSrc));
      shader->Bind();

      // Create vertex attribute buffer
      vertexBuffer.reset(VertexBuffer::Create(vertexAttributes, sizeof(vertexAttributes)));
      BufferLayout vertexBufferLayout{
        { BufferDataType::Float3, "a_Position" },
        { BufferDataType::Float3, "a_Color" }
      };
      vertexBuffer->SetLayout(vertexBufferLayout);

      //Create index buffer
      indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

      //Create vertex array
      vertexArray.reset(VertexArray::Create());
      vertexArray->SetIndexBuffer(indexBuffer);
      vertexArray->AddVertexBuffer(vertexBuffer);

      vertexArray->Unbind();

      uniformBuffer.reset(UniformBuffer::Create(4*4*4*2, 0));

      start = std::chrono::steady_clock::now();
      camera.reset(new PerspectiveCamera(
        glm::vec3(0.75f, 0.0f, -0.5f),
        glm::vec3(0.75f, 0.0f, 0.0f),
        45.0f, 1080, 720, 1.0f, 100.0f
      ));

      PSTC_INFO("SampleTriangleLayer attached!");
    }

    void OnDetach() override
    {
      PSTC_INFO("SampleTriangleLayer detached!");
    }

    void OnRender() override
    {
      
      RenderCommand::SetClearColor(clear_color);
      RenderCommand::Clear();
      
      Renderer::BeginScene();
      shader->Bind();


      auto time_step = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
      //uboData.offset.x = std::sin((double)time_step.count() / 1000) / 2;

      uniformBuffer->SetData(camera->GetDataPtr(), 4*4*4*2, 0);

      Renderer::Submit(vertexArray);

    }

  };

  Pistacio::Application* Pistacio::ApplicationFactory::doCreate()
  {
    Sandbox* sandbox = new Sandbox("Sandbox App", 1080, 720);
    SampleImGuiLayer* sampleImGuiLayer = new SampleImGuiLayer();
    SampleTriangleLayer* sampleTriangleLayer = new SampleTriangleLayer();

    sandbox->GetEventLibrary().Register<NewClearColorEvent>();
    sandbox->PushLayer(sampleTriangleLayer);
    sandbox->PushOverlay(sampleImGuiLayer);

    return sandbox;
  }

}



