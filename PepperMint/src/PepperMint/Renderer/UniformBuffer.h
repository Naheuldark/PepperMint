#pragma once

#include "PepperMint/Core/Base.h"

namespace PepperMint {

class UniformBuffer {
  public:
    UniformBuffer()                   = default;
    virtual ~UniformBuffer() noexcept = default;

    virtual void setData(const void* const iData, uint32_t iSize, uint32_t iOffset = 0) = 0;

    static Ref<UniformBuffer> Create(uint32_t iSize, uint32_t iBinding);
};
}
