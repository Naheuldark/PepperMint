#pragma once

#include <cstdint>

namespace PepperMint {

enum class ShaderDataType : uint8_t {
    NONE = 0,
    BOOL,
    INT,
    INT2,
    INT3,
    INT4,
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    MAT3,
    MAT4,
};

static uint32_t ShaderDataTypeSize(ShaderDataType iType) {
    switch (iType) {
    case ShaderDataType::BOOL:
        return 1;
    case ShaderDataType::INT:
        return 4;
    case ShaderDataType::INT2:
        return 4 * 2;
    case ShaderDataType::INT3:
        return 4 * 3;
    case ShaderDataType::INT4:
        return 4 * 4;
    case ShaderDataType::FLOAT:
        return 4;
    case ShaderDataType::FLOAT2:
        return 4 * 2;
    case ShaderDataType::FLOAT3:
        return 4 * 3;
    case ShaderDataType::FLOAT4:
        return 4 * 4;
    case ShaderDataType::MAT3:
        return 4 * 3 * 3;
    case ShaderDataType::MAT4:
        return 4 * 4 * 4;
    default:
        PM_CORE_ASSERT(false, "Unknown ShaderDataType");
        return 0;
    }
}

struct BufferElement {
    ShaderDataType type;
    std::string    name;
    uint32_t       size;
    size_t         offset;
    bool           normalized;

    BufferElement(ShaderDataType iType, const std::string& iName, bool iNormalized = false)
        : type(iType), name(iName), size(ShaderDataTypeSize(iType)), offset(0),
          normalized(iNormalized) {}

    uint32_t componentCount() const {
        switch (type) {
        case ShaderDataType::BOOL:
            return 1;
        case ShaderDataType::INT:
            return 1;
        case ShaderDataType::INT2:
            return 1 * 2;
        case ShaderDataType::INT3:
            return 1 * 3;
        case ShaderDataType::INT4:
            return 1 * 4;
        case ShaderDataType::FLOAT:
            return 1;
        case ShaderDataType::FLOAT2:
            return 1 * 2;
        case ShaderDataType::FLOAT3:
            return 1 * 3;
        case ShaderDataType::FLOAT4:
            return 1 * 4;
        case ShaderDataType::MAT3:
            return 3; // 3 * FLOAT3
        case ShaderDataType::MAT4:
            return 4; // 4 * FLOAT4
        default:
            PM_CORE_ASSERT(false, "Unknown ShaderDataType");
            return 0;
        }
    }
};

class BufferLayout {
  public:
    BufferLayout() = default;
    BufferLayout(const std::initializer_list<BufferElement>& iElements) : _elements(iElements) {
        computeOffsetAndStride();
    }

    ~BufferLayout() = default;

    inline const std::vector<BufferElement>& elements() const { return _elements; }
    inline uint32_t                          stride() const { return _stride; }

    std::vector<BufferElement>::iterator       begin() { return _elements.begin(); }
    std::vector<BufferElement>::iterator       end() { return _elements.end(); }
    std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return _elements.end(); }

  private:
    void computeOffsetAndStride() {
        size_t offset = 0;
        _stride       = 0;

        for (auto&& element : _elements) {
            element.offset = offset;
            offset += element.size;
            _stride += element.size;
        }
    }

  private:
    std::vector<BufferElement> _elements;
    uint32_t                   _stride = 0;
};

class VertexBuffer {
  public:
    virtual ~VertexBuffer() = default;

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual void setData(void* iData, uint32_t iSize) = 0;

    virtual const BufferLayout& layout() const                         = 0;
    virtual void                setLayout(const BufferLayout& iLayout) = 0;

    static Ref<VertexBuffer> Create(uint32_t iSize);
    static Ref<VertexBuffer> Create(float* iVertices, uint32_t iSize);
};

class IndexBuffer {
  public:
    virtual ~IndexBuffer() = default;

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual uint32_t count() const = 0;

    static Ref<IndexBuffer> Create(uint32_t* iIndices, uint32_t iCount);
};

}
