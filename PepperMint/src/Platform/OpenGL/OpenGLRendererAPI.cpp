#include "pmpch.h"

#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace PepperMint {

void OpenGLRendererAPI::init() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRendererAPI::setClearColor(const glm::vec4& iColor) {
	glClearColor(iColor.r, iColor.g, iColor.b, iColor.a);
}

void OpenGLRendererAPI::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& iVertexArray) {
	glDrawElements(GL_TRIANGLES, iVertexArray->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
}
}