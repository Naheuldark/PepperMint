#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Core/Timestep.h"

namespace PepperMint {

class Layer {
public:
	Layer(const std::string& iDebugName = "Layer") :
		_debugName(iDebugName) {}
	virtual ~Layer() = default;

	virtual void onAttach() {}
	virtual void onDetach() {}
	virtual void onUpdate(Timestep iTimestep) {}
	virtual void onImGuiRender() {}
	virtual void onEvent(Event& iEvent) {}

	inline const std::string& name() const { return _debugName; }

protected:
	std::string _debugName;
};

}