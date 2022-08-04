#pragma once

#include <filesystem>
#include <string>

extern "C" {
typedef struct _MonoClass  MonoClass;
typedef struct _MonoObject MonoObject;
typedef struct _MonoMethod MonoMethod;
}

namespace PepperMint {

class ScriptEngine {
    friend class ScriptClass;

  public:
    static void Init();
    static void Shutdown();

  private:
    static void InitMono();
    static void ShutdownMono();
    static void LoadAssembly(const std::filesystem::path& iFilePath);

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
}
