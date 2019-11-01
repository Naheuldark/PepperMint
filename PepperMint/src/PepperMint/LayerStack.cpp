#include "pmpch.h"

#include "LayerStack.h"

namespace PepperMint {

LayerStack::LayerStack() {
	_layerInsert = _layers.begin();
}

LayerStack::~LayerStack() {
	for (auto&& layer : _layers) {
		delete layer;
	}
}

void LayerStack::pushLayer(Layer* iLayer) {
	_layerInsert = _layers.emplace(_layerInsert, iLayer);
}

void LayerStack::pushOverlay(Layer* iOverlay) {
	_layers.emplace_back(iOverlay);
}

void LayerStack::popLayer(Layer* iLayer) {
	auto it = std::find(_layers.begin(), _layers.end(), iLayer);
	if (it != _layers.end()) {
		_layers.erase(it);
		_layerInsert--;
	}
}

void LayerStack::popOverlay(Layer* iOverlay) {
	auto it = std::find(_layers.begin(), _layers.end(), iOverlay);
	if (it != _layers.end()) {
		_layers.erase(it);
	}
}

}