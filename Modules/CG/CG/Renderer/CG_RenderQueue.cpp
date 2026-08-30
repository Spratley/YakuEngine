#include "PCH/CG_PCH.h"
#include "CG_RenderQueue.h"

#include "CG/Resource/Material/CG_Material.h"
#include "CG/Resource/Mesh/CG_Mesh.h"

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Math/YK_MatrixMath.h"

#include <algorithm>
#include <functional>

void CG_RenderQueue::Push(CG_Material const& p_material,
                          CG_Mesh const& p_mesh,
                          YK_TransformComponent const& p_transform)
{
    m_queue.emplace_back(Entry{
      .m_material = &p_material,
      .m_mesh = &p_mesh,
      .m_transform = YK_Matrix::Construct(p_transform.m_position, p_transform.m_orientation, p_transform.m_scale) });
}

void CG_RenderQueue::Bake()
{
    std::less<void const*> lowerAddress;
    std::sort(m_queue.begin(), m_queue.end(), [&lowerAddress](Entry const& p_lhs, Entry const& p_rhs) {
        if (p_lhs.m_material != p_rhs.m_material)
        {
            return lowerAddress(p_lhs.m_material, p_rhs.m_material);
        }
        return lowerAddress(p_lhs.m_mesh, p_rhs.m_mesh);
    });
}