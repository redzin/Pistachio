#include "Pistacio.h"
#include "Pistacio/Core/EntryPoint.h"
#include "ImGuiCounterLayer.h"

class CounterApp : public Pistacio::Application
{
public:
  CounterApp() : Application("Counter App", 320, 40, true) {}
  ~CounterApp() = default;
};


Pistacio::Application* Pistacio::ApplicationFactory::doCreate()
{
  CounterApp* counterApp = new CounterApp();

  counterApp->PushOverlay(new ImGuiCounterLayer());

  return counterApp;
}

