#include "PCH/CG_PCH.h"
#include "CG_RenderQueue.h"

#include "CG/Resource/Material/CG_Material.h"
#include "CG/Resource/Mesh/CG_Mesh.h"

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Math/YK_MatrixMath.h"

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

namespace CG_RenderQueue_Private
{
    template <typename EntryType>
    void SortQueue(std::vector<EntryType>& p_queue)
    {
        std::less<void const*> lowerAddress;
        std::sort(p_queue.begin(), p_queue.end(), [&lowerAddress](EntryType const& p_lhs, EntryType const& p_rhs) {
            if (p_lhs.m_material != p_rhs.m_material)
            {
                return lowerAddress(p_lhs.m_material, p_rhs.m_material);
            }
            return lowerAddress(p_lhs.m_mesh, p_rhs.m_mesh);
        });
    }
} // namespace CG_RenderQueue_Private

void CG_RenderQueue::Push(CG_Material const& p_material,
                          CG_Mesh const& p_mesh,
                          CG_Skeleton const* p_skeleton,
                          YK_TransformComponent const& p_transform)
{
    if (p_skeleton)
    {
        SkeletalEntry entry;
        entry.m_material = &p_material;
        entry.m_mesh = &p_mesh;
        entry.m_transform =
          YK_Matrix::Construct(p_transform.m_position, p_transform.m_orientation, p_transform.m_scale);
        entry.m_skeleton = p_skeleton;
        m_skeletalQueue.emplace_back(std::move(entry));
    }
    else
    {
        m_queue.emplace_back(
          Entry{ .m_material = &p_material,
                 .m_mesh = &p_mesh,
                 .m_transform =
                   YK_Matrix::Construct(p_transform.m_position, p_transform.m_orientation, p_transform.m_scale) });
    }
}

void CG_RenderQueue::Bake()
{
    CG_RenderQueue_Private::SortQueue(m_queue);
    CG_RenderQueue_Private::SortQueue(m_skeletalQueue);
}