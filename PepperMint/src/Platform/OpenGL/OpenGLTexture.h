#pragma once

#include <glad/glad.h>

#include "PepperMint/Renderer/Texture.h"

namespace PepperMint {

class OpenGLTexture2D : public Texture2D {
  public:
    OpenGLTexture2D(uint32_t iWidth, uint32_t iHeight);
    OpenGLTexture2D(const std::string& iPath);
    ~OpenGLTexture2D() override;

    uint32_t           width() const override { return _width; }
    uint32_t           height() const override { return _height; }
    uint32_t           rendererId() const override { return _rendererId; }
    const std::string& path() const override { return _path; }

    void setData(void* iData, uint32_t iSize) override;

    void bind(uint32_t iSlot = 0) const override;

    bool isLoaded() const override { return _isLoaded; }

    bool operator==(const Texture& iOther) const override { return _rendererId == iOther.rendererId(); }

  private:
    std::string _path;

    bool _isLoaded = false;

    uint32_t _width, _height;
    uint32_t _rendererId;
    GLenum   _internalFormat, _dataFormat;
};

}
