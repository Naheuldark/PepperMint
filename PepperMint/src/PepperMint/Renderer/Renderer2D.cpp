#include "pmpch.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PepperMint/Renderer/RenderCommand.h"
#include "PepperMint/Renderer/Renderer2D.h"
#include "PepperMint/Renderer/Shader.h"
#include "PepperMint/Renderer/UniformBuffer.h"
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

struct CircleVertex {
    glm::vec3 worldPosition;
    glm::vec3 localPosition;
    glm::vec4 color;
    float     thickness;
    float     fade;

    // Editor only
    int entityId;
};

struct LineVertex {
    glm::vec3 position;
    glm::vec4 color;

    // Editor only
    int entityId;
};

struct Renderer2DData {
    static const uint32_t MAX_QUADS         = 20000;
    static const uint32_t MAX_VERTICES      = MAX_QUADS * 4;
    static const uint32_t MAX_INDICES       = MAX_QUADS * 6;
    static const uint32_t MAX_TEXTURE_SLOTS = 32; // TODO Render capacity

    Ref<Texture2D> whiteTexture;

    // Quad Renderer Data
    Ref<VertexArray>  quadVertexArray;
    Ref<VertexBuffer> quadVertexBuffer;
    Ref<Shader>       quadShader;

    uint32_t    quadIndexCount       = 0;
    QuadVertex* quadVertexBufferBase = nullptr;
    QuadVertex* quadVertexBufferPtr  = nullptr;
    ////

    // Circle Renderer Data
    Ref<VertexArray>  circleVertexArray;
    Ref<VertexBuffer> circleVertexBuffer;
    Ref<Shader>       circleShader;

    uint32_t      circleIndexCount       = 0;
    CircleVertex* circleVertexBufferBase = nullptr;
    CircleVertex* circleVertexBufferPtr  = nullptr;
    ////

    // Line Renderer Data
    Ref<VertexArray>  lineVertexArray;
    Ref<VertexBuffer> lineVertexBuffer;
    Ref<Shader>       lineShader;

    uint32_t    lineVertexCount      = 0;
    LineVertex* lineVertexBufferBase = nullptr;
    LineVertex* lineVertexBufferPtr  = nullptr;

    float LINE_WIDTH = 1.0f;
    ////

    std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS> textureSlots;
    uint32_t                                      textureSlotIndex = 1; // 0 = white texture

    glm::vec4 quadVertexPositions[4]{};

    Renderer2D::Statistics stats{};

    struct CameraData {
        glm::mat4 viewProjection;
    };
    CameraData         cameraBuffer;
    Ref<UniformBuffer> cameraUniformBuffer;
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

    ////////////
    // Circle //
    ////////////

    // Vertex Array
    sData.circleVertexArray = VertexArray::Create();

    // Vertex Buffer
    sData.circleVertexBuffer = VertexBuffer::Create(Renderer2DData::MAX_VERTICES * sizeof(CircleVertex));
    sData.circleVertexBuffer->setLayout({
        {ShaderDataType::FLOAT3, "iWorldPosition"},
        {ShaderDataType::FLOAT3, "iLocalPosition"},
        {ShaderDataType::FLOAT4, "iColor"},
        {ShaderDataType::FLOAT, "iThickness"},
        {ShaderDataType::FLOAT, "iFade"},
        {ShaderDataType::INT, "iEntityId"},
    });
    sData.circleVertexArray->addVertexBuffer(sData.circleVertexBuffer);
    sData.circleVertexArray->setIndexBuffer(quadIndexBuffer); // Use same index buffer as for quad

    sData.circleVertexBufferBase = new CircleVertex[Renderer2DData::MAX_VERTICES];

    //////////
    // Line //
    //////////

    // Vertex Array
    sData.lineVertexArray = VertexArray::Create();

    // Vertex Buffer
    sData.lineVertexBuffer = VertexBuffer::Create(Renderer2DData::MAX_VERTICES * sizeof(LineVertex));
    sData.lineVertexBuffer->setLayout({
        {ShaderDataType::FLOAT3, "iPosition"},
        {ShaderDataType::FLOAT4, "iColor"},
        {ShaderDataType::INT, "iEntityId"},
    });
    sData.lineVertexArray->addVertexBuffer(sData.lineVertexBuffer);

    sData.lineVertexBufferBase = new LineVertex[Renderer2DData::MAX_VERTICES];

    //////////////
    // Textures //
    //////////////

    sData.whiteTexture        = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    sData.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

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

    sData.quadShader   = Shader::Create("assets/shaders/Renderer2D_Quad.glsl");
    sData.circleShader = Shader::Create("assets/shaders/Renderer2D_Circle.glsl");
    sData.lineShader   = Shader::Create("assets/shaders/Renderer2D_Line.glsl");

    sData.cameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
}

void Renderer2D::Shutdown() {
    PM_PROFILE_FUNCTION();

    delete[] sData.quadVertexBufferBase;
}

void Renderer2D::BeginScene(const Camera& iCamera, const glm::mat4& iTransform) {
    PM_PROFILE_FUNCTION();

    sData.cameraBuffer.viewProjection = iCamera.projection() * glm::inverse(iTransform);
    sData.cameraUniformBuffer->setData(&sData.cameraBuffer, sizeof(Renderer2DData::CameraData));

    StartBatch();
}

void Renderer2D::BeginScene(const EditorCamera& iCamera) {
    PM_PROFILE_FUNCTION();

    sData.cameraBuffer.viewProjection = iCamera.viewProjection();
    sData.cameraUniformBuffer->setData(&sData.cameraBuffer, sizeof(Renderer2DData::CameraData));

    StartBatch();
}

void Renderer2D::EndScene() {
    PM_PROFILE_FUNCTION();

    Flush();
}

void Renderer2D::Flush() {
    if (sData.quadIndexCount > 0) {
        uint32_t dataSize = (uint32_t)((uint8_t*)sData.quadVertexBufferPtr - (uint8_t*)sData.quadVertexBufferBase);
        sData.quadVertexBuffer->setData(sData.quadVertexBufferBase, dataSize);

        // Bind textures
        for (uint32_t i = 0; i < sData.textureSlotIndex; ++i) {
            sData.textureSlots[i]->bind(i);
        }

        sData.quadShader->bind();
        RenderCommand::DrawIndexed(sData.quadVertexArray, sData.quadIndexCount);

        sData.stats.drawCalls++;
    }

    if (sData.circleIndexCount > 0) {
        uint32_t dataSize = (uint32_t)((uint8_t*)sData.circleVertexBufferPtr - (uint8_t*)sData.circleVertexBufferBase);
        sData.circleVertexBuffer->setData(sData.circleVertexBufferBase, dataSize);

        sData.circleShader->bind();
        RenderCommand::DrawIndexed(sData.circleVertexArray, sData.circleIndexCount);

        sData.stats.drawCalls++;
    }

    if (sData.lineVertexCount > 0) {
        uint32_t dataSize = (uint32_t)((uint8_t*)sData.lineVertexBufferPtr - (uint8_t*)sData.lineVertexBufferBase);
        sData.lineVertexBuffer->setData(sData.lineVertexBufferBase, dataSize);

        sData.lineShader->bind();
        RenderCommand::SetLineWidth(sData.LINE_WIDTH);
        RenderCommand::DrawLines(sData.lineVertexArray, sData.lineVertexCount);

        sData.stats.drawCalls++;
    }
}

void Renderer2D::StartBatch() {
    sData.quadIndexCount      = 0;
    sData.quadVertexBufferPtr = sData.quadVertexBufferBase;

    sData.circleIndexCount      = 0;
    sData.circleVertexBufferPtr = sData.circleVertexBufferBase;

    sData.lineVertexCount     = 0;
    sData.lineVertexBufferPtr = sData.lineVertexBufferBase;

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

void Renderer2D::DrawCircle(const glm::mat4& iTransform, const glm::vec4& iColor, float iThickness, float iFade, int iEntityId) {
    PM_PROFILE_FUNCTION();

    for (size_t i = 0; i < 4; ++i) {
        sData.circleVertexBufferPtr->worldPosition = iTransform * sData.quadVertexPositions[i];
        sData.circleVertexBufferPtr->localPosition = sData.quadVertexPositions[i] * 2.0f;
        sData.circleVertexBufferPtr->color         = iColor;
        sData.circleVertexBufferPtr->thickness     = iThickness;
        sData.circleVertexBufferPtr->fade          = iFade;
        sData.circleVertexBufferPtr->entityId      = iEntityId;
        sData.circleVertexBufferPtr++;
    }

    sData.circleIndexCount += 6;

    sData.stats.quadCount++;
}

void Renderer2D::DrawLine(const glm::vec3& iPos0, glm::vec3& iPos1, const glm::vec4& iColor, int iEntityId) {
    sData.lineVertexBufferPtr->position = iPos0;
    sData.lineVertexBufferPtr->color    = iColor;
    sData.lineVertexBufferPtr->entityId = iEntityId;
    sData.lineVertexBufferPtr++;

    sData.lineVertexBufferPtr->position = iPos1;
    sData.lineVertexBufferPtr->color    = iColor;
    sData.lineVertexBufferPtr->entityId = iEntityId;
    sData.lineVertexBufferPtr++;

    sData.lineVertexCount += 2;
}

void Renderer2D::DrawRect(const glm::vec3& iPosition, const glm::vec2& iSize, const glm::vec4& iColor, int iEntityId) {
    auto&& p0 = glm::vec3(iPosition.x - iSize.x * 0.5f, iPosition.y - iSize.y * 0.5f, iPosition.z);
    auto&& p1 = glm::vec3(iPosition.x + iSize.x * 0.5f, iPosition.y - iSize.y * 0.5f, iPosition.z);
    auto&& p2 = glm::vec3(iPosition.x + iSize.x * 0.5f, iPosition.y + iSize.y * 0.5f, iPosition.z);
    auto&& p3 = glm::vec3(iPosition.x - iSize.x * 0.5f, iPosition.y + iSize.y * 0.5f, iPosition.z);

    DrawLine(p0, p1, iColor);
    DrawLine(p1, p2, iColor);
    DrawLine(p2, p3, iColor);
    DrawLine(p3, p0, iColor);
}

void Renderer2D::DrawRect(const glm::mat4& iTransform, const glm::vec4& iColor, int iEntityId) {
    glm::vec3 lineVertices[4]{};
    for (size_t i = 0; i < 4; i++) {
        lineVertices[i] = iTransform * sData.quadVertexPositions[i];
    }

    DrawLine(lineVertices[0], lineVertices[1], iColor);
    DrawLine(lineVertices[1], lineVertices[2], iColor);
    DrawLine(lineVertices[2], lineVertices[3], iColor);
    DrawLine(lineVertices[3], lineVertices[0], iColor);
}

/////////////////////
// Getter / Setter //
/////////////////////
float Renderer2D::GetLineWidth() { return sData.LINE_WIDTH; }

void Renderer2D::SetLineWidth(float iWidth) { sData.LINE_WIDTH = iWidth; }

////////////////
// Components //
////////////////
void Renderer2D::DrawSprite(const TransformComponent& iTransformComponent, const SpriteRendererComponent& iSpriteComponent, int iEntityId) {
    DrawQuad(iTransformComponent.transform(), iSpriteComponent.tilingFactor, iSpriteComponent.texture, iSpriteComponent.color, iEntityId);
}

void Renderer2D::DrawCircle(const TransformComponent& iTransformComponent, const CircleRendererComponent& iCircleComponent, int iEntityId) {
    DrawCircle(iTransformComponent.transform(), iCircleComponent.color, iCircleComponent.thickness, iCircleComponent.fade, iEntityId);
}
}
