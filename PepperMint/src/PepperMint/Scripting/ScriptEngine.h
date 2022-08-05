#pragma once

#include <filesystem>
#include <string>

#include "PepperMint/Scene/Entity.h"
#include "PepperMint/Scene/Scene.h"

extern "C" {
typedef struct _MonoClass    MonoClass;
typedef struct _MonoObject   MonoObject;
typedef struct _MonoMethod   MonoMethod;
typedef struct _MonoImage    MonoImage;
typedef struct _MonoAssembly MonoAssembly;
}

namespace PepperMint {

class ScriptEngine {
    friend class ScriptClass;

  public:
    static void Init();
    static void Shutdown();

    static void OnRuntimeStart(Scene* iScene);
    static void OnRuntimeStop();

    static void OnCreateEntity(Entity iEntity);
    static void OnUpdateEntity(Entity iEntity, Timestep iTimestep);

    static bool EntityClassExists(const std::string& iClassName);

    static Scene*     GetSceneContext();
    static MonoImage* GetCoreAssemblyImage();

  private:
    static void InitMono();
    static void ShutdownMono();

    static void LoadAssembly(const std::filesystem::path& iFilePath);
    static void LoadAssemblyClasses(MonoAssembly* iAssembly);

    static MonoObject* InstantiateClass(MonoClass* iMonoClass);
};

class ScriptClass {
  public:
    ScriptClass() = default;
    ScriptClass(const std::string& iClassNamespace, const std::string& iClassName);

    MonoObject* instantiate();
    MonoMethod* get(const std::string& iMethodName, int iMethodParameterCount);
    MonoObject* invoke(MonoObject* iInstance, MonoMethod* iMethod, void** iMethodParameters = nullptr);

  private:
    std::string _classNamespace;
    std::string _className;

    MonoClass* _monoClass = nullptr;
};

class ScriptInstance {
  public:
    ScriptInstance(Ref<ScriptClass> iScriptClass, Entity iEntity);

    void invokeOnCreate();
    void invokeOnUpdate(Timestep iTimestep);

  private:
    Ref<ScriptClass> _scriptClass;

    MonoObject* _instance       = nullptr;
    MonoMethod* _onCreateMethod = nullptr;
    MonoMethod* _onUpdateMethod = nullptr;
};
}
