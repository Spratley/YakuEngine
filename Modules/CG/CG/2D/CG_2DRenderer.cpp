#include "PCH/CG_PCH.h"
#include "CG_2DRenderer.h"

// TODO: Encapsulate into rendering wrapper so that we can swap renderers
#include <YKC/Libraries/OpenGL/GLAD/include/glad/glad.h>

#include "CG/2D/Canvas/CG_Canvas.h"

// Temp
#include "CG/Mesh/CG_MeshFactory.h"
#include "CG/OpenGL/CG_GLMeshBuffer.h"
#include "CG/OpenGL/CG_GLRenderTarget.h"
#include "CG/OpenGL/CG_GLTextureBuffer.h"
#include "CG/OpenGL/CG_GLViewportHelper.h"
#include "CG/Shader/CG_Shader.h"
#include "CG/Texture/CG_TextureFactory.h"

#include "CG/Matrix/CG_MatrixExtras.h"

// Temp for Window resizing
#include "YKC/Platforms/YKC_PlatformCore.h"
#include "YKC/Platforms/Windows/YKC_WindowsWindow.h"

constexpr YK_Matrix44 g_ortho = YK_Matrix::Orthographic(1.0f, 1080.f / 1920.f, 10.0f);

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

CG_2DRenderer::~CG_2DRenderer()
{
    delete m_renderTarget;
}

void CG_2DRenderer::Temp_Init()
{
    // Temp, move this to window initialization
    GLint currentViewport[4];
    glGetIntegerv(GL_VIEWPORT, currentViewport);
    CG_GLViewportHelper::SetViewportSize(YK_Vector2i(currentViewport[2], currentViewport[3]));

    m_renderTarget = new CG_GLRenderTarget(YK_Vector2i(800, 500));

    // Temp
    m_tempQuad = CG_MeshFactory::Quad();

    m_canvases[0].AddItem("J:/Harbourfront/Data/Textures/Splash/YakuEn_Logo_Dark.png");

    // More temp, figure this out - Initialize window stuff
    // Add layer of indirection here so I'm not directly touching Windows files
    // TODO: Make this more parallel safe, ensure CG_GLViewportHelper is aware that we're setting the main window size
    // Graphics shouldn't be parallel on the CPU, but we can't guarantee that the window is resized in sync with the update cycle
    PlatformCore& platformCore = PlatformCore::GetInstance();
    if (YKC_WindowsWindow* window = platformCore.GetMainWindow())
    {
        window->SetWindowResizedCallback(CG_GLViewportHelper::SetViewportSize);
    }
}

void CG_2DRenderer::Render() const
{
    RenderToFramebuffer();

    // Draw Fullscreen Quad to overlay result
    m_fsqShader.Use();

    // TODO: Don't manually call this- I'm just clearing the data so that we don't have bunk data waiting on the GPU
    glBindVertexArray(g_nullVAO);

    glBindTexture(GL_TEXTURE_2D, m_renderTarget->GetColorBufferID());

    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void CG_2DRenderer::RenderToFramebuffer() const
{
    CG_GLRenderTarget::Binding renderTargetBinding = m_renderTarget->Bind();

    constexpr float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    constexpr float depthClearColor = 1.0f;
    glClearBufferfv(GL_COLOR, 0, clearColor);
    glClearBufferfv(GL_DEPTH, 0, &depthClearColor);

    m_2DShader.Use();
    m_tempQuad->GetGLData().Bind(); // Can we skip binding a mesh? 2DR will only ever deal with quads

    // TODO: Refactor this so it's more data oriented and not object oriented
    // TODO: And then refactor it so we just batch render all the quads
    for (CG_Canvas const& canvas : m_canvases)
    {
        for (CG_CanvasTextureItem const& textureItem : canvas.GetItems())
        {
            YK_Matrix44 transform(1.0f);
            YK_Matrix::Translate(transform, YK_Vector3f(0.0f, 0.0f, 0.0f));
            YK_Matrix44 scale(1.0f);
            scale[1][1] = 1.0f / textureItem.GetAspectRatio();
            transform = scale * transform * g_ortho;
            m_2DShader.SetMatrix44("transform", transform.GetData());

            textureItem.GetTexture()->GetGLData().Bind(0);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }
}