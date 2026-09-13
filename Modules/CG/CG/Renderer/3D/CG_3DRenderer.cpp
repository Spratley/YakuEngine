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
#include "CG/Resource/Animation/CG_Animation.h"
#include "CG/Resource/Material/CG_Material.h"
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG/Resource/Shader/CG_Shader.h"
#include "CG/Resource/Skeleton/CG_Skeleton.h"

#include <vector>

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

    // TEMP
    YK_Matrix44 GetGlobalJointMatrix(CG_Animation const& p_animation, CG_Skeleton::Bone const& p_bone)
    {
        float t = YK_Time::ElapsedTime();
        t = YK_FloatModulo(t, 3.0f); // Out of sync with test animation to see sampling out of range

        YK_Vector3f position;
        YK_Quaternion orientation;
        YK_Vector3f scale = YK_Vector3f::One();

        for (auto const& positionChannel : p_animation.m_positionChannels)
        {
            if (positionChannel.m_boneIndex == p_bone.m_index)
            {
                position = positionChannel.Sample(t);
                break;
            }
        }

        for (auto const& orientationChannel : p_animation.m_orientationChannels)
        {
            if (orientationChannel.m_boneIndex == p_bone.m_index)
            {
                orientation = orientationChannel.Sample(t);
                break;
            }
        }

        for (auto const& scaleChannel : p_animation.m_scaleChannels)
        {
            if (scaleChannel.m_boneIndex == p_bone.m_index)
            {
                scale = scaleChannel.Sample(t);
                break;
            }
        }

        YK_Matrix44 result = YK_Matrix::Construct(position, orientation, scale);
        if (p_bone.m_parent)
        {
            result = GetGlobalJointMatrix(p_animation, *p_bone.m_parent) * result;
        }
        return result;
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
        renderQueue.Push(*rendererComponent.m_material,
                         *meshComponent.m_mesh,
                         meshComponent.m_skeleton,
                         meshComponent.m_animation,
                         transform);
    }
    renderQueue.Bake();

    YK_Matrix44 const cameraMatrix = p_camera.CalculateCameraMatrix(CG_3DRenderer_Private::viewportAspectRatio);

    p_target.Bind();
    for (CG_RenderQueue::Entry const& item : renderQueue.GetQueue())
    {
        p_bindings.Bind(*item.m_material);
        p_bindings.Bind(*item.m_mesh);
        YK_Matrix44 perspectiveTransform = cameraMatrix * item.m_transform;
        item.m_material->m_shader->SetMatrix44("u_mvp", perspectiveTransform.GetData());
        glDrawElements(GL_TRIANGLES, p_bindings.GetBoundMesh()->GetIndexBufferSize(), GL_UNSIGNED_INT, 0);
    }

    for (CG_RenderQueue::SkeletalEntry const& skeletalItem : renderQueue.GetSkeletalQueue())
    {
        p_bindings.Bind(*skeletalItem.m_material);
        p_bindings.Bind(*skeletalItem.m_mesh);

        CG_Shader const* shader = skeletalItem.m_material->m_shader;
        YK_Matrix44 perspectiveTransform = cameraMatrix * skeletalItem.m_transform;
        shader->SetMatrix44("u_mvp", perspectiveTransform.GetData());

        // Temp
        std::vector<YK_Matrix44> bones;
        bones.resize(64);

        for (auto i : YK_CountTo(skeletalItem.m_skeleton->m_bones.size()))
        {
            YK_Matrix44 jointTransform =
              CG_3DRenderer_Private::GetGlobalJointMatrix(*skeletalItem.m_animation,
                                                          skeletalItem.m_skeleton->m_bones[i]);
            bones[i] = jointTransform * skeletalItem.m_skeleton->m_inverseBindMatrices[i];
        }

        shader->SetSkeletonData(bones);

        glDrawElements(GL_TRIANGLES, p_bindings.GetBoundMesh()->GetIndexBufferSize(), GL_UNSIGNED_INT, 0);
    }

    p_bindings.TempInvalidate();
}