#include <PepperMint.h>
#include <PepperMint/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Menthol {

class Menthol : public PepperMint::Application {
  public:
    Menthol(const PepperMint::ApplicationSpecification& iSpecification) : PepperMint::Application(iSpecification) {
        pushLayer(PepperMint::CreateRef<EditorLayer>());
    }
    ~Menthol() = default;
};
}

PepperMint::Application* PepperMint::CreateApplication(PepperMint::ApplicationCommandLineArgs iArgs) {
    PepperMint::ApplicationSpecification specification;
    specification.name            = "Menthol Editor";
    specification.commandLineArgs = iArgs;

    return new Menthol::Menthol(specification);
}
