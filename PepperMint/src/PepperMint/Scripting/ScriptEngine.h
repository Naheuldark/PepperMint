#pragma once

#include <filesystem>
#include <map>
#include <string>

#include "PepperMint/Scene/Entity.h"
#include "PepperMint/Scene/Scene.h"

extern "C" {
typedef struct _MonoClass      MonoClass;
typedef struct _MonoObject     MonoObject;
typedef struct _MonoMethod     MonoMethod;
typedef struct _MonoImage      MonoImage;
typedef struct _MonoAssembly   MonoAssembly;
typedef struct _MonoClassField MonoClassField;
}

namespace PepperMint {

enum class ScriptFieldType {
    NONE = 0,
    FLOAT,
    DOUBLE,
    BOOL,
    CHAR,
    BYTE,
    SHORT,
    INT,
    LONG,
    USHORT,
    UINT,
    ULONG,
    VECTOR2,
    VECTOR3,
    VECTOR4,
    ENTITY,
};

struct ScriptField {
    ScriptFieldType type = ScriptFieldType::NONE;
    std::string     name;

    MonoClassField* classField = nullptr;
};

class ScriptFieldInstance {
    friend class ScriptEngine;
    friend class ScriptInstance;

  public:
    ScriptFieldInstance() { memset(_buffer, 0, sizeof(_buffer)); }

    template <typename Type>
    Type value() {
        static_assert(sizeof(Type) <= 8, "Type is too large!");
        return *(Type*)_buffer;
    }

    template <typename Type>
    void setValue(Type iValue) {
        static_assert(sizeof(Type) <= 8, "Type is too large!");
        memcpy(_buffer, &iValue, sizeof(Type));
    }

  public:
    ScriptField field;

  private:
    ;
    uint8_t _buffer[8];
};

class ScriptClass {
  public:
    ScriptClass() = default;
    ScriptClass(const std::string& iClassNamespace, const std::string& iClassName, bool iIsCore = false);

    MonoObject* instantiate();
    MonoMethod* get(const std::string& iMethodName, int iMethodParameterCount);
    MonoObject* invoke(MonoObject* iInstance, MonoMethod* iMethod, void** iMethodParameters = nullptr);

    const std::map<std::string, ScriptField>& fields() const { return _fields; }
    std::map<std::string, ScriptField>&       fields() { return _fields; }

  private:
    std::string _classNamespace;
    std::string _className;

    std::map<std::string, ScriptField> _fields;

    MonoClass* _monoClass = nullptr;
};

class ScriptInstance {
    friend class ScriptEngine;
    friend class ScriptFieldInstance;

  public:
    ScriptInstance(Ref<ScriptClass> iScriptClass, Entity iEntity);

    void invokeOnCreate();
    void invokeOnUpdate(Timestep iTimestep);

    Ref<ScriptClass> scriptClass() { return _scriptClass; }

    template <typename Type>
    Type fieldValue(const std::string& iName) {
        static_assert(sizeof(Type) <= 8, "Type is too large!");
        if (fieldValueInternal(iName, sFieldValueBuffer)) {
            return *(Type*)sFieldValueBuffer;
        }
        return Type();
    }

    template <typename Type>
    void setFieldValue(const std::string& iName, Type iValue) {
        static_assert(sizeof(Type) <= 8, "Type is too large!");
        setFieldValueInternal(iName, &iValue);
    }

  private:
    bool fieldValueInternal(const std::string& iName, void* oBuffer);
    bool setFieldValueInternal(const std::string& iName, const void* iValue);

  private:
    Ref<ScriptClass> _scriptClass;

    MonoObject* _instance       = nullptr;
    MonoMethod* _onCreateMethod = nullptr;
    MonoMethod* _onUpdateMethod = nullptr;

    inline static char sFieldValueBuffer[8];
};

using ScriptFieldMap    = std::unordered_map<std::string, ScriptFieldInstance>;
using ScriptFieldsMap   = std::unordered_map<UUID, ScriptFieldMap>;
using ScriptClassMap    = std::unordered_map<std::string, Ref<ScriptClass>>;
using ScriptInstanceMap = std::unordered_map<UUID, Ref<ScriptInstance>>;

class ScriptEngine {
    friend class ScriptClass;
    friend class ScriptInstance;

  public:
    static void Init();
    static void Shutdown();

    static void OnRuntimeStart(Scene* iScene);
    static void OnRuntimeStop();

    static void OnCreateEntity(Entity iEntity);
    static void OnUpdateEntity(Entity iEntity, Timestep iTimestep);

    static Scene*              GetSceneContext();
    static Ref<ScriptInstance> GetEntityScriptInstance(UUID iEntityId);

    static bool             EntityClassExists(const std::string& iClassName);
    static Ref<ScriptClass> GetEntityClass(const std::string& iName);
    static ScriptClassMap   GetEntityClasses();

    static ScriptFieldMap& GetScriptFieldMap(Entity iEntity);

    static MonoImage* GetCoreAssemblyImage();

  private:
    static void InitMono();
    static void ShutdownMono();

    static void LoadAssembly(const std::filesystem::path& iFilePath);
    static void LoadAppAssembly(const std::filesystem::path& iFilePath);
    static void LoadAssemblyClasses();

    static MonoObject* InstantiateClass(MonoClass* iMonoClass);
};

}
