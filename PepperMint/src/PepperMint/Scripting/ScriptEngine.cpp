#include "pmpch.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

#include "PepperMint/Scripting/ScriptAPI.h"
#include "PepperMint/Scripting/ScriptEngine.h"

namespace PepperMint {

namespace {

const std::string kNAMESPACE        = "PepperMint";
const std::string kENTITY_CLASSNAME = "Entity";

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

    *oSize = size;
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
}

struct ScriptEngineData {
    MonoDomain* rootDomain = nullptr;
    MonoDomain* appDomain  = nullptr;

    MonoAssembly* coreAssembly      = nullptr;
    MonoImage*    coreAssemblyImage = nullptr;

    ScriptClass entityClass;

    std::unordered_map<std::string, Ref<ScriptClass>> entityClasses;
    std::unordered_map<UUID, Ref<ScriptInstance>>     entityInstances;

    // Runtime
    Scene* sceneContext;
};

static ScriptEngineData sData;

///////////////////
// Script Engine //
///////////////////

void ScriptEngine::Init() {
    InitMono();
    LoadAssembly("resources/scripts/PepperMint-ScriptCore.dll");
    LoadAssemblyClasses(sData.coreAssembly);

    // Print some types
    printAssemblyTypes(sData.coreAssemblyImage);

    ScriptAPI::RegisterComponents();
    ScriptAPI::RegisterFunctions();

    // Retrieve and instantiate class
    sData.entityClass = ScriptClass(kNAMESPACE, kENTITY_CLASSNAME);
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

void ScriptEngine::LoadAssemblyClasses(MonoAssembly* iAssembly) {
    sData.entityClasses.clear();

    auto&& image                = mono_assembly_get_image(iAssembly);
    auto&& typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    auto&& numTypes             = mono_table_info_get_rows(typeDefinitionsTable);
    auto&& entityClass          = mono_class_from_name(image, kNAMESPACE.c_str(), kENTITY_CLASSNAME.c_str());

    for (size_t i = 0; i < numTypes; i++) {
        uint32_t cols[MONO_TYPEDEF_SIZE];
        mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

        auto&& scope = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
        auto&& name  = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

        std::string fullname;
        if (strlen(scope) != 0) {
            fullname = fmt::format("{}.{}", scope, name);
        } else {
            fullname = name;
        }

        auto&& monoClass = mono_class_from_name(image, scope, name);
        if (monoClass == entityClass) {
            continue;
        }

        bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
        if (isEntity) {
            sData.entityClasses[fullname] = CreateRef<ScriptClass>(scope, name);
        }
    }
}

MonoObject* ScriptEngine::InstantiateClass(MonoClass* iMonoClass) {
    auto&& instance = mono_object_new(sData.appDomain, iMonoClass);
    mono_runtime_object_init(instance);
    return instance;
}

bool ScriptEngine::EntityClassExists(const std::string& iClassName) { return sData.entityClasses.find(iClassName) != sData.entityClasses.end(); }

//////////////////
// Script Class //
//////////////////

ScriptClass::ScriptClass(const std::string& iClassNamespace, const std::string& iClassName)
    : _classNamespace(iClassNamespace), _className(iClassName) {
    _monoClass = mono_class_from_name(sData.coreAssemblyImage, iClassNamespace.c_str(), iClassName.c_str());
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

}
