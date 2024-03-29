
#include "pch.h"
#include "LayerStack.h"

namespace Pistachio
{

  LayerStack::~LayerStack()
  {
    for (Layer* layer : Layers)
    {
      delete layer;
    }
  }

  void LayerStack::PushLayer(Layer* layer)
  {
    Layers.emplace(Layers.begin() + InsertIndex, layer);
    InsertIndex++;
  }

  void LayerStack::PushOverlay(Layer* layer)
  {
    Layers.push_back(layer);
  }

  void LayerStack::PopOverlay(Layer* layer, Window& window, EventLibrary& EventLib)
  {
    auto iterator = std::find(Layers.begin(), Layers.begin() + InsertIndex, layer);
    if (iterator != Layers.begin() + InsertIndex)
    {
      layer->OnDetach(window, EventLib);
      Layers.erase(iterator);
      InsertIndex--;
    }
  }

  void LayerStack::PopLayer(Layer* overlay, Window& window, EventLibrary& EventLib)
  {
    auto iterator = std::find(Layers.begin() + InsertIndex, Layers.end(), overlay);
    if (iterator != Layers.end())
    {
      overlay->OnDetach(window, EventLib);
      Layers.erase(iterator);
    }
  }

}

