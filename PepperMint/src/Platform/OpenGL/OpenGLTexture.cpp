#include "pmpch.h"

#include "OpenGLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace PepperMint {

OpenGLTexture2D::OpenGLTexture2D(const std::string& iPath) :
	_path(iPath) {

	int width, height, channels;

	stbi_set_flip_vertically_on_load(1);

	stbi_uc* data = stbi_load(iPath.c_str(), &width, &height, &channels, 0);
	PM_CORE_ASSERT(data, "Failed to load the image");

	_width = width;
	_height = height;

	glCreateTextures(GL_TEXTURE_2D, 1, &_rendererId);
	glTextureStorage2D(_rendererId, 1, GL_RGB8, _width, _height);

	glTextureParameteri(_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureSubImage2D(_rendererId, 0, 0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() {
	glDeleteTextures(1, &_rendererId);
}

void OpenGLTexture2D::bind(uint32_t iSlot) const {
	glBindTextureUnit(iSlot, _rendererId);
}
}