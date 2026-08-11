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
#include "CG/OpenGL/CG_GLMeshBuffer.h"
#include "CG/OpenGL/CG_GLTextureBuffer.h"
#include "CG/RenderTarget/OpenGL/CG_GLRenderTarget.h"
#include "CG/Resource/Mesh/CG_MeshFactory.h"
#include "CG/Resource/Shader/CG_Shader.h"
#include "CG/Resource/Texture/CG_TextureFactory.h"

#include "CG/Matrix/CG_MatrixExtras.h"
#include "YK/Math/YK_MatrixMath.h"

#include "YK/IO/Display/YK_DisplaySurface.h"

constexpr YK_Matrix44 g_ortho = YK_Matrix::Orthographic2D(1.0f, 1080.f / 1920.f);

// TODO: Don't make this locally in 2DRenderer.cpp
YK_U32 g_nullVAO = 0;

CG_2DRenderer::CG_2DRenderer()
    : m_canvases()
    , m_2DShader("J:/Harbourfront/Data/Shaders/ShaderCode/2DR_Vertex.vs",
                 "J:/Harbourfront/Data/Shaders/ShaderCode/2DR_Fragment.fs")
    , m_fsqShader("J:/Harbourfront/Data/Shaders/ShaderCode/FSQ.vs",
                  "J:/Harbourfront/Data/Shaders/ShaderCode/SolidTexture.fs")
    , m_renderTarget(nullptr)
{
    // TODO: Don't do this (sob)
    if (g_nullVAO == 0)
    {
        glGenVertexArrays(1, &g_nullVAO);
    }
}

CG_2DRenderer::~CG_2DRenderer() { delete m_renderTarget; }

void CG_2DRenderer::Temp_Init()
{
    // Temp, move this to window initialization

    m_renderTarget = new CG_GLRenderTarget(YK_Vector2i(1920, 1080));

    // Temp
    m_tempQuad = CG_MeshFactory::Quad();

    m_canvases[0].AddItem("J:/Harbourfront/Data/Textures/Splash/YakuEn_Logo_Dark.png");
}

void CG_2DRenderer::Render() const
{
    glDisable(GL_DEPTH_TEST);
    RenderToFramebuffer();

    // Draw Fullscreen Quad to overlay result
    m_fsqShader.Use();

    // TODO: Don't manually call this- I'm just clearing the data so that we don't have bunk data waiting on the GPU
    glBindVertexArray(g_nullVAO);

    glBindTexture(GL_TEXTURE_2D, m_renderTarget->GetColorBufferID());

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glEnable(GL_DEPTH_TEST);
}

void CG_2DRenderer::RenderToFramebuffer() const
{
    CG_GLRenderTarget::Binding renderTargetBinding = m_renderTarget->Bind();

    constexpr float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, clearColor);

    m_2DShader.Use();
    glBindVertexArray(g_nullVAO);

    // TODO: And then refactor it so we just batch render all the quads
    // TODO: Also like sort back to front? Opaque and transparent passes?
    for (CG_Canvas const& canvas : m_canvases)
    {
        for (CG_CanvasTextureItem const& textureItem : canvas.GetItems())
        {
            YK_Matrix44 transform = YK_Matrix::Construct(YK_Vector3f(0.0f, 0.0f, 0.0f),
                                                         YK_Vector3f(1.0f, 1.0f / textureItem.GetAspectRatio(), 1.0f));
            transform = g_ortho * transform;
            m_2DShader.SetMatrix44("transform", transform.GetData());

            textureItem.GetTexture()->GetGLData().Bind(0);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }
}