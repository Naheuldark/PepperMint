#include "pmpch.h"

#include <stb_image.h>

#include "Platform/OpenGL/OpenGLTexture.h"

namespace PepperMint {

OpenGLTexture2D::OpenGLTexture2D(uint32_t iWidth, uint32_t iHeight) : _width(iWidth), _height(iHeight) {
    PM_PROFILE_FUNCTION();

    _internalFormat = GL_RGBA8;
    _dataFormat     = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &_rendererId);
    glTextureStorage2D(_rendererId, 1, _internalFormat, _width, _height);

    glTextureParameteri(_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& iPath) : _path(iPath) {
    PM_PROFILE_FUNCTION();

    int width, height, channels;

    stbi_set_flip_vertically_on_load(1);

    stbi_uc* data = nullptr;
    {
        PM_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
        data = stbi_load(iPath.c_str(), &width, &height, &channels, 0);
    }

    if (data) {
        _isLoaded = true;

        _width  = width;
        _height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat     = GL_RGBA;
        } else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat     = GL_RGB;
        }

        _internalFormat = internalFormat;
        _dataFormat     = dataFormat;

        PM_CORE_ASSERT(internalFormat & dataFormat, "Image format not supported");

        glCreateTextures(GL_TEXTURE_2D, 1, &_rendererId);
        glTextureStorage2D(_rendererId, 1, internalFormat, _width, _height);

        glTextureParameteri(_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(_rendererId, 0, 0, 0, _width, _height, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }
}

OpenGLTexture2D::~OpenGLTexture2D() {
    PM_PROFILE_FUNCTION();

    glDeleteTextures(1, &_rendererId);
}

void OpenGLTexture2D::setData(void* iData, uint32_t iSize) {
    PM_PROFILE_FUNCTION();

    uint32_t bpp = (_dataFormat == GL_RGBA ? 4 : 3);
    PM_CORE_ASSERT(iSize == _width * _height * bpp, "Data must be entire texture!");
    glTextureSubImage2D(_rendererId, 0, 0, 0, _width, _height, _dataFormat, GL_UNSIGNED_BYTE, iData);
}

void OpenGLTexture2D::bind(uint32_t iSlot) const {
    PM_PROFILE_FUNCTION();

    glBindTextureUnit(iSlot, _rendererId);
}
}
