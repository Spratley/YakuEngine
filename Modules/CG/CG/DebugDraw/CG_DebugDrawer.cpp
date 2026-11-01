#include "PCH/CG_PCH.h"
#include "CG_DebugDrawer.h"

#include "CG/DebugDraw/CG_DebugDefines.h"
#if CG_DEBUG_DRAW_ENABLED

#include "YK/Core/YK_Core.h"
#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Platforms/YK_PlatformDefines.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Matrix.h"
#include "YK/Types/Math/YK_Quaternion.h"
#include "YK/Types/Math/YK_Vector.h"

#include "CG/Camera/CG_CameraComponent.h"
#include "CG/RenderTarget/CG_RenderTarget.h"
#include "CG/Renderer/CG_RenderBinding.h"
#include "CG/Renderer/CG_RenderQueue.h"
#include "CG/Resource/Material/CG_Material.h"
#include "CG/Resource/Material/CG_MaterialLoader.h"
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG/Resource/Mesh/CG_MeshLoader.h"
#include "CG/Resource/Shader/CG_Shader.h"

#if YK_PLATFORM == YK_WASM
// Emscripten specific GL headers
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <YK/Libraries/OpenGL/GLAD/include/glad/glad.h>
#endif

#include <sstream>
#include <string>

namespace CG_DebugDrawer_Private
{
    CG_Material DebugMaterial;

    // TODO: Make constexpr
    CG_Mesh PrimitiveSphere;

    CG_DebugDrawer* Instance = nullptr;
} // namespace CG_DebugDrawer_Private

CG_DebugDrawer::CG_DebugDrawer()
{
    YK_ASSERT(!CG_DebugDrawer_Private::Instance, "Overwriting existing debug drawer instance!");
    CG_DebugDrawer_Private::Instance = this;

    // Load resources.. Temporary, this should be made constexpr #embed
    // Too bad it's not universally supported yet...
    // C++26 I love you

    std::string sphereData =
#include "Resources/CG_DebugSphere"
      ;
    std::stringstream dataStream(sphereData);
    CG_DebugDrawer_Private::PrimitiveSphere = CG_MeshLoader::ParseOBJ(dataStream);

    CG_DebugDrawer_Private::DebugMaterial = CG_MaterialLoader::Load(YK_FilePath("Materials/Error.YKM"));
}

CG_DebugDrawer::~CG_DebugDrawer()
{
    YK_ASSERT(CG_DebugDrawer_Private::Instance == this, "Somehow, instance was corrupted!");
    CG_DebugDrawer_Private ::Instance = nullptr;
}

void CG_DebugDrawer::AddSphere(YK_Vector3f const& p_position, float p_radius)
{
    m_queue.push_back(CG_DebugDrawEntry{
      .m_transform = YK_Transform{ p_position, YK_Quaternion{}, YK_Vector3f{ p_radius } },
      .m_mesh = &CG_DebugDrawer_Private::PrimitiveSphere,
      .m_lifetime = 0u
    });
}

void CG_DebugDrawer::Render(CG_RenderTarget const& p_target,
                            CG_RenderBinding& p_bindings,
                            CG_Camera const& p_camera) const
{
    if (m_queue.empty())
    {
        return;
    }

    CG_RenderQueueBuilder renderQueueBuilder;
    for (CG_DebugDrawEntry const& debugDrawEntry : m_queue)
    {
        renderQueueBuilder.PushDebugEntry(debugDrawEntry.m_transform,
                                          CG_DebugDrawer_Private::DebugMaterial,
                                          *debugDrawEntry.m_mesh);
    }
    CG_RenderQueue renderQueue = renderQueueBuilder.Build();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    p_target.Bind();
    YK_Matrix44 const cameraMatrix = p_camera.CalculateCameraMatrix(p_target.GetAspectRatio());

    YK_SizeT itemIndex = 0;
    YK_SizeT meshEntryIndex = 0;
    for (auto const& [material, materialEndIndex] : renderQueue.m_materialEntries)
    {
        p_bindings.Bind(*material);
        CG_Shader const& shader = *(material->m_shader);

        YK_SizeT endIndex;
        do
        {
            auto const& [mesh, meshEndIndex] = renderQueue.m_meshEntries[meshEntryIndex++];
            p_bindings.Bind(*mesh);
            endIndex = meshEndIndex;
            YK_Matrix44 perspectiveTransform;

            for (; itemIndex <= endIndex; ++itemIndex)
            {
                perspectiveTransform = cameraMatrix * renderQueue.m_transforms[itemIndex];
                shader.SetMatrix44("u_mvp", perspectiveTransform.GetData());
                glDrawElements(GL_TRIANGLES, p_bindings.GetBoundMesh()->GetIndexBufferSize(), GL_UNSIGNED_INT, 0);
            }

        } while (endIndex < materialEndIndex);
    }

    m_queue.clear();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

CG_DebugDrawer& YK_Debug::GetDrawer() { return *CG_DebugDrawer_Private::Instance; }
#endif // CG_DEBUG_DRAW_ENABLED