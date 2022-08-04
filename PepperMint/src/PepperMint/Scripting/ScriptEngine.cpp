#include "pmpch.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

#include "ScriptAPI.h"
#include "ScriptEngine.h"

namespace PepperMint {

namespace {

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
};

static ScriptEngineData sData;

void ScriptEngine::Init() {
    InitMono();
    LoadAssembly("resources/scripts/PepperMint-ScriptCore.dll");

    // Print some types
    printAssemblyTypes(sData.coreAssemblyImage);

    ScriptAPI::RegisterFunctions();

    // Retrieve and instantiate class (with constructor)
    sData.entityClass = ScriptClass("PepperMint", "Entity");
    auto&& instance   = sData.entityClass.instantiate();

    // Call a function
    sData.entityClass.invoke(instance, sData.entityClass.get("PrintHello", 0));

    // Call a function with parameters
    int   value = 5;
    void* param = &value;
    sData.entityClass.invoke(instance, sData.entityClass.get("PrintInt", 1), &param);

    int   value1    = 42;
    int   value2    = 826;
    void* params[2] = {&value1, &value2};
    sData.entityClass.invoke(instance, sData.entityClass.get("PrintInts", 2), params);

    auto&& monoString = mono_string_new(sData.appDomain, "Hello World from C++!");
    void*  strParam   = monoString;
    sData.entityClass.invoke(instance, sData.entityClass.get("PrintMessage", 1), &strParam);
}

void ScriptEngine::Shutdown() { ShutdownMono(); }

//////////////////
// Mono Library //
//////////////////

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

MonoObject* ScriptEngine::InstantiateClass(MonoClass* iMonoClass) {
    auto&& instance = mono_object_new(sData.appDomain, iMonoClass);
    mono_runtime_object_init(instance);
    return instance;
}

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

}
