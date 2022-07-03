#include <Pistacio.h>
#include "Pistacio/Core/EntryPoint.h"

class Sandbox : public Pistacio::Application
{
public:
  Sandbox() {}
  ~Sandbox() {}
};

class SampleLayer : public Pistacio::Layer
{
  void OnUpdate()
  {

  }

  void OnGuiRender()
  {

  }

  void OnEvent(Pistacio::Event& e)
  {
    #ifdef PSTC_VERBOSE_DEBUG
      PSTC_TRACE("SampleLayer: {0}", e.GetName());
    #endif
  }

  void OnAttach()
  {
    PSTC_INFO("SampleLayer attached");
  }

  void OnDetach()
  {
    PSTC_INFO("SampleLayer detached");
  }

};

Pistacio::Application* Pistacio::ApplicationFactory::doCreate()
{
  Sandbox* sandbox = new Sandbox();
  SampleLayer* sampleLayer = new SampleLayer();

  sandbox->PushLayer(sampleLayer);

  return sandbox;
}

