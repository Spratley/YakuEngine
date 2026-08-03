#include "PCH/CG_PCH.h"
#include "CG_RenderModule.h"

#if YK_WEB_ASSEMBLY
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YKC/Libraries/OpenGL/GLAD/include/glad/glad.h>
#include <YKC/Libraries/OpenGL/GLFW/include/glfw3.h>
#endif

// TEMP
#include "CG/OpenGL/CG_GLMeshBuffer.h"
#include "CG/OpenGL/CG_GLTextureBuffer.h"
#include "CG/Renderable/CG_Renderable.h"
#include "CG/Resource/Mesh/CG_MeshFactory.h"
#include "CG/Resource/Shader/CG_Shader.h"
#include "CG/Resource/Texture/CG_TextureFactory.h"

#include "CG/Matrix/CG_MatrixExtras.h"

// EVEN MORE TEMP
#include "YKC/ECS/YKC_TEMP_TransformComponent.h"
#include "YKC/Libraries/Zen/Zen_Garden.h"

YK_Matrix44 g_perspective;

void CG_RenderModule::TempInit()
{
    CG_TextureFactory::Init();

    ShaderResources& shaderResources = m_cgResources.GetResourceContainer<CG_Shader>();
#if YK_WEB_ASSEMBLY
    shader = shaderResources.Load("J:/Harbourfront/Data/Shaders/ShaderCode/WASM/Vertex_WASM.vs",
                                  "J:/Harbourfront/Data/Shaders/ShaderCode/WASM/Fragment_WASM.fs");
#else
    shader = shaderResources.Load("J:/Harbourfront/Data/Shaders/ShaderCode/Vertex.vs",
                                  "J:/Harbourfront/Data/Shaders/ShaderCode/Fragment.fs");
#endif // WEB_ASSEMBLY

    g_perspective = YK_Matrix::Perspective<float>(60.0f, 1920.0f / 1080.0f, 0.1f, 100.0f);

    m_2dRenderer.Temp_Init();

    // Should this have a better home?
    glEnable(GL_CULL_FACE);
}

void CG_RenderModule::Render(YK_Matrix44 const& p_viewMatrix, Zen::Garden const& p_entityGarden) const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CG_Shader* temp_shader = shader.Get();

    temp_shader->Use();

    // TODO: Sort by mesh?
    Zen::EntityView view = p_entityGarden.ViewComponents<TransformComponent, CG_MeshComponent, CG_RendererComponent>();
    for (auto [transform, meshComponent, rendererComponent] : view)
    {
        meshComponent.m_mesh->GetGLData().Bind();
        rendererComponent.m_texture->GetGLData().Bind(0);

        YK_Matrix44 perspectiveTransform = g_perspective * p_viewMatrix * YK_Matrix::Construct(transform.m_position, transform.m_orientation, transform.m_scale);
        temp_shader->SetMatrix44("transform", perspectiveTransform.GetData());
        glDrawElements(GL_TRIANGLES, meshComponent.m_mesh->GetIndexBufferSize(), GL_UNSIGNED_INT, 0);
    }

    m_2dRenderer.Render();

    glfwSwapBuffers(m_glfwWindow);
}