#include "pmpch.h"

#include "LayerStack.h"

namespace PepperMint {

LayerStack::~LayerStack() {
	for (auto&& layer : _layers) {
		delete layer;
	}
}

void LayerStack::pushLayer(Layer* iLayer) {
	_layers.emplace(_layers.begin() + _layerInsertIndex, iLayer);
	_layerInsertIndex++;
	iLayer->onAttach();
}

void LayerStack::pushOverlay(Layer* iOverlay) {
	_layers.emplace_back(iOverlay);
	iOverlay->onAttach();
}

void LayerStack::popLayer(Layer* iLayer) {
	auto it = std::find(_layers.begin(), _layers.end(), iLayer);
	if (it != _layers.end()) {
		iLayer->onDetach();
		_layers.erase(it);
		_layerInsertIndex--;
	}
}

void LayerStack::popOverlay(Layer* iOverlay) {
	auto it = std::find(_layers.begin(), _layers.end(), iOverlay);
	if (it != _layers.end()) {
		iOverlay->onDetach();
		_layers.erase(it);
	}
}

}