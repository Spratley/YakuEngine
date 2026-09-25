#include "PCH/CG_PCH.h"
#include "CG_RenderQueue.h"

#include "YK/ECS/Components/YK_TransformComponent.h"
#include "YK/Math/YK_MatrixMath.h"
#include "YK/Math/YK_NumericLimits.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Matrix.h"
#include "YK/Utils/YK_AlgorithmUtils.h"

#include "CG/ECS/CG_Components.h"
#include "CG/Resource/Material/CG_Material.h"
#include "CG/Resource/Mesh/CG_Mesh.h"
#include "CG/Resource/Skeleton/CG_Skeleton.h"

#include <algorithm>
#include <functional>
#include <vector>

namespace CG_RenderQueue_Private
{
    template <typename EntryType>
    void SortEntries(std::vector<EntryType>& p_entries)
    {
        std::less<void const*> lowerAddress;
        std::sort(p_entries.begin(), p_entries.end(), [&lowerAddress](EntryType const& p_lhs, EntryType const& p_rhs) {
            if (p_lhs.m_material != p_rhs.m_material)
            {
                return lowerAddress(p_lhs.m_material, p_rhs.m_material);
            }
            return lowerAddress(p_lhs.m_mesh, p_rhs.m_mesh);
        });
    }

    template <typename T>
    struct RenderAttributeQueueHelper
    {
        T const* m_test = nullptr;

        bool PushToQueue(T const* p_item, std::vector<CG_RenderQueue::Entry<T>>& p_entryArray, YK_SizeT p_index)
        {
            if (m_test == p_item)
            {
                return false;
            }
            if (p_entryArray.size() > 0)
            {
                p_entryArray.back().m_endIndex = p_index - 1;
            }
            m_test = p_item;
            p_entryArray.push_back(
              CG_RenderQueue::Entry<T>{ .m_data = p_item, .m_endIndex = YK_NumericLimits<YK_SizeT>::Max });
            return true;
        }

        constexpr void Invalidate() { m_test = nullptr; }
    };

    // TODO: This needs major improvement
    // This doesn't cache the bone matrix yet, so we're recalculating the entire hierarchy per bone
    // 100% bad, but also good enough for the scale this is operating at FOR NOW
    YK_Matrix44 CalculateBoneMatrix(CG_PoseComponent const& p_pose, CG_Skeleton::Bone const& p_bone)
    {
        YK_TransformComponent const& boneTransform = p_pose.m_pose[p_bone.m_index];
        YK_Matrix44 result =
          YK_Matrix::Construct(boneTransform.m_position, boneTransform.m_orientation, boneTransform.m_scale);
        if (p_bone.m_parent)
        {
            result = CalculateBoneMatrix(p_pose, *p_bone.m_parent) * result;
        }
        return result;
    }
} // namespace CG_RenderQueue_Private

CG_RenderQueue const CG_RenderQueueBuilder::Build()
{
    CG_RenderQueue_Private::SortEntries(m_entries);

    CG_RenderQueue renderQueue;
    renderQueue.m_itemCount = m_entries.size();
    renderQueue.m_transforms.resize(m_entries.size() + m_bonesInQueue);

    CG_RenderQueue_Private::RenderAttributeQueueHelper<CG_Material> materialHelper;
    CG_RenderQueue_Private::RenderAttributeQueueHelper<CG_Mesh> meshHelper;

    YK_SizeT transformIndex = 0;
    for (auto i : YK_CountTo(m_entries.size()))
    {
        Entry const& entry = m_entries[i];
        if (materialHelper.PushToQueue(entry.m_material, renderQueue.m_materialEntries, transformIndex))
        {
            meshHelper.Invalidate();
        }
        meshHelper.PushToQueue(entry.m_mesh, renderQueue.m_meshEntries, transformIndex);

        renderQueue.m_transforms[transformIndex] = YK_Matrix::Construct(entry.m_transform->m_position,
                                                                        entry.m_transform->m_orientation,
                                                                        entry.m_transform->m_scale);

        if (entry.m_skeleton && entry.m_pose)
        {
            for (auto boneIndex : YK_CountTo(entry.m_skeleton->m_bones.size()))
            {
                CG_Skeleton::Bone const& bone = entry.m_skeleton->m_bones[boneIndex];
                YK_Matrix44 boneMatrix = CG_RenderQueue_Private::CalculateBoneMatrix(*entry.m_pose, bone);
                transformIndex++;
                renderQueue.m_transforms[transformIndex] = boneMatrix * entry.m_skeleton->m_inverseBindMatrices[boneIndex];
            }
        }

        ++transformIndex;
    }

    YK_SizeT const lastIndex = renderQueue.m_itemCount - 1;
    renderQueue.m_materialEntries.back().m_endIndex = lastIndex;
    renderQueue.m_meshEntries.back().m_endIndex = lastIndex;

    return renderQueue;
}

void CG_RenderQueueBuilder::PushEntry(YK_TransformComponent const& p_transform,
                                      CG_RendererComponent const& p_rendererComponent)
{
    YK_SizeT index = m_entries.size();
    m_entries.push_back(Entry{ .m_transform = &p_transform, .m_material = p_rendererComponent.m_material });
    m_workingEntry = &m_entries[index];
}

void CG_RenderQueueBuilder::SetMesh(CG_MeshComponent const& p_mesh) { m_workingEntry->m_mesh = p_mesh.m_mesh; }

void CG_RenderQueueBuilder::SetMesh(CG_SkeletalMeshComponent const& p_skeletalMesh, CG_PoseComponent const& p_pose)
{
    YK_ASSERT(!m_workingEntry->m_skeleton, "Overwriting skeleton data!");

    m_workingEntry->m_mesh = p_skeletalMesh.m_mesh;
    m_workingEntry->m_skeleton = p_skeletalMesh.m_skeleton;
    m_workingEntry->m_pose = &p_pose;

    m_bonesInQueue += p_skeletalMesh.m_skeleton->m_bones.size();
}