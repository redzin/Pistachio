#include "pch.h"
#include "ImGuiRenderer.h"

namespace Pistacio
{
  void ImGuiRenderer::Init()
  {
    const char* glsl_version = "#version 410";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    
    // Setup Dear ImGui style
    ImGui::StyleColorsClassic();

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

  void ImGuiRenderer::BeginRender()
  {

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void ImGuiRenderer::EndRender()
  {
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    GLFWwindow* window = glfwGetCurrentContext();
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  }

  void ImGuiRenderer::Cleanup()
  {
    ImGui::DestroyContext();
  }
}

