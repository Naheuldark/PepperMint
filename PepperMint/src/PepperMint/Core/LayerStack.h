#pragma once

#include "Core.h"
#include "Layer.h"

namespace PepperMint {

class LayerStack {
public:
	LayerStack() = default;
	~LayerStack();

	void pushLayer(Ref<Layer> iLayer);
	void pushOverlay(Ref<Layer> iOverlay);
	void popLayer(Ref<Layer> iLayer);
	void popOverlay(Ref<Layer> iOverlay);

	std::vector<Ref<Layer>>::iterator begin() { return _layers.begin(); }
	std::vector<Ref<Layer>>::iterator end() { return _layers.end(); }

private:
	std::vector<Ref<Layer>> _layers;
	size_t _layerInsertIndex = 0;
};

}