#pragma once

// This is a very minimal debug drawer
// Eventually I want to strip this out and replace it with something more robust

#include "CG/DebugDraw/CG_DebugDefines.h"
#if CG_DEBUG_DRAW_ENABLED

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Types/Math/YK_Integer.h"

#include "CG/Resource/Mesh/CG_Mesh.h"

#include <vector>

class CG_RenderBinding;
class CG_RenderTarget;
struct CG_Camera;

struct CG_DebugDrawEntry
{
    YK_Transform const m_transform;
    CG_Mesh const* const m_mesh;
    YK_SizeT m_lifetime;
};

class CG_DebugDrawer
{
public:
    CG_DebugDrawer();
    ~CG_DebugDrawer();

    void AddSphere(YK_Vector3f const& p_position, float p_radius);

    void Render(CG_RenderTarget const& p_target, CG_RenderBinding& p_bindings, CG_Camera const& p_camera) const;

private:
    mutable std::vector<CG_DebugDrawEntry> m_queue;
};

namespace YK_Debug
{
    CG_DebugDrawer& GetDrawer();
}

#endif CG_DEBUG_DRAW_ENABLED