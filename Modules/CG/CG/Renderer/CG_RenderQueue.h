#pragma once

#include "YK/Math/YK_NumericLimits.h"
#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Matrix.h"

#include "CG/ECS/CG_Components.h"

#include <vector>

class CG_Material;
class CG_Mesh;
struct CG_Skeleton;
struct YK_TransformComponent;

struct CG_Animation;

struct CG_RenderQueue
{
    friend class CG_RenderQueueBuilder;

    template <typename DataType>
    struct Entry
    {
        DataType const* m_data = nullptr;
        YK_SizeT m_endIndex = YK_NumericLimits<YK_SizeT>::Max;
    };
    using MaterialEntry = Entry<CG_Material>;
    using MeshEntry = Entry<CG_Mesh>;

    std::vector<MaterialEntry> m_materialEntries;
    std::vector<MeshEntry> m_meshEntries;
    std::vector<YK_Matrix44> m_transforms;
    YK_SizeT m_itemCount = 0;
};

class CG_RenderQueueBuilder
{
public:
    CG_RenderQueue const Build();

    void PushEntry(YK_TransformComponent const& p_transform, CG_RendererComponent const& p_rendererComponent);
    void SetMesh(CG_MeshComponent const& p_mesh);
    void SetMesh(CG_SkeletalMeshComponent const& p_skeletalMesh, CG_PoseComponent const& p_pose);

private:
    struct Entry
    {
        YK_TransformComponent const* m_transform = nullptr;
        CG_Material const* m_material = nullptr;
        CG_Mesh const* m_mesh = nullptr;
        CG_Skeleton const* m_skeleton = nullptr;
        CG_PoseComponent const* m_pose = nullptr;
    };

    Entry* m_workingEntry = nullptr;
    std::vector<Entry> m_entries;
    YK_SizeT m_bonesInQueue = 0;
};