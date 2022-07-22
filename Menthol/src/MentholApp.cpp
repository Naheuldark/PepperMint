#include <PepperMint.h>
#include <PepperMint/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Menthol {

class Menthol : public PepperMint::Application {
  public:
    Menthol(PepperMint::ApplicationCommandLineArgs iArgs) : PepperMint::Application("Menthol", iArgs) { pushLayer(PepperMint::CreateRef<EditorLayer>()); }
    ~Menthol() = default;
};
}

PepperMint::Application* PepperMint::CreateApplication(PepperMint::ApplicationCommandLineArgs iArgs) { return new Menthol::Menthol(iArgs); }
