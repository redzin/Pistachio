#include "Pistachio.h"
#include "Pistachio/Core/EntryPoint.h"
#include "ImGuiCounterLayer.h"

class CounterApp : public Pistachio::Application
{
public:
  CounterApp() : Application("Counter App", 320, 40, true) {}
  ~CounterApp() = default;
};


Pistachio::Application* Pistachio::ApplicationFactory::doCreate()
{
  CounterApp* counterApp = new CounterApp();

  counterApp->AddOverlay(new ImGuiCounterLayer());

  return counterApp;
}

