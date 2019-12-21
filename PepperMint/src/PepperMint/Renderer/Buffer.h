#pragma once

#include <cstdint>

namespace PepperMint {

class VertexBuffer {
public:
	virtual ~VertexBuffer() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	static VertexBuffer* Create(float* iVertices, uint32_t  iSize);
};

class IndexBuffer {
public:
	virtual ~IndexBuffer() = default;

	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	virtual uint32_t count() const = 0;

	static IndexBuffer* Create(uint32_t* iIndices, uint32_t iSize);
};

}