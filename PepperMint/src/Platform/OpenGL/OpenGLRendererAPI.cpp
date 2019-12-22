#include "pmpch.h"

#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace PepperMint {

void OpenGLRendererAPI::setClearColor(const glm::vec4& iColor) {
	glClearColor(iColor.r, iColor.g, iColor.b, iColor.a);
}

void OpenGLRendererAPI::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::drawIndexed(const std::shared_ptr<VertexArray>& iVertexArray) {
	glDrawElements(GL_TRIANGLES, iVertexArray->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
}
}