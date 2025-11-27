#include "PCH/CG_PCH.h"
#include "CG_2DRenderer.h"

// TODO: Encapsulate into rendering wrapper so that we can swap renderers
#include <GLAD/include/glad/glad.h>

#include "CG/2D/Canvas/CG_Canvas.h"

// Temp
#include "CG/Mesh/CG_MeshFactory.h"
#include "CG/OpenGL/CG_GLMeshBuffer.h"
#include "CG/OpenGL/CG_GLRenderTarget.h"
#include "CG/OpenGL/CG_GLTextureBuffer.h"
#include "CG/OpenGL/CG_GLViewportHelper.h"
#include "CG/Shader/CG_Shader.h"
#include "CG/Texture/CG_TextureFactory.h"

CG_2DRenderer::CG_2DRenderer()
    : m_canvases()
    , m_2DShader("J:/Harbourfront/Data/Shaders/ShaderCode/2DR_Vertex.vs",
                 "J:/Harbourfront/Data/Shaders/ShaderCode/2DR_Fragment.fs")

    , m_fsqShader("J:/Harbourfront/Data/Shaders/ShaderCode/Passthrough.vs",
                  "J:/Harbourfront/Data/Shaders/ShaderCode/SolidTexture.fs")
    , m_renderTarget(nullptr)
{}

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
    m_tempTexture = CG_TextureFactory::LoadPNG("J:/Harbourfront/Data/Textures/Splash/YakuEn_Logo_Dark.png");

    // Make FSQ
    {
        float vertices[] = {// Positions		// UVs
                            1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
                            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f};

        unsigned int indices[] = {0, 1, 3, 1, 2, 3};

        m_fsq = CG_MeshFactory::FromData(vertices, 20, indices, 6);
    }
}

void CG_2DRenderer::Render() const
{
    {
        CG_GLRenderTarget::Binding renderTargetBinding = m_renderTarget->Bind();

        constexpr float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        constexpr float depthClearColor = 1.0f;
        glClearBufferfv(GL_COLOR, 0, clearColor);
        glClearBufferfv(GL_DEPTH, 0, &depthClearColor);

        m_2DShader.Use();
        YK_Matrix44 identity(1.0f);
        YK_Matrix::Translate(identity, YK_Vector3f(0.0f, 0.0f, 0.5f));

        m_2DShader.SetMatrix44("transform", identity.GetData());

        m_tempTexture->GetGLData().Bind(0);
        m_tempQuad->GetGLData().Bind();

        // TODO: Refactor into batched rendering
        // for (CG_Canvas const& canvas : m_canvases)
        //{
        //    for (CG_CanvasTextureItem const& textureItem : canvas.GetItems()) {
        //        //glDrawElements(GL_TRIANGLES)
        //    }
        //}

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    // Draw Fullscreen Quad to overlay result
    m_fsqShader.Use();
    m_fsq->GetGLData().Bind();
    glBindTexture(GL_TEXTURE_2D, m_renderTarget->GetColorBufferID());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}