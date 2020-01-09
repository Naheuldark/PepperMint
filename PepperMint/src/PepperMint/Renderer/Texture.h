#pragma once

#include "PepperMint/Core/Core.h"

#include <string>

namespace PepperMint {

class Texture {
public:
	virtual ~Texture() = default;
	
	virtual uint32_t width() const = 0;
	virtual uint32_t height() const = 0;

	virtual void bind(uint32_t iSlot = 0) const = 0;
};

class Texture2D : public Texture {
public:
	static Ref<Texture2D> Create(const std::string& iPath);
};
}