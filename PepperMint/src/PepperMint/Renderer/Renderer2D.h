#pragma once

#include "PepperMint/Renderer/Camera.h"
#include "PepperMint/Renderer/EditorCamera.h"
#include "PepperMint/Renderer/Texture.h"
#include "PepperMint/Scene/Components.h"

namespace PepperMint {

class Renderer2D {
  public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Camera& iCamera, const glm::mat4& iTransform);
    static void BeginScene(const EditorCamera& iCamera);
    static void EndScene();
    static void Flush();

    // Primitives
    static void DrawQuad(const glm::vec2& iPosition,
                         float            iRotation     = 0.0f,
                         const glm::vec2& iScale        = glm::vec2(1.0f),
                         float            iTilingFactor = 1.0f,
                         Ref<Texture2D>   iTexture      = nullptr,
                         const glm::vec4& iColor        = glm::vec4(1.0f),
                         int              iEntityId     = -1);

    static void DrawQuad(const glm::vec3& iPosition,
                         float            iRotation     = 0.0f,
                         const glm::vec2& iScale        = glm::vec2(1.0f),
                         float            iTilingFactor = 1.0f,
                         Ref<Texture2D>   iTexture      = nullptr,
                         const glm::vec4& iColor        = glm::vec4(1.0f),
                         int              iEntityId     = -1);

    static void DrawQuad(const glm::mat4& iTransform,
                         float            iTilingFactor = 1.0f,
                         Ref<Texture2D>   iTexture      = nullptr,
                         const glm::vec4& iColor        = glm::vec4(1.0f),
                         int              iEntityId     = -1);

    static void DrawCircle(const glm::mat4& iTransform, const glm::vec4& iColor, float iThickness, float iFade, int iEntityId = -1);

    static void DrawLine(const glm::vec3& iPos0, glm::vec3& iPos1, const glm::vec4& iColor, int iEntityId = -1);

    static void DrawRect(const glm::vec3& iPosition, const glm::vec2& iSize, const glm::vec4& iColor, int iEntityId = -1);

    static void DrawRect(const glm::mat4& iTransform, const glm::vec4& iColor, int iEntityId = -1);

    // Getters / Setters
    static float GetLineWidth();
    static void  SetLineWidth(float iWidth);

    // Components
    static void DrawSprite(const TransformComponent& iTransformComponent, const SpriteRendererComponent& iSpriteComponent, int iEntityId = -1);
    static void DrawCircle(const TransformComponent& iTransformComponent, const CircleRendererComponent& iCircleComponent, int iEntityId = -1);

    // Statistics
    struct Statistics {
        uint32_t drawCalls;
        uint32_t quadCount;

        uint32_t totalVertexCount() const { return quadCount * 4; }
        uint32_t totalIndexCount() const { return quadCount * 6; }
    };

    static void       ResetStats();
    static Statistics Stats();

  private:
    static void StartBatch();
    static void NextBatch();
};

}
