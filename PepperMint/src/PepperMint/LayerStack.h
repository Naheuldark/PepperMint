#pragma once

#include "Core.h"
#include "Layer.h"

namespace PepperMint {

class LayerStack {
public:
	LayerStack() = default;
	~LayerStack();

	void pushLayer(Layer* iLayer);
	void pushOverlay(Layer* iOverlay);
	void popLayer(Layer* iLayer);
	void popOverlay(Layer* iOverlay);

	std::vector<Layer*>::iterator begin() { return _layers.begin(); }
	std::vector<Layer*>::iterator end() { return _layers.end(); }

private:
	std::vector<Layer*> _layers;
	size_t _layerInsertIndex = 0;
};

}