#pragma once

#include "YK/Types/Math/YK_Integer.h"
#include "YK/Types/Math/YK_Matrix.h"

#include <vector>

class CG_Material;
class CG_Mesh;
struct CG_Skeleton;
struct YK_TransformComponent;

class CG_RenderQueue
{
public:
    void Clear() { m_queue.clear(); }
    void Allocate(YK_SizeT p_count)
    {
        m_queue.reserve(p_count);
        m_skeletalQueue.reserve(p_count);
    }
    void Push(CG_Material const& p_material,
              CG_Mesh const& p_mesh,
              CG_Skeleton const* p_skeleton,
              YK_TransformComponent const& p_transform);
    void Bake();

    struct Entry
    {
        CG_Material const* m_material;
        CG_Mesh const* m_mesh;
        YK_Matrix44 m_transform;
    };

    struct SkeletalEntry : public Entry
    {
        CG_Skeleton const* m_skeleton;
    };

    std::vector<Entry> const& GetQueue() const { return m_queue; }
    std::vector<SkeletalEntry> const& GetSkeletalQueue() const { return m_skeletalQueue; }

private:
    std::vector<Entry> m_queue;
    std::vector<SkeletalEntry> m_skeletalQueue;
};