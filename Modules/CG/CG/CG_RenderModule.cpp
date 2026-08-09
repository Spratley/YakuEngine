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

#if YK_PLATFORM == YK_WASM
    shader = new CG_Shader(CG_ShaderLoader::Load("J:/Harbourfront/Data/Shaders/ShaderCode/WASM/Vertex_WASM.vs",
                                                 "J:/Harbourfront/Data/Shaders/ShaderCode/WASM/Fragment_WASM.fs"));
#else
    shader = new CG_Shader(CG_ShaderLoader::Load("J:/Harbourfront/Data/Shaders/ShaderCode/Vertex.vs",
                                                 "J:/Harbourfront/Data/Shaders/ShaderCode/Fragment.fs"));
#endif // YK_PLATFORM == YK_WASM

    // Super temp
    RecalculatePerspectiveMatrix(YK_Vector2i(1920, 1080));
    p_displaySurface.GetResizedCallback().Attach(RecalculatePerspectiveMatrix);

    m_2dRenderer.Temp_Init(p_displaySurface);

    // Should this have a better home?
    glEnable(GL_CULL_FACE);
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

    m_2dRenderer.Render();

    m_display->SwapBuffers();
}