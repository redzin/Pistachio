#include <Pistacio.h>
#include "Pistacio/Core/EntryPoint.h"
#include "Pistacio/Rendering/ImGui/ImGuiRenderer.h"
#include "Pistacio/Rendering/Swapchain.h"
#include "Pistacio/Rendering/Shader.h"
#include "Pistacio/Rendering/Buffer.h"
#include "Pistacio/Rendering/VertexArray.h"
#include "Pistacio/Rendering/Renderer.h"
#include "Pistacio/Rendering/UniformBuffer.h"
#include "Pistacio/Rendering/Texture.h"
#include "Pistacio/Rendering/Framebuffer.h"
#include "Pistacio/Rendering/Camera/PerspectiveCamera.h"
#include "Pistacio/Rendering/Camera/CameraController_FPS.h"
#include "Pistacio/Rendering/Camera/CameraController_Pan.h"
#include "Pistacio/Rendering/Camera/CameraController_Zoom.h"
#include "Pistacio/Rendering/Camera/CameraController_Orbit.h"

namespace Pistacio
{
  class Sandbox : public Pistacio::Application
  {
  public:
    Sandbox() : Application("Sandbox") {}
    Sandbox(std::string name, uint32_t width, uint32_t height) : Application(name, width, height, false) {}
    ~Sandbox() {}
  };

  class SampleTriangleLayer : public Layer
  {
  public:

    bool vsync = Application::Get()->GetWindow().IsVSync();
    ImVec4 imgui_clear_color = ImVec4(0.7f, 0.85f, 0.92f, 1.00f);
    EventLibrary& eventLibrary;

    glm::vec4 clear_color{ 0.65f, 0.88f, 0.92f, 1.00f };

    float vertexAttributes[8 * 4] = {
      -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
       1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
       1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
    };
    unsigned int indices[6] = { 0, 1, 2, 2, 3, 0};

    Ref<VertexBuffer> vertexBuffer;
    Ref<IndexBuffer> indexBuffer;
    Ref<VertexArray> vertexArray;
    Ref<Texture2D> texture;
    Ref<Texture2D> textureWithAlpha;
    Ref<Framebuffer> framebuffer;
    Ref<Framebuffer> presentbuffer;

    Camera camera;
    inline static constexpr float fovY = 70;
    inline static constexpr float zNear = 0.1f;
    inline static constexpr float zFar = 100.0f;
    CameraController_Pan cameraControllerPan;
    CameraController_Zoom cameraControllerZoom;
    CameraController_Orbit cameraControllerOrbit;

    const char* GetName() const { return "SimpleImGuiLayer"; }

    SampleTriangleLayer(uint32_t width, uint32_t height) : eventLibrary(Pistacio::Application::Get()->GetEventLibrary())
    {
      //camera = PerspectiveCamera::Create(glm::vec3{ 0.0f, 0.0f, -0.5f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, 45.0f, 1080, 720, 0.1f, 100.0f);
      camera = CameraController_Orbit::CreateCamera(cameraControllerOrbit, glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), fovY, width, height, zNear, zFar);
    }

    void OnAttach() override
    {
      EventLibrary eventLib = Pistacio::Application::Get()->GetEventLibrary();
      eventLib.Subscribe<MouseMoveEvent>([this](MouseMoveEvent e)
        {
          cameraControllerPan = CameraController_Pan::OnMouseMoveEvent(cameraControllerPan, e);
          cameraControllerOrbit = CameraController_Orbit::OnMouseMoveEvent(cameraControllerOrbit, e);
          return true;
        }
      );
      eventLib.Subscribe<AppUpdateEvent>([this](AppUpdateEvent e)
        {
          std::tie(camera, cameraControllerPan) = CameraController_Pan::OnUpdate(camera, e.dt, cameraControllerPan);
          std::tie(camera, cameraControllerOrbit) = CameraController_Orbit::OnUpdate(camera, e.dt, cameraControllerOrbit);
          //std::tie(camera, cameraControllerZoom) = CameraController_Zoom::OnUpdate(camera, e.dt, cameraControllerZoom);
          return true;
        }
      );
      eventLib.Subscribe<MouseClickEvent>([this](MouseClickEvent e)
        {
          ImGuiIO guiIO = ImGui::GetIO();
          if (!guiIO.WantCaptureMouse)
          {
            Window& window = Application::Get()->GetWindow();
            if (e.action == Input::ButtonAction::KeyPressed && e.mouseKey == Input::MouseCode::ButtonRight)
            {
              // do not use for now
              // cameraControllerPan = CameraController_Pan::Enable(cameraControllerPan);
              window.HideCursor();
            }
            else if (e.action == Input::ButtonAction::KeyPressed && e.mouseKey == Input::MouseCode::ButtonLeft)
            {
              cameraControllerOrbit = CameraController_Orbit::Enable(cameraControllerOrbit);
              window.HideCursor();
            }
            else if (e.action == Input::ButtonAction::KeyReleased && e.mouseKey == Input::MouseCode::ButtonRight)
            {
              cameraControllerPan = CameraController_Pan::Disable(cameraControllerPan);
              window.ShowCursor();
            }
            else if (e.action == Input::ButtonAction::KeyReleased && e.mouseKey == Input::MouseCode::ButtonLeft)
            {
              cameraControllerOrbit = CameraController_Orbit::Disable(cameraControllerOrbit);
              window.ShowCursor();
            }
          }
          
          return true;
        }
      );
      eventLib.Subscribe<MouseScrollEvent>([this](MouseScrollEvent e)
        {
          cameraControllerOrbit = CameraController_Orbit::OnMouseScrollEvent(cameraControllerOrbit, e);
          return true;
        }
      );
      eventLib.Subscribe<WindowResizeEvent>([this](WindowResizeEvent e)
        {
          camera = PerspectiveCamera::NewPerspective(camera, fovY, e.width, e.height, zNear, zFar);
          return true;
        }
      );


      ShaderLibrary& shaderLibrary = Renderer::GetShaderLibrary();
      shaderLibrary.Load("assets/shaders/TextureQuad.glsl");
      shaderLibrary.Load("assets/shaders/TextureQuadCamera.glsl");
      
      // Create vertex attribute buffer
      vertexBuffer = VertexBuffer::Create(vertexAttributes, sizeof(vertexAttributes));
      BufferLayout vertexBufferLayout{
        { BufferDataType::Float3, "a_Position" },
        { BufferDataType::Float3, "a_Color" },
        { BufferDataType::Float2, "a_TexCoord" }
      };
      vertexBuffer->SetLayout(vertexBufferLayout);
      
      //Create index buffer
      indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
      
      //Create vertex array
      vertexArray = VertexArray::Create();
      vertexArray->SetIndexBuffer(indexBuffer);
      vertexArray->AddVertexBuffer(vertexBuffer);
      
      
      TextureDescriptor textDesc;
      textDesc.MagFilter = TextureFilter::Nearest;
      textDesc.MinFilter = TextureFilter::Nearest;
      texture = Texture2D::Create("assets/textures/Checkerboard.png", textDesc);
      
      textDesc.MagFilter = TextureFilter::Linear;
      textDesc.MinFilter = TextureFilter::Linear;
      textureWithAlpha = Texture2D::Create("assets/textures/ChernoLogo.png", textDesc);
      
      FramebufferDescriptor fbDesc;
      fbDesc.Width = Application::Get()->GetWindow().GetWidth();
      fbDesc.Height = Application::Get()->GetWindow().GetHeight();
      framebuffer = Framebuffer::Create(fbDesc);
      
      fbDesc.IsSwapchainTarget = true;
      presentbuffer = Framebuffer::Create(fbDesc);


      PSTC_INFO("SampleTriangleLayer attached!");
    }

    void OnDetach() override
    {
      PSTC_INFO("SampleTriangleLayer detached!");
    }

    void OnRender() override
    {
      auto shader = Renderer::GetShaderLibrary().Get("TextureQuadCamera");
      glm::mat4 transform(1);
      
      Renderer::BeginScene(camera, framebuffer, clear_color);
      Renderer::Submit(shader, transform, vertexArray, texture);
      Renderer::Submit(shader, glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.5f)), vertexArray, textureWithAlpha);
      Renderer::EndScene();
      Renderer::Present(presentbuffer);

    }

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
            Application::Get()->GetEventLibrary().Publish<WindowCloseEvent>(std::move(WindowCloseEvent()));
          ImGui::EndMenu();
        }
        //ImGuiRenderer::HelpMarker("Helper text!");

        ImGui::EndMenuBar();
      }

      ImGui::End();
    }

    void OnGuiRender() override
    {

      //ImGui::ShowDemoWindow();

      RenderImGuiDockspace();

      ImGui::Begin("Pistacio Controls Panel");

      ImGui::Checkbox("VSync", &vsync);

      if (vsync != Application::Get()->GetWindow().IsVSync())
        Application::Get()->GetWindow().SetVSync(vsync);

      //static float f = 0.0f;
      //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

      ImGui::ColorEdit3("clear color", (float*)&imgui_clear_color); // Edit 3 floats representing a color
      clear_color.x = imgui_clear_color.x;
      clear_color.y = imgui_clear_color.y;
      clear_color.z = imgui_clear_color.z;
      clear_color.w = imgui_clear_color.w;

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::Text("Primary camera location: (%.2f, %.2f, %.2f)", camera.position.x, camera.position.y, camera.position.z);
      ImGui::End();

      ImGui::Begin("Viewport");
      ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
      TextureDescriptor colorTextDesc = framebuffer->GetColorAttachment()->GetDescriptor();
      
      //if (ImGui::IsWindowHovered() && ImGui::IsItemHovered())
      //  PSTC_CORE_INFO("Window hovered!");

      ImGui::Image((void*)framebuffer->GetColorAttachment()->GetRendererID(), ImVec2(colorTextDesc.Width, colorTextDesc.Height), ImVec2(0, 1), ImVec2(1, 0));
      ImGui::End();

    }
  };

  Pistacio::Application* Pistacio::ApplicationFactory::doCreate()
  {
    Sandbox* sandbox = new Sandbox("Sandbox App", 1080, 720);

    SampleTriangleLayer* sampleTriangleLayer = new SampleTriangleLayer(1080, 720);

    Application::Get()->GetEventLibrary().Subscribe<KeyEvent>([](KeyEvent e)
      {
        if (e.key == Input::KeyCode::Escape && e.action == Input::ButtonAction::KeyPressed)
          Application::Get()->GetEventLibrary().Publish<WindowCloseEvent>(WindowCloseEvent{});
        return true;
      }
    );

    sandbox->AddLayer(sampleTriangleLayer);

    return sandbox;
  }

}



