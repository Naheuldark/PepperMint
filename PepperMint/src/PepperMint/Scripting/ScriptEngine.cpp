#include "pmpch.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/tabledefs.h>

#include "PepperMint/Scripting/ScriptAPI.h"
#include "PepperMint/Scripting/ScriptEngine.h"

namespace PepperMint {

namespace {

const std::string kNAMESPACE        = "PepperMint";
const std::string kENTITY_CLASSNAME = "Entity";

const std::unordered_map<std::string, ScriptFieldType> kSCRIPT_FIELD_TYPE_MAP = {
    {"System.Single", ScriptFieldType::FLOAT},
    {"System.Double", ScriptFieldType::DOUBLE},
    {"System.Boolean", ScriptFieldType::BOOL},
    {"System.Char", ScriptFieldType::CHAR},
    {"System.Byte", ScriptFieldType::BYTE},
    {"System.Int16", ScriptFieldType::SHORT},
    {"System.Int32", ScriptFieldType::INT},
    {"System.Int64", ScriptFieldType::LONG},
    {"System.UInt16", ScriptFieldType::USHORT},
    {"System.UInt32", ScriptFieldType::UINT},
    {"System.UInt64", ScriptFieldType::ULONG},

    {"PepperMint.Vector2", ScriptFieldType::VECTOR2},
    {"PepperMint.Vector3", ScriptFieldType::VECTOR3},
    {"PepperMint.Vector4", ScriptFieldType::VECTOR4},

    {"PepperMint.Entity", ScriptFieldType::ENTITY},
};

char* readBytes(const std::filesystem::path& iFilePath, uint32_t* oSize) {
    std::ifstream stream(iFilePath, std::ios::binary | std::ios::ate);

    if (!stream) {
        // File cannot be opened
        return nullptr;
    }

    std::streampos end = stream.tellg();
    stream.seekg(0, std::ios::beg);
    uint32_t size = end - stream.tellg();

    if (size == 0) {
        // File is empty
        return nullptr;
    }

    char* buffer = new char[size];
    stream.read((char*)buffer, size);
    stream.close();

    *oSize = (uint32_t)size;
    return buffer;
}

MonoAssembly* loadMonoAssembly(const std::filesystem::path& iAssemblyPath) {
    uint32_t filesize = 0;
    auto&&   filedata = readBytes(iAssemblyPath, &filesize);

    MonoImageOpenStatus status;
    auto&&              image = mono_image_open_from_data_full(filedata, filesize, true, &status, false);

    if (status != MONO_IMAGE_OK) {
        auto&& error = mono_image_strerror(status);
        PM_CORE_FATAL(error);
        return nullptr;
    }

    auto&& assembly = mono_assembly_load_from_full(image, iAssemblyPath.string().c_str(), &status, false);
    mono_image_close(image);

    delete[] filedata;
    return assembly;
}

void printAssemblyTypes(MonoImage* iAssemblyImage) {
    auto&& typeDefinitionsTable = mono_image_get_table_info(iAssemblyImage, MONO_TABLE_TYPEDEF);
    auto&& numTypes             = mono_table_info_get_rows(typeDefinitionsTable);

    for (size_t i = 0; i < numTypes; ++i) {
        uint32_t columns[MONO_TYPEDEF_SIZE];
        mono_metadata_decode_row(typeDefinitionsTable, i, columns, MONO_TYPEDEF_SIZE);

        auto&& scope = mono_metadata_string_heap(iAssemblyImage, columns[MONO_TYPEDEF_NAMESPACE]);
        auto&& name  = mono_metadata_string_heap(iAssemblyImage, columns[MONO_TYPEDEF_NAME]);

        PM_CORE_TRACE("{}.{}", scope, name);
    }
}

ScriptFieldType monoTypeToScriptFieldType(MonoType* iMonoType) {
    auto&& typeName = mono_type_get_name(iMonoType);

    if (kSCRIPT_FIELD_TYPE_MAP.find(typeName) == kSCRIPT_FIELD_TYPE_MAP.end()) {
        PM_CORE_ERROR("Unknown type: {}", typeName);
        return ScriptFieldType::NONE;
    }
    return kSCRIPT_FIELD_TYPE_MAP.at(typeName);
}
}

namespace Utils {

const char* scriptFieldTypeToString(ScriptFieldType iType) {
    switch (iType) {
        case ScriptFieldType::FLOAT:
            return "Float";
        case ScriptFieldType::DOUBLE:
            return "Double";
        case ScriptFieldType::BOOL:
            return "Bool";
        case ScriptFieldType::CHAR:
            return "Char";
        case ScriptFieldType::BYTE:
            return "Byte";
        case ScriptFieldType::SHORT:
            return "Short";
        case ScriptFieldType::INT:
            return "Int";
        case ScriptFieldType::LONG:
            return "Long";
        case ScriptFieldType::USHORT:
            return "UShort";
        case ScriptFieldType::UINT:
            return "UInt";
        case ScriptFieldType::ULONG:
            return "ULong";
        case ScriptFieldType::VECTOR2:
            return "Vector2";
        case ScriptFieldType::VECTOR3:
            return "Vector3";
        case ScriptFieldType::VECTOR4:
            return "Vector4";
        case ScriptFieldType::ENTITY:
            return "Entity";
        default:
            PM_CORE_ASSERT(false, "Unknown ScriptFieldType");
            return "None";
    }
}

ScriptFieldType scriptFieldTypeFromString(std::string_view iType) {
    if (iType == "None") {
        return ScriptFieldType::NONE;
    } else if (iType == "Float") {
        return ScriptFieldType::FLOAT;
    } else if (iType == "Double") {
        return ScriptFieldType::DOUBLE;
    } else if (iType == "Bool") {
        return ScriptFieldType::BOOL;
    } else if (iType == "Char") {
        return ScriptFieldType::CHAR;
    } else if (iType == "Byte") {
        return ScriptFieldType::BYTE;
    } else if (iType == "Short") {
        return ScriptFieldType::SHORT;
    } else if (iType == "Int") {
        return ScriptFieldType::INT;
    } else if (iType == "Long") {
        return ScriptFieldType::LONG;
    } else if (iType == "UShort") {
        return ScriptFieldType::USHORT;
    } else if (iType == "UInt") {
        return ScriptFieldType::UINT;
    } else if (iType == "ULong") {
        return ScriptFieldType::ULONG;
    } else if (iType == "Vector2") {
        return ScriptFieldType::VECTOR2;
    } else if (iType == "Vector3") {
        return ScriptFieldType::VECTOR3;
    } else if (iType == "Vector4") {
        return ScriptFieldType::VECTOR4;
    } else if (iType == "Entity") {
        return ScriptFieldType::ENTITY;
    }

    PM_CORE_ASSERT(false, "Unknown ScriptFieldType");
    return ScriptFieldType::NONE;
}
}

struct ScriptEngineData {
    MonoDomain* rootDomain = nullptr;
    MonoDomain* appDomain  = nullptr;

    MonoAssembly* coreAssembly      = nullptr;
    MonoImage*    coreAssemblyImage = nullptr;
    MonoAssembly* appAssembly       = nullptr;
    MonoImage*    appAssemblyImage  = nullptr;

    ScriptClass entityClass;

    ScriptClassMap    entityClasses;
    ScriptInstanceMap entityInstances;
    ScriptFieldsMap   entityScriptFields;

    // Runtime
    Scene* sceneContext;
};

static ScriptEngineData sData;

//////////////////
// Script Class //
//////////////////

ScriptClass::ScriptClass(const std::string& iClassNamespace, const std::string& iClassName, bool iIsCore)
    : _classNamespace(iClassNamespace), _className(iClassName) {
    _monoClass = mono_class_from_name(iIsCore ? sData.coreAssemblyImage : sData.appAssemblyImage, iClassNamespace.c_str(), iClassName.c_str());
}

MonoObject* ScriptClass::instantiate() { return ScriptEngine::InstantiateClass(_monoClass); }

MonoMethod* ScriptClass::get(const std::string& iMethodName, int iMethodParameterCount) {
    return mono_class_get_method_from_name(_monoClass, iMethodName.c_str(), iMethodParameterCount);
}

MonoObject* ScriptClass::invoke(MonoObject* iInstance, MonoMethod* iMethod, void** iMethodParameters) {
    return mono_runtime_invoke(iMethod, iInstance, iMethodParameters, nullptr);
}

/////////////////////
// Script Instance //
/////////////////////

ScriptInstance::ScriptInstance(Ref<ScriptClass> iScriptClass, Entity iEntity) : _scriptClass(iScriptClass) {
    _instance = iScriptClass->instantiate();

    _onCreateMethod = iScriptClass->get("OnCreate", 0);
    _onUpdateMethod = iScriptClass->get("OnUpdate", 1);

    // Call Entity constructor
    {
        UUID  entityID = iEntity.uuid();
        void* param    = &entityID;
        iScriptClass->invoke(_instance, sData.entityClass.get(".ctor", 1), &param);
    }
}

void ScriptInstance::invokeOnCreate() {
    if (_onCreateMethod) {
        _scriptClass->invoke(_instance, _onCreateMethod);
    }
}

void ScriptInstance::invokeOnUpdate(Timestep iTimestep) {
    if (_onUpdateMethod) {
        void* param = &iTimestep;
        _scriptClass->invoke(_instance, _onUpdateMethod, &param);
    }
}

bool ScriptInstance::fieldValueInternal(const std::string& iName, void* oBuffer) {
    auto&& fields = _scriptClass->fields();
    if (fields.find(iName) == fields.end()) {
        return false;
    }

    auto&& field = _scriptClass->fields().at(iName);
    mono_field_get_value(_instance, field.classField, oBuffer);
    return true;
}

bool ScriptInstance::setFieldValueInternal(const std::string& iName, const void* iValue) {
    auto&& fields = _scriptClass->fields();
    if (fields.find(iName) == fields.end()) {
        return false;
    }

    auto&& field = _scriptClass->fields().at(iName);
    mono_field_set_value(_instance, field.classField, (void*)iValue);
    return true;
}

///////////////////
// Script Engine //
///////////////////

void ScriptEngine::Init() {
    InitMono();
    LoadAssembly("resources/scripts/PepperMint-ScriptCore.dll");
    LoadAppAssembly("../Sandbox/bin/Sandbox.dll");
    LoadAssemblyClasses();

    // Print some types
    printAssemblyTypes(sData.coreAssemblyImage);

    ScriptAPI::RegisterComponents();
    ScriptAPI::RegisterFunctions();

    // Retrieve and instantiate class
    sData.entityClass = ScriptClass(kNAMESPACE, kENTITY_CLASSNAME, true);
}

void ScriptEngine::Shutdown() { ShutdownMono(); }

void ScriptEngine::OnRuntimeStart(Scene* iScene) { sData.sceneContext = iScene; }

void ScriptEngine::OnRuntimeStop() {
    sData.sceneContext = nullptr;
    sData.entityInstances.clear();
}

void ScriptEngine::OnCreateEntity(Entity iEntity) {
    auto&& scriptComponent = iEntity.get<ScriptComponent>();
    if (ScriptEngine::EntityClassExists(scriptComponent.className)) {
        auto&& scriptInstance                 = CreateRef<ScriptInstance>(sData.entityClasses.at(scriptComponent.className), iEntity);
        sData.entityInstances[iEntity.uuid()] = scriptInstance;

        // Copy field values
        if (sData.entityScriptFields.find(iEntity.uuid()) != sData.entityScriptFields.end()) {
            auto&& fields = sData.entityScriptFields.at(iEntity.uuid());
            for (auto&& [name, fieldInstance] : fields) {
                scriptInstance->setFieldValueInternal(name, fieldInstance._buffer);
            }
        }

        scriptInstance->invokeOnCreate();
    }
}

void ScriptEngine::OnUpdateEntity(Entity iEntity, Timestep iTimestep) {
    auto&& uuid = iEntity.uuid();
    PM_CORE_ASSERT(sData.entityInstances.find(uuid) != sData.entityInstances.end());

    auto&& scriptInstance = sData.entityInstances.at(uuid);
    scriptInstance->invokeOnUpdate(iTimestep);
}

Scene* ScriptEngine::GetSceneContext() { return sData.sceneContext; }

MonoImage* ScriptEngine::GetCoreAssemblyImage() { return sData.coreAssemblyImage; }

Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUID iEntityId) {
    if (sData.entityInstances.find(iEntityId) == sData.entityInstances.end()) {
        return nullptr;
    }
    return sData.entityInstances.at(iEntityId);
}

void ScriptEngine::InitMono() {
    mono_set_assemblies_path("mono/lib");

    // Create and store the domain
    auto&& rootDomain = mono_jit_init("PepperMintJITRuntime");
    PM_CORE_ASSERT(rootDomain);
    sData.rootDomain = rootDomain;
}

void ScriptEngine::ShutdownMono() {
    sData.appDomain  = nullptr;
    sData.rootDomain = nullptr;
}

void ScriptEngine::LoadAssembly(const std::filesystem::path& iFilePath) {
    // Create and store an app domain
    auto&& appDomain = mono_domain_create_appdomain("PepperMintScriptRuntime", nullptr);
    PM_CORE_ASSERT(appDomain);
    sData.appDomain = appDomain;

    // Use the app domain
    mono_domain_set(sData.appDomain, true);

    // Create and store the assembly and its image
    auto&& coreAssembly = loadMonoAssembly(iFilePath);
    PM_CORE_ASSERT(coreAssembly);
    sData.coreAssembly = coreAssembly;

    auto&& coreAssemblyImage = mono_assembly_get_image(sData.coreAssembly);
    PM_CORE_ASSERT(coreAssemblyImage);
    sData.coreAssemblyImage = coreAssemblyImage;
}

void ScriptEngine::LoadAppAssembly(const std::filesystem::path& iFilePath) {
    // Create and store the assembly and its image
    auto&& appAssembly = loadMonoAssembly(iFilePath);
    PM_CORE_ASSERT(appAssembly);
    sData.appAssembly = appAssembly;

    auto&& appAssemblyImage = mono_assembly_get_image(sData.appAssembly);
    PM_CORE_ASSERT(appAssemblyImage);
    sData.appAssemblyImage = appAssemblyImage;
}

void ScriptEngine::LoadAssemblyClasses() {
    sData.entityClasses.clear();

    auto&& typeDefinitionsTable = mono_image_get_table_info(sData.appAssemblyImage, MONO_TABLE_TYPEDEF);
    auto&& numTypes             = mono_table_info_get_rows(typeDefinitionsTable);
    auto&& entityClass          = mono_class_from_name(sData.coreAssemblyImage, kNAMESPACE.c_str(), kENTITY_CLASSNAME.c_str());

    for (size_t i = 0; i < numTypes; i++) {
        uint32_t cols[MONO_TYPEDEF_SIZE];
        mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

        auto&& scope     = mono_metadata_string_heap(sData.appAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
        auto&& classname = mono_metadata_string_heap(sData.appAssemblyImage, cols[MONO_TYPEDEF_NAME]);

        std::string fullname;
        if (strlen(scope) != 0) {
            fullname = fmt::format("{}.{}", scope, classname);
        } else {
            fullname = classname;
        }

        auto&& monoClass = mono_class_from_name(sData.appAssemblyImage, scope, classname);
        if (monoClass == entityClass) {
            continue;
        }

        bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
        if (!isEntity) {
            continue;
        }

        auto&& scriptClass            = CreateRef<ScriptClass>(scope, classname);
        sData.entityClasses[fullname] = scriptClass;

        // This routine is an iterator routine for retrieving the fields in a class.
        // You must pass a gpointer that points to zero and is treated as an opaque handle
        // to iterate over all of the elements. When no more values are available, the return value is NULL.

        int fieldCount = mono_class_num_fields(monoClass);
        PM_CORE_WARN("{} has {} fields: ", classname, fieldCount);

        void* it = nullptr;
        while (auto&& field = mono_class_get_fields(monoClass, &it)) {
            auto&&   fieldName = mono_field_get_name(field);
            uint32_t flags     = mono_field_get_flags(field);

            if (flags & FIELD_ATTRIBUTE_PUBLIC) {
                MonoType*       type      = mono_field_get_type(field);
                ScriptFieldType fieldType = monoTypeToScriptFieldType(type);
                PM_CORE_WARN("\t{} ({})", fieldName, Utils::scriptFieldTypeToString(fieldType));

                scriptClass->fields()[fieldName] = {fieldType, fieldName, field};
            }
        }
    }
}

MonoObject* ScriptEngine::InstantiateClass(MonoClass* iMonoClass) {
    auto&& instance = mono_object_new(sData.appDomain, iMonoClass);
    mono_runtime_object_init(instance);
    return instance;
}

bool ScriptEngine::EntityClassExists(const std::string& iClassName) { return sData.entityClasses.find(iClassName) != sData.entityClasses.end(); }

Ref<ScriptClass> ScriptEngine::GetEntityClass(const std::string& iName) {
    if (sData.entityClasses.find(iName) == sData.entityClasses.end()) {
        return nullptr;
    }
    return sData.entityClasses.at(iName);
}

ScriptClassMap ScriptEngine::GetEntityClasses() { return sData.entityClasses; }

ScriptFieldMap& ScriptEngine::GetScriptFieldMap(Entity iEntity) {
    PM_CORE_ASSERT(iEntity);
    return sData.entityScriptFields[iEntity.uuid()];
}
}
