#include "pmpch.h"

#include <glm/gtc/matrix_transform.hpp>

#include "PepperMint/Renderer/RenderCommand.h"
#include "PepperMint/Renderer/Renderer2D.h"
#include "PepperMint/Renderer/Shader.h"
#include "PepperMint/Renderer/VertexArray.h"

namespace PepperMint {

struct QuadVertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoord;
    float     texIndex;
    float     tilingFactor;

    // Editor only
    int entityId;
};

struct Renderer2DData {
    static const uint32_t MAX_QUADS         = 20000;
    static const uint32_t MAX_VERTICES      = MAX_QUADS * 4;
    static const uint32_t MAX_INDICES       = MAX_QUADS * 6;
    static const uint32_t MAX_TEXTURE_SLOTS = 32; // TODO Render capacity

    Ref<VertexArray>  quadVertexArray;
    Ref<VertexBuffer> quadVertexBuffer;
    Ref<Shader>       textureShader;
    Ref<Texture2D>    whiteTexture;

    uint32_t    quadIndexCount       = 0;
    QuadVertex* quadVertexBufferBase = nullptr;
    QuadVertex* quadVertexBufferPtr  = nullptr;

    std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS> textureSlots;
    uint32_t                                      textureSlotIndex = 1; // 0 = white texture

    glm::vec4 quadVertexPositions[4]{};

    Renderer2D::Statistics stats{};
};

static Renderer2DData sData;

void Renderer2D::Init() {
    PM_PROFILE_FUNCTION();

    //////////
    // Quad //
    //////////

    // Vertex Array
    sData.quadVertexArray = VertexArray::Create();

    // Vertex Buffer
    sData.quadVertexBuffer = VertexBuffer::Create(Renderer2DData::MAX_VERTICES * sizeof(QuadVertex));
    sData.quadVertexBuffer->setLayout({
        {ShaderDataType::FLOAT3, "iPosition"},
        {ShaderDataType::FLOAT4, "iColor"},
        {ShaderDataType::FLOAT2, "iTexCoord"},
        {ShaderDataType::FLOAT, "iTexIndex"},
        {ShaderDataType::FLOAT, "iTilingFactor"},
        {ShaderDataType::INT, "iEntityId"},
    });
    sData.quadVertexArray->addVertexBuffer(sData.quadVertexBuffer);

    sData.quadVertexBufferBase = new QuadVertex[Renderer2DData::MAX_VERTICES];

    // Index Buffer
    uint32_t* quadIndices = new uint32_t[Renderer2DData::MAX_INDICES];
    uint32_t  offset      = 0;

    for (uint32_t i = 0; i < Renderer2DData::MAX_INDICES; i += 6) {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }

    Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, Renderer2DData::MAX_INDICES);
    sData.quadVertexArray->setIndexBuffer(quadIndexBuffer);
    delete[] quadIndices;

    //////////////
    // Textures //
    //////////////

    sData.whiteTexture        = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    sData.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

    int32_t samplers[Renderer2DData::MAX_TEXTURE_SLOTS];
    for (uint32_t i = 0; i < Renderer2DData::MAX_TEXTURE_SLOTS; ++i) {
        samplers[i] = i;
    }

    // Set first texture slot to 0
    sData.textureSlots[0] = sData.whiteTexture;

    //////////////
    // Position //
    //////////////

    sData.quadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
    sData.quadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
    sData.quadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
    sData.quadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};

    /////////////
    // Shaders //
    /////////////

    sData.textureShader = Shader::Create("assets/shaders/Texture.glsl");
    sData.textureShader->bind();
    sData.textureShader->setIntArray("uTextures", samplers, Renderer2DData::MAX_TEXTURE_SLOTS);
}

void Renderer2D::Shutdown() {
    PM_PROFILE_FUNCTION();

    delete[] sData.quadVertexBufferBase;
}

void Renderer2D::BeginScene(const Camera& iCamera, const glm::mat4& iTransform) {
    PM_PROFILE_FUNCTION();

    glm::mat4 viewProjectionMatrix = iCamera.projection() * glm::inverse(iTransform);

    sData.textureShader->bind();
    sData.textureShader->setMat4("uViewProjection", viewProjectionMatrix);

    StartBatch();
}

void Renderer2D::BeginScene(const EditorCamera& iCamera) {
    PM_PROFILE_FUNCTION();

    glm::mat4 viewProjectionMatrix = iCamera.viewProjection();

    sData.textureShader->bind();
    sData.textureShader->setMat4("uViewProjection", viewProjectionMatrix);

    StartBatch();
}

void Renderer2D::EndScene() {
    PM_PROFILE_FUNCTION();

    Flush();
}

void Renderer2D::Flush() {
    if (sData.quadIndexCount == 0) {
        return; // Nothing to draw
    }

    uint32_t dataSize = (uint32_t)((uint8_t*)sData.quadVertexBufferPtr - (uint8_t*)sData.quadVertexBufferBase);
    sData.quadVertexBuffer->setData(sData.quadVertexBufferBase, dataSize);

    // Bind textures
    for (uint32_t i = 0; i < sData.textureSlotIndex; ++i) {
        sData.textureSlots[i]->bind(i);
    }

    RenderCommand::DrawIndexed(sData.quadVertexArray, sData.quadIndexCount);

    sData.stats.drawCalls++;
}

void Renderer2D::StartBatch() {
    sData.quadIndexCount      = 0;
    sData.quadVertexBufferPtr = sData.quadVertexBufferBase;

    sData.textureSlotIndex = 1;
}

void Renderer2D::NextBatch() {
    Flush();
    StartBatch();
}

void Renderer2D::ResetStats() { memset(&sData.stats, 0, sizeof(Statistics)); }

Renderer2D::Statistics Renderer2D::Stats() { return sData.stats; }

////////////////
// Primitives //
////////////////
void Renderer2D::DrawQuad(const glm::vec2& iPosition,
                          float            iRotation,
                          const glm::vec2& iScale,
                          float            iTilingFactor,
                          Ref<Texture2D>   iTexture,
                          const glm::vec4& iColor,
                          int              iEntityId) {
    DrawQuad({iPosition.x, iPosition.y, 0.0f}, iRotation, iScale, iTilingFactor, iTexture, iColor, iEntityId);
}

void Renderer2D::DrawQuad(const glm::vec3& iPosition,
                          float            iRotation,
                          const glm::vec2& iScale,
                          float            iTilingFactor,
                          Ref<Texture2D>   iTexture,
                          const glm::vec4& iColor,
                          int              iEntityId) {
    auto&& transform = glm::translate(glm::mat4(1.0f), iPosition) * glm::rotate(glm::mat4(1.0f), glm::radians(iRotation), {0.0f, 0.0f, 1.0f}) *
                       glm::scale(glm::mat4(1.0f), {iScale.x, iScale.y, 1.0f});
    DrawQuad(transform, iTilingFactor, iTexture, iColor, iEntityId);
}

void Renderer2D::DrawQuad(const glm::mat4& iTransform, float iTilingFactor, Ref<Texture2D> iTexture, const glm::vec4& iColor, int iEntityId) {
    PM_PROFILE_FUNCTION();

    constexpr size_t    quadVertexCount = 4;
    constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

    if (sData.quadIndexCount >= Renderer2DData::MAX_INDICES) {
        NextBatch();
    }

    float textureIndex = 0.0f; // White texture (default)
    if (iTexture) {
        for (uint32_t i = 1; i < sData.textureSlotIndex; i++) {
            if (*sData.textureSlots[i] == *iTexture) {
                textureIndex = (float)i;
                break;
            }
        }

        // Add the new texture if it does not exist yet
        if (textureIndex == 0.0f) {
            if (sData.textureSlotIndex >= Renderer2DData::MAX_TEXTURE_SLOTS) {
                NextBatch();
            }

            textureIndex                               = (float)sData.textureSlotIndex;
            sData.textureSlots[sData.textureSlotIndex] = iTexture;
            sData.textureSlotIndex++;
        }
    }

    for (size_t i = 0; i < quadVertexCount; i++) {
        sData.quadVertexBufferPtr->position     = iTransform * sData.quadVertexPositions[i];
        sData.quadVertexBufferPtr->color        = iColor;
        sData.quadVertexBufferPtr->texCoord     = textureCoords[i];
        sData.quadVertexBufferPtr->texIndex     = textureIndex;
        sData.quadVertexBufferPtr->tilingFactor = iTilingFactor;
        sData.quadVertexBufferPtr->entityId     = iEntityId;
        sData.quadVertexBufferPtr++;
    }

    sData.quadIndexCount += 6;

    sData.stats.quadCount++;
}

////////////////
// Components //
////////////////
void Renderer2D::DrawSprite(const TransformComponent& iTransformComponent, const SpriteRendererComponent& iSpriteComponent, int iEntityId) {
    DrawQuad(iTransformComponent.transform(), 1.0f, nullptr, iSpriteComponent.color, iEntityId);
}
}
