#pragma once

#include "PepperMint/Core/Base.h"
#include "PepperMint/Scene/Scene.h"

namespace PepperMint {

class SceneSerializer {
  public:
    SceneSerializer(const Ref<Scene>& iScene) : _scene(iScene) {}
    ~SceneSerializer() = default;

    // Serialize / Deserialize functions
    void serialize(const std::string& iFilepath);
    bool deserialize(const std::string& iFilepath);

    void serializeRuntime(const std::string& iFilepath);
    bool deserializeRuntime(const std::string& iFilepath);

  private:
    Ref<Scene> _scene;
};
}
