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

#include "YK/IO/Display/YK_DisplaySurface.h"

#include "CG/Material/CG_Material.h"
#include "CG/Renderer/CG_RenderBinding.h"
#include "CG/Renderer/CG_RenderQueue.h"

// TEMP
#include "CG/Matrix/CG_MatrixExtras.h"
#include "CG/OpenGL/CG_GLMeshBuffer.h"
#include "CG/OpenGL/CG_GLTextureBuffer.h"
#include "CG/OpenGL/CG_GLViewportHelper.h"
#include "CG/Renderable/CG_Renderable.h"
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG/Resource/Shader/CG_Shader.h"
#include "CG/Resource/Texture/CG_TextureFactory.h"

#include "YK/Math/YK_MatrixMath.h"
#include "YK/Platforms/YK_PlatformDefines.h"
#include "YK/Types/Math/YK_Matrix.h"

// EVEN MORE TEMP
#include "CG/Resource/Shader/CG_ShaderResource.h"
#include "YK/ECS/Components/YK_TransformComponent.h"

void CG_RenderModule::TempInit(YK_DisplaySurface& p_displaySurface)
{
    CG_TextureFactory::Init();

    // Super temp - Initialize window stuff
    GLint currentViewport[4];
    glGetIntegerv(GL_VIEWPORT, currentViewport);
    auto const viewportSize = YK_Vector2i(currentViewport[2], currentViewport[3]);
    CG_GLViewportHelper::SetViewportSize(viewportSize);

    // Add layer of indirection here so I'm not directly touching Windows files
    // TODO: Make this more parallel safe, ensure CG_GLViewportHelper is aware that we're setting the main window size
    // Graphics shouldn't be parallel on the CPU, but we can't guarantee that the window is resized in sync with the
    // update cycle

    // This shouldn't be callbacks here
    // The resized callback should just update the internal tracked size of the main render target
    // Perspective matrix should be recalculated automatically by THAT, and SetViewportSize should be called when
    // rendering to any target
    p_displaySurface.GetResizedCallback().Attach(CG_GLViewportHelper::SetViewportSize);

    m_3dRenderer.Temp_Init(p_displaySurface);
    m_2dRenderer.Temp_Init();

    // Should this have a better home?
    glEnable(GL_CULL_FACE);
    glClearColor(0.1133f, 0.1269f, 0.1122f, 1.0f);
}

void CG_RenderModule::Render(YK_Matrix44 const& p_viewMatrix, Zen::Garden const& p_entityGarden) const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_3dRenderer.Render(m_renderBindings, p_viewMatrix, p_entityGarden);
    //m_2dRenderer.Render();

    m_display->SwapBuffers();
}