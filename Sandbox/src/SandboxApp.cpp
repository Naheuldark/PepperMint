#include <PepperMint.h>
#include <PepperMint/Core/EntryPoint.h>

#include "Sandbox2D/Sandbox2D.h"
#include "Example/ExampleLayer.h"
#include "FlappyShip/FlappyShipLayer.h"
#include "Graph/GraphLayer.h"

class Sandbox : public PepperMint::Application {
public:
	Sandbox() { 
		//pushLayer(PepperMint::CreateRef<ExampleLayer>());
		pushLayer(PepperMint::CreateRef<Sandbox2D>());
		
		//pushLayer(PepperMint::CreateRef<FlappyShipLayer>());
		//pushLayer(PepperMint::CreateRef<GraphLayer>());
	}

	~Sandbox() = default;
};

PepperMint::Application* PepperMint::CreateApplication() {
	return new Sandbox();
}