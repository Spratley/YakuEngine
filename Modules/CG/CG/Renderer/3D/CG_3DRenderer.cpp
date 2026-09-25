#include "PCH/CG_PCH.h"
#include "CG_3DRenderer.h"

#include "YK/Core/YK_Core.h"
#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/IO/Display/GLFW/YK_DisplaySurface_GLFW.hpp"
#include "YK/IO/Display/YK_DisplaySurface.h"
#include "YK/Libraries/Zen/Zen_Garden.h"
#include "YK/Math/YK_MatrixMath.h"
#include "YK/Platforms/YK_PlatformDefines.h"
#include "YK/Time/YK_Time.h"
#include "YK/Types/Containers/YK_StaticArray.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Matrix.h"
#include "YK/Types/Math/YK_Quaternion.h"
#include "YK/Types/Math/YK_Vector.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include "CG/Camera/CG_CameraComponent.h"
#include "CG/ECS/CG_Components.h"
#include "CG/RenderTarget/CG_RenderTarget.h"
#include "CG/Renderer/CG_RenderBinding.h"
#include "CG/Renderer/CG_RenderQueue.h"
#include "CG/Resource/Material/CG_Material.h"
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG/Resource/Shader/CG_Shader.h"

#include <vector>

#if YK_PLATFORM == YK_WASM
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YK/Libraries/OpenGL/GLAD/include/glad/glad.h>
#endif

#include <cstring>

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

    CG_RenderQueueBuilder renderQueueBuilder;

    for (auto [transform, renderer, mesh] :
         entityGarden.ViewComponents<YK_TransformComponent, CG_RendererComponent, CG_MeshComponent>())
    {
        renderQueueBuilder.PushEntry(transform, renderer);
        renderQueueBuilder.SetMesh(mesh);
    }

    for (auto [transform, renderer, skeletalMesh, pose] :
         entityGarden
           .ViewComponents<YK_TransformComponent, CG_RendererComponent, CG_SkeletalMeshComponent, CG_PoseComponent>())
    {
        renderQueueBuilder.PushEntry(transform, renderer);
        renderQueueBuilder.SetMesh(skeletalMesh, pose);
    }

    CG_RenderQueue renderQueue = renderQueueBuilder.Build();

    YK_Matrix44 const cameraMatrix = p_camera.CalculateCameraMatrix(CG_3DRenderer_Private::viewportAspectRatio);
    p_target.Bind();

    YK_SizeT itemIndex = 0;
    YK_SizeT meshEntryIndex = 0;
    for (auto const& [material, materialEndIndex] : renderQueue.m_materialEntries)
    {
        p_bindings.Bind(*material);
        CG_Shader const& shader = *material->m_shader;

        YK_SizeT endIndex;
        do
        {
            auto const& [mesh, meshEndIndex] = renderQueue.m_meshEntries[meshEntryIndex++];
            p_bindings.Bind(*mesh);
            endIndex = meshEndIndex;

            for (; itemIndex <= endIndex; ++itemIndex)
            {
                YK_Matrix44 perspectiveTransform = cameraMatrix * renderQueue.m_transforms[itemIndex];
                shader.SetMatrix44("u_mvp", perspectiveTransform.GetData());

                YK_U32 const boneCount = mesh->GetBoneCount();
                if (boneCount > 0)
                {
                    YK_ASSERT(shader.IsSkeletal(), "Non-Skeletal shader being used with a skeleton!");
                    // Static array here is just to make sure we always have valid data to send to the GPU
                    // WebGL demands we always send the exact number of bone matrices that the shader requested
                    static YK_StaticArray<YK_Matrix44, 64> Bones;
                    memcpy(static_cast<void*>(&Bones),
                           static_cast<void const*>(&renderQueue.m_transforms[itemIndex + 1]),
                           sizeof(YK_Matrix44) * boneCount);
                    shader.SetSkeletonData(Bones);
                }

                glDrawElements(GL_TRIANGLES, p_bindings.GetBoundMesh()->GetIndexBufferSize(), GL_UNSIGNED_INT, 0);
            }

        } while (endIndex < materialEndIndex);
    }

    p_bindings.TempInvalidate();
}