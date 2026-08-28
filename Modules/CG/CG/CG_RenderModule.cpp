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

#include "CG/Renderer/2D/CG_2DRenderer.h"
#include "CG/Renderer/3D/CG_3DRenderer.h"

#include "YK/IO/Display/YK_DisplaySurface.h"

// Temp
#include "CG/OpenGL/CG_GLViewportHelper.h"

CG_RenderModule::CG_RenderModule(YK_DisplaySurface& p_displaySurface)
    : m_display(&p_displaySurface)
    , m_3DRenderer(p_displaySurface)
    , m_2DRenderer()
{
    CG_GLViewportHelper::SetViewportSize(p_displaySurface.GetDimensions());

    // Add layer of indirection here so I'm not directly touching Windows files
    // TODO: Make this more parallel safe, ensure CG_GLViewportHelper is aware that we're setting the main window size
    // Graphics shouldn't be parallel on the CPU, but we can't guarantee that the window is resized in sync with the
    // update cycle

    // This shouldn't be callbacks here
    // The resized callback should just update the internal tracked size of the main render target
    // Perspective matrix should be recalculated automatically by THAT, and SetViewportSize should be called when
    // rendering to any target
    p_displaySurface.GetResizedCallback().Attach(CG_GLViewportHelper::SetViewportSize);

    // Should this have a better home?
    glEnable(GL_CULL_FACE);
    glClearColor(0.1133f, 0.1269f, 0.1122f, 1.0f);
}

void CG_RenderModule::Render(CG_CameraComponent const& p_camera) const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_3DRenderer.Render(m_renderBindingsCache, p_camera);
    m_2DRenderer.Render();

    m_display->SwapBuffers();
}