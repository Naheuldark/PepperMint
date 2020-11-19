#include <PepperMint.h>
#include <PepperMint/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace PepperMint {

class Menthol : public Application {
  public:
    Menthol() : Application("Menthol") { pushLayer(CreateRef<EditorLayer>()); }
    ~Menthol() = default;
};
}

PepperMint::Application* PepperMint::CreateApplication() { return new PepperMint::Menthol(); }
