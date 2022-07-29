#include <PepperMint.h>
#include <PepperMint/Core/EntryPoint.h>

#include "Example/ExampleLayer.h"
#include "FlappyShip/FlappyShipLayer.h"
#include "Graph/GraphLayer.h"
#include "Sandbox2D/Sandbox2D.h"

namespace Sandbox {

class Sandbox : public PepperMint::Application {
  public:
    Sandbox(const PepperMint::ApplicationSpecification& iSpecification) : PepperMint::Application(iSpecification) {
        // pushLayer(PepperMint::CreateRef<ExampleLayer>());
        pushLayer(PepperMint::CreateRef<Sandbox2D>());

        // pushLayer(PepperMint::CreateRef<FlappyShipLayer>());
        // pushLayer(PepperMint::CreateRef<GraphLayer>());
    }

    ~Sandbox() = default;
};
}

PepperMint::Application* PepperMint::CreateApplication(PepperMint::ApplicationCommandLineArgs iArgs) {
    PepperMint::ApplicationSpecification spec;
    spec.name             = "Sandbox";
    spec.workingDirectory = "../Menthol";
    spec.commandLineArgs  = iArgs;

    return new Sandbox::Sandbox(spec);
}
