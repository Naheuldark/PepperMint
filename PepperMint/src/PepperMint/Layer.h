#pragma once

#include "PepperMint/Core.h"
#include "PepperMint/Events/Event.h"

namespace PepperMint {

class Layer {
public:
	Layer(const std::string& iDebugName = "Layer") :
		_debugName(iDebugName) {}
	virtual ~Layer() = default;

	virtual void onAttach() {}
	virtual void onDetach() {}
	virtual void onUpdate() {}
	virtual void onImGuiRender() {}
	virtual void onEvent(Event& iEvent) {}

	inline const std::string& name() const { return _debugName; }

protected:
	std::string _debugName;
};

}