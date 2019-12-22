#include "pmpch.h"

#include "Renderer.h"

namespace PepperMint {

void Renderer::BeginScene() {}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<VertexArray>& iVertexArray) {
	iVertexArray->bind();
	RenderCommand::DrawIndexed(iVertexArray);
}
}