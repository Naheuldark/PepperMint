#include <PepperMint.h>
#include <PepperMint/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Menthol {

class Menthol : public PepperMint::Application {
  public:
    Menthol() : Application("Menthol") { pushLayer(PepperMint::CreateRef<EditorLayer>()); }
    ~Menthol() = default;
};
}

PepperMint::Application* PepperMint::CreateApplication() { return new Menthol::Menthol(); }
