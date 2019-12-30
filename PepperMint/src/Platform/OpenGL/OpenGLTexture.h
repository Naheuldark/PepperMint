#pragma once

#include "PepperMint/Renderer/Texture.h"

namespace PepperMint {

class OpenGLTexture2D : public Texture2D {
public:
	OpenGLTexture2D(const std::string& iPath);
	~OpenGLTexture2D() override;

	inline uint32_t width() const override { return _width; }
	inline uint32_t height() const override { return _height; }

	void bind(uint32_t iSlot = 0) const override;

private:
	std::string _path;
	
	uint32_t _width, _height;
	uint32_t _rendererId;
};

}