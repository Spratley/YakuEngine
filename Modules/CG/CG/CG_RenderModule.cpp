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

    temp_quad = CG_MeshFactory::LoadOBJ("J:/Harbourfront/Data/Models/HeartTest.obj");

    ShaderResources& shaderResources = m_cgResources.GetResourceContainer<CG_Shader>();
#if YK_WEB_ASSEMBLY
    shader = shaderResources.Load("J:/Harbourfront/Data/Shaders/ShaderCode/WASM/Vertex_WASM.vs",
                                  "J:/Harbourfront/Data/Shaders/ShaderCode/WASM/Fragment_WASM.fs");
#else
    shader = shaderResources.Load("J:/Harbourfront/Data/Shaders/ShaderCode/Vertex.vs",
                                  "J:/Harbourfront/Data/Shaders/ShaderCode/Fragment.fs");
#endif // WEB_ASSEMBLY

    temp_texture = CG_TextureFactory::LoadPNG("J:/Harbourfront/Data/Textures/HeartTest.png");

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

    temp_texture->GetGLData().Bind(0);
    temp_quad->GetGLData().Bind();
    
    Zen::EntityView view = p_entityGarden.ViewComponents<TransformComponent, RenderableComponent>();
    for (auto [transform, _] : view)
    {
        YK_Unused(_);
        YK_Matrix44 perspectiveTransform = transform.m_transform * p_viewMatrix * g_perspective;
        temp_shader->SetMatrix44("transform", perspectiveTransform.GetData());
        glDrawElements(GL_TRIANGLES, 1496 * 3, GL_UNSIGNED_INT, 0);
    }

    m_2dRenderer.Render();

    glfwSwapBuffers(m_glfwWindow);
}