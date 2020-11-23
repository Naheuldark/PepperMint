#pragma once

#include "PepperMint/Renderer/Camera.h"
#include "PepperMint/Renderer/OrthographicCamera.h"
#include "PepperMint/Renderer/Texture.h"

namespace PepperMint {

class Renderer2D {
  public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Camera& iCamera, const glm::mat4& iTransform);
    static void BeginScene(const OrthographicCamera& iCamera); // TODO TO REMOVE
    static void EndScene();
    static void Flush();

    // Primitives
    static void DrawQuad(const glm::vec2& iPosition,
                         const float      iRotation     = 0.0f,
                         const glm::vec2& iScale        = glm::vec2(1.0f),
                         const float      iTilingFactor = 1.0f,
                         Ref<Texture2D>   iTexture      = nullptr,
                         const glm::vec4& iColor        = glm::vec4(1.0f));

    static void DrawQuad(const glm::vec3& iPosition,
                         const float      iRotation     = 0.0f,
                         const glm::vec2& iScale        = glm::vec2(1.0f),
                         const float      iTilingFactor = 1.0f,
                         Ref<Texture2D>   iTexture      = nullptr,
                         const glm::vec4& iColor        = glm::vec4(1.0f));

    static void DrawQuad(const glm::mat4& iTransform,
                         const float      iTilingFactor = 1.0f,
                         Ref<Texture2D>   iTexture      = nullptr,
                         const glm::vec4& iColor        = glm::vec4(1.0f));

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
