#pragma once

#include "PepperMint/Renderer/Texture.h"

#include <glad/glad.h>

namespace PepperMint {

class OpenGLTexture2D : public Texture2D {
public:
	OpenGLTexture2D(uint32_t iWidth, uint32_t iHeight);
	OpenGLTexture2D(const std::string& iPath);
	~OpenGLTexture2D() override;

	inline uint32_t width() const override { return _width; }
	inline uint32_t height() const override { return _height; }
	inline uint32_t rendererId() const override { return _rendererId; }

	void setData(void* iData, uint32_t iSize) override;

	void bind(uint32_t iSlot = 0) const override;

	bool operator==(const Texture& iOther) const override {
		return _rendererId == ((OpenGLTexture2D&)iOther)._rendererId;
	}

private:
	std::string _path;
	
	uint32_t _width, _height;
	uint32_t _rendererId;
	GLenum _internalFormat, _dataFormat;
};

}