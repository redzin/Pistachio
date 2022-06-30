#include <Pistacio.h>

class Sandbox : public Pistacio::Application {
public:
  Sandbox() {}
  ~Sandbox() {}
};

Pistacio::Application* Pistacio::ApplicationFactory::doCreate()
{
  return new Sandbox();
}

