#include "pch.h"
#include "Pistacio/Rendering/ImGui/ImGuiRenderer.h"
#include "ImGuiRenderer_GLFW_OpenGL.h"

namespace Pistacio
{
  ImGuiRenderer_GLFW_OpenGL::ImGuiRenderer_GLFW_OpenGL()
  {

    const char* glsl_version = "#version 410";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigViewportsNoAutoMerge = false;
    io.ConfigViewportsNoTaskBarIcon = false;

    // Setup Dear ImGui style
    ImGui::StyleColorsClassic();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    GLFWwindow* window = glfwGetCurrentContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load fonts
#ifdef PSTC_PLATFORM_WINDOWS
    ImFont* font = nullptr;
    if (std::filesystem::exists("c:\\Windows\\Fonts\\SegoeUI.ttf"))
      font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\SegoeUI.ttf", 20.0f);
    else if (std::filesystem::exists("c:\\Windows\\Fonts\\ArialUni.ttf"))
      font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL);
    else font = io.Fonts->AddFontDefault();

    IM_ASSERT(font != NULL);
#endif

    // Ini file:
    io.IniFilename = NULL;

  }

  ImGuiRenderer_GLFW_OpenGL::~ImGuiRenderer_GLFW_OpenGL()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
  }

  void ImGuiRenderer_GLFW_OpenGL::BeginRender()
  {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void ImGuiRenderer_GLFW_OpenGL::EndRender()
  {
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }
  }

}

