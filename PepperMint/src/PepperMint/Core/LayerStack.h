#pragma once

#include "PepperMint/Core/Base.h"
#include "PepperMint/Core/Layer.h"

namespace PepperMint {

class LayerStack {
  public:
    LayerStack() = default;
    ~LayerStack();

    void pushLayer(Ref<Layer> iLayer);
    void pushOverlay(Ref<Layer> iOverlay);
    void popLayer(Ref<Layer> iLayer);
    void popOverlay(Ref<Layer> iOverlay);

    std::vector<Ref<Layer>>::iterator         begin() { return _layers.begin(); }
    std::vector<Ref<Layer>>::iterator         end() { return _layers.end(); }
    std::vector<Ref<Layer>>::reverse_iterator rbegin() { return _layers.rbegin(); }
    std::vector<Ref<Layer>>::reverse_iterator rend() { return _layers.rend(); }

    std::vector<Ref<Layer>>::const_iterator         begin() const { return _layers.begin(); }
    std::vector<Ref<Layer>>::const_iterator         end() const { return _layers.end(); }
    std::vector<Ref<Layer>>::const_reverse_iterator rbegin() const { return _layers.rbegin(); }
    std::vector<Ref<Layer>>::const_reverse_iterator rend() const { return _layers.rend(); }

  private:
    std::vector<Ref<Layer>> _layers;
    size_t                  _layerInsertIndex = 0;
};

}
