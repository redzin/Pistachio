
#include "pch.h"
#include "LayerStack.h"

namespace Pistacio
{

  LayerStack::~LayerStack()
  {
    for (Layer* layer : Layers)
    {
      layer->OnDetach();
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

  void LayerStack::PopOverlay(Layer* layer)
  {
    auto iterator = std::find(Layers.begin(), Layers.begin() + InsertIndex, layer);
    if (iterator != Layers.begin() + InsertIndex)
    {
      layer->OnDetach();
      Layers.erase(iterator);
      InsertIndex--;
    }
  }

  void LayerStack::PopLayer(Layer* overlay)
  {
    auto iterator = std::find(Layers.begin() + InsertIndex, Layers.end(), overlay);
    if (iterator != Layers.end())
    {
      overlay->OnDetach();
      Layers.erase(iterator);
    }
  }

}

