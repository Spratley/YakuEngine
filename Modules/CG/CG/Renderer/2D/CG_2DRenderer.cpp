#include "PCH/CG_PCH.h"
#include "CG_2DRenderer.h"

// TODO: Encapsulate into rendering wrapper so that we can swap renderers
// Time has come, this has to be done. For now, hack
#if YK_PLATFORM == YK_WASM
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YK/Libraries/OpenGL/GLAD/include/glad/glad.h>
#endif

#include "CG/2D/Canvas/CG_Canvas.h"

// Temp
#include "CG/2D/Canvas/CG_CanvasTextureItem.h"
#include "CG/Matrix/CG_MatrixExtras.h"
#include "CG/OpenGL/CG_GLTextureBuffer.h"
#include "CG/RenderTarget/CG_RenderTarget.h"
#include "CG/Resource/Shader/CG_Shader.h"

#include "YK/Core/YK_Core.h"
#include "YK/IO/Asset/YK_AssetManager.h"
#include "YK/IO/File/YK_FilePath.h"
#include "YK/Math/YK_MatrixMath.h"

constexpr YK_Matrix44 g_ortho = YK_Matrix::OrthographicUI(1.0f, 1080.f / 1920.f);

// TODO: Don't make this locally in 2DRenderer.cpp
YK_U32 g_nullVAO = 0;

CG_2DRenderer::CG_2DRenderer()
{
    YK_Core& engine = YK_Core::GetEngine();
    YK_AssetManager& assetManager = engine.GetAssetManager();

    m_2DShader = &assetManager.GetAsset<CG_Shader>(YK_FilePath("Shaders/2DR.YKS"));
    m_fsqShader = &assetManager.GetAsset<CG_Shader>(YK_FilePath("Shaders/FSQ.YKS"));

    // TODO: Don't do this (sob)
    if (g_nullVAO == 0)
    {
        glGenVertexArrays(1, &g_nullVAO);
    }

    // Temp, move this to window initialization
    m_renderTarget = new CG_RenderTarget(YK_Vector2i(1920 / 2, 1080 / 2));
    m_renderTarget->Initialize();

    m_canvases[0].AddItem(YK_FilePath("Textures/Splash/YakuEn_Logo_Dark.png"));
}

CG_2DRenderer::~CG_2DRenderer() { delete m_renderTarget; }

void CG_2DRenderer::Render() const
{
    glDisable(GL_DEPTH_TEST);
    RenderToFramebuffer();

    // Draw Fullscreen Quad to overlay result
    m_fsqShader->Use();

    // TODO: Don't manually call this- I'm just clearing the data so that we don't have bunk data waiting on the GPU
    glBindVertexArray(g_nullVAO);

    m_renderTarget->BindAsInputTexture(0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glEnable(GL_DEPTH_TEST);
}

void CG_2DRenderer::RenderToFramebuffer() const
{
    m_renderTarget->Bind();

    constexpr float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, clearColor);

    m_2DShader->Use();
    glBindVertexArray(g_nullVAO);

    // TODO: Also like sort back to front? Opaque and transparent passes?
    for (CG_Canvas const& canvas : m_canvases)
    {
        YK_Matrix44 const canvasMatrix = canvas.CalculateMatrix();
        for (CG_CanvasTextureItem const& textureItem : canvas.GetItems())
        {
            YK_Matrix44 transform = YK_Matrix::Construct(YK_Vector3f(0.0f, 0.0f, 0.0f),
                                                         YK_Vector3f(1.0f, 1.0f / textureItem.GetAspectRatio(), 1.0f));
            transform = g_ortho * canvasMatrix * transform;
            m_2DShader->SetMatrix44("transform", transform.GetData());
            textureItem.GetTexture().GetGLData().Bind(0);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}