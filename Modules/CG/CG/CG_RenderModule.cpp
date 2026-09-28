#include "PCH/CG_PCH.h"
#include "CG_RenderModule.h"

#include "YK/IO/Display/GLFW/YK_DisplaySurface_GLFW.hpp"
#include "YK/IO/Display/YK_DisplaySurface.h"
#include "YK/Libraries/Zen/Entity/Zen_Entity.h"
#include "YK/Platforms/YK_PlatformDefines.h"

#include "CG/Camera/CG_CameraComponent.h"
#include "CG/RenderTarget/CG_RenderTarget.h"
#include "CG/Renderer/2D/CG_2DRenderer.h"
#include "CG/Renderer/3D/CG_3DRenderer.h"

#if YK_PLATFORM == YK_WASM
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YK/Libraries/OpenGL/GLAD/include/glad/glad.h>
#include <YK/Libraries/OpenGL/GLFW/include/glfw3.h>
#endif

CG_RenderModule::CG_RenderModule(YK_DisplaySurface& p_displaySurface)
    : m_display(&p_displaySurface)
    , m_displayRenderTarget()
    , m_3DRenderer(p_displaySurface)
    , m_2DRenderer()
    , m_activeCamera(Zen::Entity{})
{
    m_displayRenderTarget.SetSize(p_displaySurface.GetDimensions());
    p_displaySurface.GetResizedCallback().Attach<CG_RenderTarget, &CG_RenderTarget::SetSize>(&m_displayRenderTarget);

    // TODO: This should have a better home
    glEnable(GL_CULL_FACE);
    glClearColor(0.1133f, 0.1269f, 0.1122f, 1.0f);
}

void CG_RenderModule::Render() const
{
    if (!m_activeCamera)
    {
        return;
    }

    CG_CameraComponent const* camera = m_activeCamera.GetComponent<CG_CameraComponent>();

    m_displayRenderTarget.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_3DRenderer.Render(m_displayRenderTarget, m_renderBindingsCache, *camera);
    m_2DRenderer.Render(m_displayRenderTarget);

    m_display->SwapBuffers();
}