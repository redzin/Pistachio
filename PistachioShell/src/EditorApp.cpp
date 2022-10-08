
#include <Pistachio.h>
#include "EditorLayer.h"
#include "Pistachio/Core/EntryPoint.h"


namespace Pistachio
{

  class PistachioShell : public Pistachio::Application
  {
  public:
    PistachioShell() : Application("Sandbox") {}
    PistachioShell(std::string name, uint32_t width, uint32_t height) : Application(name, width, height, false) {}
    ~PistachioShell() {}
  };

  Pistachio::Application* Pistachio::ApplicationFactory::doCreate()
  {
    EditorLayer* sampleTriangleLayer = new EditorLayer(1280, 720);

    PistachioShell* app = new PistachioShell("Sandbox App", 1280, 720);

    app->AddLayer(sampleTriangleLayer);

    return app;
  }

}



