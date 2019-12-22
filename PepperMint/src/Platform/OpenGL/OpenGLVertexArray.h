#pragma once

#include "PepperMint/Renderer/VertexArray.h"

namespace PepperMint {

class OpenGLVertexArray : public VertexArray {
public:
	OpenGLVertexArray();
	~OpenGLVertexArray() override;

	void bind() const override;
	void unbind() const override;

	void addVertexBuffer(const std::shared_ptr<VertexBuffer>& iVertexBuffer) override;
	void setIndexBuffer(const std::shared_ptr<IndexBuffer>& iIndexBuffer) override;

	inline const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers() const override { return _vertexBuffers; }
	inline const std::shared_ptr<IndexBuffer>& indexBuffer() const override { return _indexBuffer; }

private:
	uint32_t _rendererId;
	std::vector<std::shared_ptr<VertexBuffer>> _vertexBuffers;
	std::shared_ptr<IndexBuffer> _indexBuffer;
};

}