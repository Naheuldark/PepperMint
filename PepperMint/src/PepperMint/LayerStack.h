#pragma once

#include "pmpch.h"

#include "PepperMint/Core.h"
#include "PepperMint/Layer.h"

namespace PepperMint {

class PM_API LayerStack {
public:
	LayerStack();
	~LayerStack();

	void pushLayer(Layer* iLayer);
	void pushOverlay(Layer* iOverlay);
	void popLayer(Layer* iLayer);
	void popOverlay(Layer* iOverlay);

	std::vector<Layer*>::iterator begin() { return _layers.begin(); }
	std::vector<Layer*>::iterator end() { return _layers.end(); }

private:
	std::vector<Layer*> _layers;
	std::vector<Layer*>::iterator _layerInsert;
};

}