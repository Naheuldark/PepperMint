#include "pmpch.h"

#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace PepperMint {

namespace {

void OpenGLMessageCallback(unsigned, unsigned, unsigned, unsigned int iSeverity, int, const char* iMessage, const void*) {
    switch (iSeverity) {
        case GL_DEBUG_SEVERITY_HIGH:
            PM_CORE_FATAL(iMessage);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:
            PM_CORE_ERROR(iMessage);
            return;
        case GL_DEBUG_SEVERITY_LOW:
            PM_CORE_WARN(iMessage);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            PM_CORE_TRACE(iMessage);
            return;
        default:
            PM_CORE_ASSERT(false, "Unknown severity level!");
    }
}
}

void OpenGLRendererAPI::init() {
    PM_PROFILE_FUNCTION();

#ifdef PM_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif // PM_DEBUG

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LINE_SMOOTH);
}

void OpenGLRendererAPI::setViewport(uint32_t iX, uint32_t iY, uint32_t iWidth, uint32_t iHeight) { glViewport(iX, iY, iWidth, iHeight); }

void OpenGLRendererAPI::setClearColor(const glm::vec4& iColor) { glClearColor(iColor.r, iColor.g, iColor.b, iColor.a); }

void OpenGLRendererAPI::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLRendererAPI::drawIndexed(Ref<VertexArray> iVertexArray, uint32_t iIndexCount) {
    iVertexArray->bind();
    auto&& count = iIndexCount ? iIndexCount : iVertexArray->indexBuffer()->count();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::drawLines(Ref<VertexArray> iVertexArray, uint32_t iVertexCount) {
    iVertexArray->bind();
    glDrawArrays(GL_LINES, 0, iVertexCount);
}

void OpenGLRendererAPI::setLineWidth(float iWidth) { glLineWidth(iWidth); }
}
