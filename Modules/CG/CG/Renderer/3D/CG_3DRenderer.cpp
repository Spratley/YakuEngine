#include "PCH/CG_PCH.h"
#include "CG_3DRenderer.h"

#include "CG/Camera/CG_Camera.h"
#include "CG/Resource/Shader/CG_ShaderResource.h"

// Temp
#include "CG/Material/CG_Material.h"
#include "CG/Matrix/CG_MatrixExtras.h"
#include "CG/Renderable/CG_Renderable.h"
#include "CG/Renderer/CG_RenderBinding.h"
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/IO/Display/YK_DisplaySurface.h"
#include "YK/Libraries/Zen/Zen_Garden.h"

#if YK_PLATFORM == YK_WASM
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YK/Libraries/OpenGL/GLAD/include/glad/glad.h>
#include <YK/Libraries/OpenGL/GLFW/include/glfw3.h>
#endif

// TODO: Move to a better place
namespace CG_3DRenderer_Private
{
    YK_Vector2i viewportDimensions;
    float viewportAspectRatio;

    void RecalculateViewport(YK_Vector2i p_newDimensions)
    {
        viewportDimensions = p_newDimensions;
        viewportAspectRatio = static_cast<float>(p_newDimensions.x) / static_cast<float>(p_newDimensions.y);
    }
} // namespace CG_3DRenderer_Private

void CG_3DRenderer::Temp_Init(YK_DisplaySurface& p_displaySurface)
{
    GLint currentViewport[4];
    glGetIntegerv(GL_VIEWPORT, currentViewport);
    CG_3DRenderer_Private::RecalculateViewport(YK_Vector2i(currentViewport[2], currentViewport[3]));
    p_displaySurface.GetResizedCallback().Attach(CG_3DRenderer_Private::RecalculateViewport);
}

void CG_3DRenderer::Render(CG_RenderBinding& p_bindings,
                           CG_CameraComponent const& p_camera,
                           Zen::Garden const& p_garden) const
{
    Zen::EntityView renderableEntities =
      p_garden.ViewComponents<YK_TransformComponent, CG_MeshComponent, CG_RendererComponent>();
    for (auto [transform, meshComponent, rendererComponent] : renderableEntities)
    {
        m_renderQueue.Push(*rendererComponent.m_material, *meshComponent.m_mesh, transform);
    }
    m_renderQueue.Bake();

    YK_Matrix44 const cameraMatrix =
      CG_CameraUtils::CalculateCameraMatrix(p_camera, 60.0f, CG_3DRenderer_Private::viewportAspectRatio, 0.1f, 100.0f);

    for (CG_RenderQueue::Entry const& item : m_renderQueue)
    {
        p_bindings.Bind(*item.m_material);
        p_bindings.Bind(*item.m_mesh);
        YK_Matrix44 perspectiveTransform = cameraMatrix * item.m_transform;
        item.m_material->m_shader->SetMatrix44("transform", perspectiveTransform.GetData());
        glDrawElements(GL_TRIANGLES, p_bindings.GetBoundMesh()->GetIndexBufferSize(), GL_UNSIGNED_INT, 0);
    }

    m_renderQueue.Clear();
    p_bindings.TempInvalidate();
}