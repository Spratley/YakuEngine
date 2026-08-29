#include "PCH/CG_PCH.h"
#include "CG_RenderModule.h"

#if YK_PLATFORM == YK_WASM
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YK/Libraries/OpenGL/GLAD/include/glad/glad.h>
#include <YK/Libraries/OpenGL/GLFW/include/glfw3.h>
#endif

#include "CG/RenderTarget/CG_RenderTarget.h"
#include "CG/Renderer/2D/CG_2DRenderer.h"
#include "CG/Renderer/3D/CG_3DRenderer.h"

#include "YK/IO/Display/YK_DisplaySurface.h"

CG_RenderModule::CG_RenderModule(YK_DisplaySurface& p_displaySurface)
    : m_display(&p_displaySurface)
    , m_displayRenderTarget(p_displaySurface.GetDimensions())
    , m_3DRenderer(p_displaySurface)
    , m_2DRenderer()
{
    p_displaySurface.GetResizedCallback().Attach<CG_RenderTarget, &CG_RenderTarget::SetSize>(&m_displayRenderTarget);

    // TODO: This should have a better home
    glEnable(GL_CULL_FACE);
    glClearColor(0.1133f, 0.1269f, 0.1122f, 1.0f);
}

void CG_RenderModule::Render(CG_CameraComponent const& p_camera) const
{
    m_displayRenderTarget.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_3DRenderer.Render(m_renderBindingsCache, p_camera);
    m_2DRenderer.Render();

    m_display->SwapBuffers();
}