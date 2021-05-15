#include <PepperMint.h>
#include <PepperMint/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace PepperMint {

class Menthol : public Application {
  public:
    Menthol(PepperMint::ApplicationCommandLineArgs iArgs) : Application("Menthol", iArgs) { pushLayer(CreateRef<EditorLayer>()); }
    ~Menthol() = default;
};
}

PepperMint::Application* PepperMint::CreateApplication(PepperMint::ApplicationCommandLineArgs iArgs) { return new PepperMint::Menthol(iArgs); }
