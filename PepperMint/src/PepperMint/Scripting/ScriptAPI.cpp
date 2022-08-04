#include "pmpch.h"

#include <mono/metadata/object.h>

#include "ScriptAPI.h"

#define PM_ADD_INTERNAL_CALL(method) mono_add_internal_call("PepperMint.InternalCalls::" #method, method)

namespace PepperMint {

namespace {

void NativeLog(MonoString* iMessage, int iValue) {
    auto&&      cstr = mono_string_to_utf8(iMessage);
    std::string str(cstr);
    mono_free(cstr);
    PM_CORE_TRACE("{0}, {1}", str, iValue);
}

void NativeLog_Vector(glm::vec3* iVector, glm::vec3* oResult) {
    PM_CORE_TRACE("Vector: {0}", *iVector);
    *oResult = glm::normalize(*iVector);
}

float NativeLog_VectorDot(glm::vec3* iVector) {
    PM_CORE_TRACE("Vector: {0}", *iVector);
    return glm::dot(*iVector, *iVector);
}
}

void ScriptAPI::RegisterFunctions() {
    PM_ADD_INTERNAL_CALL(NativeLog);
    PM_ADD_INTERNAL_CALL(NativeLog_Vector);
    PM_ADD_INTERNAL_CALL(NativeLog_VectorDot);
}
}
