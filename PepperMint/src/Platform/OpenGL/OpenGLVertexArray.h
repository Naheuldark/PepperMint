#pragma once

#include "PepperMint/Renderer/VertexArray.h"

namespace PepperMint {

class OpenGLVertexArray : public VertexArray {
public:
	OpenGLVertexArray();
	~OpenGLVertexArray() override;

	void bind() const override;
	void unbind() const override;

	void addVertexBuffer(const Ref<VertexBuffer>& iVertexBuffer) override;
	void setIndexBuffer(const Ref<IndexBuffer>& iIndexBuffer) override;

	inline const std::vector<Ref<VertexBuffer>>& vertexBuffers() const override { return _vertexBuffers; }
	inline const Ref<IndexBuffer>& indexBuffer() const override { return _indexBuffer; }

private:
	uint32_t _rendererId;
	std::vector<Ref<VertexBuffer>> _vertexBuffers;
	Ref<IndexBuffer> _indexBuffer;
};

}