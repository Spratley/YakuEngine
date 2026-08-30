#include "PCH/CG_PCH.h"
#include "CG_3DRenderer.h"

#include "CG/Camera/CG_CameraComponent.h"

#include "CG/ECS/CG_Components.h"
#include "CG/RenderTarget/CG_RenderTarget.h"
#include "CG/Renderer/CG_RenderBinding.h"
#include "CG/Renderer/CG_RenderQueue.h"
#include "CG/Resource/Material/CG_Material.h"
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG/Resource/Shader/CG_Shader.h"

#include "YK/Core/YK_Core.h"
#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/IO/Display/YK_DisplaySurface.h"
#include "YK/Libraries/Zen/Zen_Garden.h"
#include "YK/Types/Math/YK_Matrix.h"
#include "YK/Types/Math/YK_Vector.h"

#if YK_PLATFORM == YK_WASM
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YK/Libraries/OpenGL/GLAD/include/glad/glad.h>
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

CG_3DRenderer::CG_3DRenderer(YK_DisplaySurface& p_displaySurface)
{
    GLint currentViewport[4];
    glGetIntegerv(GL_VIEWPORT, currentViewport);
    CG_3DRenderer_Private::RecalculateViewport(YK_Vector2i(currentViewport[2], currentViewport[3]));
    p_displaySurface.GetResizedCallback().Attach<&CG_3DRenderer_Private::RecalculateViewport>();
}

void CG_3DRenderer::Render(CG_RenderTarget const& p_target,
                           CG_RenderBinding& p_bindings,
                           CG_CameraComponent const& p_camera) const
{
    Zen::Garden& entityGarden = YK_Core::GetEngine().GetZenGarden();

    Zen::EntityView renderableEntities =
      entityGarden.ViewComponents<YK_TransformComponent, CG_MeshComponent, CG_RendererComponent>();

    CG_RenderQueue renderQueue;
    renderQueue.Allocate(renderableEntities.CountU());
    for (auto [transform, meshComponent, rendererComponent] : renderableEntities)
    {
        renderQueue.Push(*rendererComponent.m_material, *meshComponent.m_mesh, transform);
    }
    renderQueue.Bake();

    YK_Matrix44 const cameraMatrix = p_camera.CalculateCameraMatrix(CG_3DRenderer_Private::viewportAspectRatio);

    p_target.Bind();
    for (CG_RenderQueue::Entry const& item : renderQueue)
    {
        p_bindings.Bind(*item.m_material);
        p_bindings.Bind(*item.m_mesh);
        YK_Matrix44 perspectiveTransform = cameraMatrix * item.m_transform;
        item.m_material->m_shader->SetMatrix44("transform", perspectiveTransform.GetData());
        glDrawElements(GL_TRIANGLES, p_bindings.GetBoundMesh()->GetIndexBufferSize(), GL_UNSIGNED_INT, 0);
    }

    p_bindings.TempInvalidate();
}