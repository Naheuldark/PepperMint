#include <PepperMint.h>
#include <PepperMint/Core/EntryPoint.h>

#include "EditorLayer.h"

using namespace PepperMint;

namespace Menthol {

class Menthol : public Application {
  public:
    Menthol() : Application("Menthol") { pushLayer(CreateRef<EditorLayer>()); }
    ~Menthol() = default;
};
}

Application* PepperMint::CreateApplication() { return new Menthol::Menthol(); }
