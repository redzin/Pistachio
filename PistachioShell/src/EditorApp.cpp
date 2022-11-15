
#include <Pistachio.h>
#include "EditorLayer.h"
#include "Pistachio/Core/EntryPoint.h"


namespace Pistachio
{

  class PistachioShell : public Pistachio::Application
  {
  public:
    PistachioShell() : Application("Pistachio Editor") {}
    PistachioShell(std::string name, uint32_t width, uint32_t height) : Application(name, width, height, false) {}
    ~PistachioShell() {}
  };

  Pistachio::Application* Pistachio::ApplicationFactory::doCreate()
  {
    EditorLayer* editorLayer = new EditorLayer();

    PistachioShell* app = new PistachioShell("Pistachio Editor", 1280, 720);

    app->AddLayer(editorLayer);

    return app;
  }

}



