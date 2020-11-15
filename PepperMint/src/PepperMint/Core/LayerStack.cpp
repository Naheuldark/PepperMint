#include "pmpch.h"

#include "PepperMint/Core/LayerStack.h"

namespace PepperMint {

LayerStack::~LayerStack() {
    for (auto&& layer : _layers) {
        layer->onDetach();
    }
}

void LayerStack::pushLayer(Ref<Layer> iLayer) {
    _layers.emplace(_layers.begin() + _layerInsertIndex, iLayer);
    _layerInsertIndex++;
}

void LayerStack::pushOverlay(Ref<Layer> iOverlay) { _layers.emplace_back(iOverlay); }

void LayerStack::popLayer(Ref<Layer> iLayer) {
    auto it = std::find(_layers.begin(), _layers.end(), iLayer);
    if (it != _layers.begin() + _layerInsertIndex) {
        iLayer->onDetach();
        _layers.erase(it);
        _layerInsertIndex--;
    }
}

void LayerStack::popOverlay(Ref<Layer> iOverlay) {
    auto it = std::find(_layers.begin(), _layers.end(), iOverlay);
    if (it != _layers.end()) {
        iOverlay->onDetach();
        _layers.erase(it);
    }
}

}
