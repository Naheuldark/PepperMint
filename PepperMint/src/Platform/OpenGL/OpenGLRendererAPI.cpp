#include "pmpch.h"

#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace PepperMint {

void OpenGLRendererAPI::init() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::setViewport(uint32_t iX, uint32_t iY, uint32_t iWidth, uint32_t iHeight) {
	glViewport(iX, iY, iWidth, iHeight);
}

void OpenGLRendererAPI::setClearColor(const glm::vec4& iColor) {
	glClearColor(iColor.r, iColor.g, iColor.b, iColor.a);
}

void OpenGLRendererAPI::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::drawIndexed(Ref<VertexArray> iVertexArray) {
	glDrawElements(GL_TRIANGLES, iVertexArray->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
}
}