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
#include "YK/Libraries/Zen/Zen_Garden.h"

YK_Matrix44 g_perspective;

void RecalculatePerspectiveMatrix(YK_Vector2i p_viewportDimensions)
{
    g_perspective = YK_Matrix::Perspective<float>(60.0f,
                                                  static_cast<float>(p_viewportDimensions.x)
                                                    / static_cast<float>(p_viewportDimensions.y),
                                                  0.1f,
                                                  100.0f);
}

void CG_RenderModule::TempInit(YK_DisplaySurface& p_displaySurface)
{
    CG_TextureFactory::Init();

    shader = new CG_Shader(CG_ShaderLoader::Load("J:/Harbourfront/Data/Shaders/ShaderCode/Vertex.vs",
                                                 "J:/Harbourfront/Data/Shaders/ShaderCode/Fragment.fs"));

    // Super temp - Initialize window stuff
    GLint currentViewport[4];
    glGetIntegerv(GL_VIEWPORT, currentViewport);
    auto const viewportSize = YK_Vector2i(currentViewport[2], currentViewport[3]);
    CG_GLViewportHelper::SetViewportSize(viewportSize);
    RecalculatePerspectiveMatrix(viewportSize);
    // Add layer of indirection here so I'm not directly touching Windows files
    // TODO: Make this more parallel safe, ensure CG_GLViewportHelper is aware that we're setting the main window size
    // Graphics shouldn't be parallel on the CPU, but we can't guarantee that the window is resized in sync with the
    // update cycle

    // This shouldn't be callbacks here
    // The resized callback should just update the internal tracked size of the main render target
    // Perspective matrix should be recalculated automatically by THAT, and SetViewportSize should be called when
    // rendering to any target
    p_displaySurface.GetResizedCallback().Attach(CG_GLViewportHelper::SetViewportSize);
    p_displaySurface.GetResizedCallback().Attach(RecalculatePerspectiveMatrix);


    m_2dRenderer.Temp_Init();

    // Should this have a better home?
    glEnable(GL_CULL_FACE);
    glClearColor(0.1133f, 0.1269f, 0.1122f, 1.0f);
}

void CG_RenderModule::Render(YK_Matrix44 const& p_viewMatrix, Zen::Garden const& p_entityGarden) const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->Use();

    // TODO: Sort by mesh?
    Zen::EntityView view =
      p_entityGarden.ViewComponents<YK_TransformComponent, CG_MeshComponent, CG_RendererComponent>();
    for (auto [transform, meshComponent, rendererComponent] : view)
    {
        meshComponent.m_mesh->GetGLData().Bind();
        rendererComponent.m_texture->GetGLData().Bind(0);

        YK_Matrix44 perspectiveTransform =
          g_perspective * p_viewMatrix
          * YK_Matrix::Construct(transform.m_position, transform.m_orientation, transform.m_scale);
        shader->SetMatrix44("transform", perspectiveTransform.GetData());
        glDrawElements(GL_TRIANGLES, meshComponent.m_mesh->GetIndexBufferSize(), GL_UNSIGNED_INT, 0);
    }

    //m_2dRenderer.Render();

    m_display->SwapBuffers();
}