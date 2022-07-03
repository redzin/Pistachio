#pragma once

#include "pch.h"
#include "Layer.h"

namespace Pistacio
{
  class LayerStack
  {

  public:
    LayerStack() = default;
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);
    void PopOverlay(Layer* layer);
    void PopLayer(Layer* layer);

    std::vector<Layer*>::iterator begin() { return Layers.begin(); }
    std::vector<Layer*>::iterator end() { return Layers.end(); }
    std::vector<Layer*>::reverse_iterator rbegin() { return Layers.rbegin(); }
    std::vector<Layer*>::reverse_iterator rend() { return Layers.rend(); }

    std::vector<Layer*>::const_iterator begin() const { return Layers.begin(); }
    std::vector<Layer*>::const_iterator end() const { return Layers.end(); }
    std::vector<Layer*>::const_reverse_iterator rbegin() const { return Layers.rbegin(); }
    std::vector<Layer*>::const_reverse_iterator rend() const { return Layers.rend(); }

  private:
    std::vector<Layer*> Layers;
    unsigned int InsertIndex = 0;
  };
}

