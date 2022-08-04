#include "pmpch.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

#include "ScriptEngine.h"

namespace PepperMint {

namespace {

char* readBytes(const std::string& iFilePath, uint32_t* oSize) {
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

MonoAssembly* loadAssembly(const std::string& iAssemblyPath) {
    uint32_t filesize = 0;
    auto&&   filedata = readBytes(iAssemblyPath, &filesize);

    MonoImageOpenStatus status;
    auto&&              image = mono_image_open_from_data_full(filedata, filesize, true, &status, false);

    if (status != MONO_IMAGE_OK) {
        auto&& error = mono_image_strerror(status);
        PM_CORE_FATAL(error);
        return nullptr;
    }

    auto&& assembly = mono_assembly_load_from_full(image, iAssemblyPath.c_str(), &status, false);
    mono_image_close(image);

    delete[] filedata;
    return assembly;
}

void printAssemblyTypes(MonoAssembly* iAssembly) {
    auto&& image                = mono_assembly_get_image(iAssembly);
    auto&& typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    auto&& numTypes             = mono_table_info_get_rows(typeDefinitionsTable);

    for (size_t i = 0; i < numTypes; ++i) {
        uint32_t columns[MONO_TYPEDEF_SIZE];
        mono_metadata_decode_row(typeDefinitionsTable, i, columns, MONO_TYPEDEF_SIZE);

        auto&& scope = mono_metadata_string_heap(image, columns[MONO_TYPEDEF_NAMESPACE]);
        auto&& name  = mono_metadata_string_heap(image, columns[MONO_TYPEDEF_NAME]);

        PM_CORE_TRACE("{}.{}", scope, name);
    }
}
}

struct ScriptEngineData {
    MonoDomain* rootDomain = nullptr;
    MonoDomain* appDomain  = nullptr;

    MonoAssembly* coreAssembly = nullptr;
};

static ScriptEngineData sData;

void ScriptEngine::Init() { InitMono(); }

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

    // Create and store an app domain
    auto&& appDomain = mono_domain_create_appdomain("PepperMintScriptRuntime", nullptr);
    PM_CORE_ASSERT(appDomain);
    sData.appDomain = appDomain;

    // Use the app domain
    mono_domain_set(sData.appDomain, true);

    // Create and store the assembly
    auto&& coreAssembly = loadAssembly("resources/scripts/PepperMint-ScriptCore.dll");
    PM_CORE_ASSERT(coreAssembly);
    sData.coreAssembly = coreAssembly;

    // Print some types
    printAssemblyTypes(sData.coreAssembly);

    auto&& monoClass = mono_class_from_name(mono_assembly_get_image(sData.coreAssembly), "PepperMint", "Main");

    // 1. Create an object (and call constructor)
    auto&& instance = mono_object_new(sData.appDomain, monoClass);
    mono_runtime_object_init(instance);

    // 2. Call a function
    auto&& printHelloFunc = mono_class_get_method_from_name(monoClass, "PrintHello", 0);
    mono_runtime_invoke(printHelloFunc, instance, nullptr, nullptr);

    // 3. Call a function with parameters
    auto&& printIntFunc = mono_class_get_method_from_name(monoClass, "PrintInt", 1);

    int   value = 5;
    void* param = &value;
    mono_runtime_invoke(printIntFunc, instance, &param, nullptr);

    // ---

    auto&& printIntsFunc = mono_class_get_method_from_name(monoClass, "PrintInts", 2);

    int   value1    = 42;
    int   value2    = 826;
    void* params[2] = {&value1, &value2};
    mono_runtime_invoke(printIntsFunc, instance, params, nullptr);

    // ---

    auto&& printMessageFunc = mono_class_get_method_from_name(monoClass, "PrintMessage", 1);

    auto&& monoString = mono_string_new(sData.appDomain, "Hello World from C++!");
    void*  strParam   = monoString;
    mono_runtime_invoke(printMessageFunc, instance, &strParam, nullptr);
}

void ScriptEngine::ShutdownMono() {
    sData.appDomain  = nullptr;
    sData.rootDomain = nullptr;
}
}
